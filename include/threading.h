#ifndef THREADING_H
#define THREADING_H

#include <windows.h>
#include "vertex.h"
#include "utils.h"

// Thread job structure for vertex transformations
typedef struct {
    vec3* vertices;      // Input/output vertex array
    int start;           // Start index for this thread
    int end;             // End index for this thread
    float scale;         // Scale factor
    vec3 rotation;       // Rotation in degrees
    vec3 translation;    // Translation vector
} VertexTransformJob;

// Thread job structure for triangle processing
typedef struct {
    vec3* vertices;      // Vertex array (already transformed)
    int* indices;        // Index array
    int startTri;        // Start triangle index
    int endTri;          // End triangle index
    float nearPlane;
    float farPlane;
    float fov;
    Camera cam;
    Frag* frag;
    int WIDTH;
    int HEIGHT;
    CRITICAL_SECTION* fragLock;  // Lock for thread-safe fragment buffer access
} TriangleProcessJob;

// Worker thread function for triangle processing
DWORD WINAPI triangleProcessThread(LPVOID param) {
    TriangleProcessJob* job = (TriangleProcessJob*)param;
    
    for (int i = job->startTri; i < job->endTri; i++) {
        tri tri1 = triangleBuild(job->vertices, job->indices, i);
        
        vec3 norm = normal(tri1.v1, tri1.v2, tri1.v3);
        
        vec3 distance_to_cam = {
            tri1.v1.x - job->cam.position.x,
            tri1.v1.y - job->cam.position.y,
            tri1.v1.z - job->cam.position.z
        };
        
        if (dot(norm, distance_to_cam) < 0) {
            tri tri2 = tri1;
            
            // Move to camera space
            view(&tri2.v1, 1, job->cam.position, job->cam.rotation);
            view(&tri2.v2, 1, job->cam.position, job->cam.rotation);
            view(&tri2.v3, 1, job->cam.position, job->cam.rotation);
            
            // Project the triangle vertices
            proj(&tri2.v1, job->farPlane, job->nearPlane, job->fov, job->WIDTH, job->HEIGHT);
            proj(&tri2.v2, job->farPlane, job->nearPlane, job->fov, job->WIDTH, job->HEIGHT);
            proj(&tri2.v3, job->farPlane, job->nearPlane, job->fov, job->WIDTH, job->HEIGHT);
            
            tri2.v1 = toTerminal(tri2.v1, job->WIDTH, job->HEIGHT);
            tri2.v2 = toTerminal(tri2.v2, job->WIDTH, job->HEIGHT);
            tri2.v3 = toTerminal(tri2.v3, job->WIDTH, job->HEIGHT);
            
            List* list = createList();
            
            drawTriangle(tri2, &list, job->WIDTH, job->HEIGHT);
            fillTriangle(&list, job->WIDTH, job->HEIGHT);
            
            // Lock before writing to shared fragment buffer
            EnterCriticalSection(job->fragLock);
            placeFrag(job->frag, list, job->WIDTH, job->HEIGHT);
            LeaveCriticalSection(job->fragLock);
        }
    }
    
    return 0;
}

// Parallel triangle processing
void parallelDrawTriangles(vec3 vertices[], int indices[], int shapes, float nearPlane, float farPlane, 
                          float fov, Camera cam, Frag* frag, int WIDTH, int HEIGHT, int threadCount) {
    
    if (threadCount <= 1 || shapes < 50) {
        // For small triangle counts, use single-threaded approach
        for (int i = 0; i < shapes; i++) {
            tri tri1 = triangleBuild(vertices, indices, i);
            vec3 norm = normal(tri1.v1, tri1.v2, tri1.v3);
            vec3 distance_to_cam = {tri1.v1.x - cam.position.x, tri1.v1.y - cam.position.y, tri1.v1.z - cam.position.z};
            
            if (dot(norm, distance_to_cam) < 0) {
                tri tri2 = tri1;
                view(&tri2.v1, 1, cam.position, cam.rotation);
                view(&tri2.v2, 1, cam.position, cam.rotation);
                view(&tri2.v3, 1, cam.position, cam.rotation);
                
                proj(&tri2.v1, farPlane, nearPlane, fov, WIDTH, HEIGHT);
                proj(&tri2.v2, farPlane, nearPlane, fov, WIDTH, HEIGHT);
                proj(&tri2.v3, farPlane, nearPlane, fov, WIDTH, HEIGHT);
                
                tri2.v1 = toTerminal(tri2.v1, WIDTH, HEIGHT);
                tri2.v2 = toTerminal(tri2.v2, WIDTH, HEIGHT);
                tri2.v3 = toTerminal(tri2.v3, WIDTH, HEIGHT);
                
                List* list = createList();
                drawTriangle(tri2, &list, WIDTH, HEIGHT);
                fillTriangle(&list, WIDTH, HEIGHT);
                placeFrag(frag, list, WIDTH, HEIGHT);
            }
        }
        return;
    }
    
    // Initialize critical section for fragment buffer synchronization
    CRITICAL_SECTION fragLock;
    InitializeCriticalSection(&fragLock);
    
    // Allocate thread handles and job structures
    HANDLE* threads = (HANDLE*)malloc(sizeof(HANDLE) * threadCount);
    TriangleProcessJob* jobs = (TriangleProcessJob*)malloc(sizeof(TriangleProcessJob) * threadCount);
    
    if (!threads || !jobs) {
        if (threads) free(threads);
        if (jobs) free(jobs);
        DeleteCriticalSection(&fragLock);
        // Fallback handled above
        return;
    }
    
    // Calculate chunk size for each thread
    int chunkSize = shapes / threadCount;
    int remainder = shapes % threadCount;
    
    // Create and launch threads
    int currentStart = 0;
    for (int i = 0; i < threadCount; i++) {
        int currentChunkSize = chunkSize + (i < remainder ? 1 : 0);
        
        jobs[i].vertices = vertices;
        jobs[i].indices = indices;
        jobs[i].startTri = currentStart;
        jobs[i].endTri = currentStart + currentChunkSize;
        jobs[i].nearPlane = nearPlane;
        jobs[i].farPlane = farPlane;
        jobs[i].fov = fov;
        jobs[i].cam = cam;
        jobs[i].frag = frag;
        jobs[i].WIDTH = WIDTH;
        jobs[i].HEIGHT = HEIGHT;
        jobs[i].fragLock = &fragLock;
        
        threads[i] = CreateThread(NULL, 0, triangleProcessThread, &jobs[i], 0, NULL);
        
        if (threads[i] == NULL) {
            for (int j = 0; j < i; j++) {
                WaitForSingleObject(threads[j], INFINITE);
                CloseHandle(threads[j]);
            }
            free(threads);
            free(jobs);
            DeleteCriticalSection(&fragLock);
            return;
        }
        
        currentStart += currentChunkSize;
    }
    
    // Wait for all threads to complete
    WaitForMultipleObjects(threadCount, threads, TRUE, INFINITE);
    
    // Clean up
    for (int i = 0; i < threadCount; i++) {
        CloseHandle(threads[i]);
    }
    free(threads);
    free(jobs);
    DeleteCriticalSection(&fragLock);
}

// Worker thread function for vertex transformation
DWORD WINAPI vertexTransformThread(LPVOID param) {
    VertexTransformJob* job = (VertexTransformJob*)param;
    
    // Apply transformations to this thread's chunk
    // 1. Scale
    for (int i = job->start; i < job->end; i++) {
        job->vertices[i].x *= job->scale;
        job->vertices[i].y *= job->scale;
        job->vertices[i].z *= job->scale;
    }
    
    // 2. Rotation
    float radX = job->rotation.x * (M_PI / 180.0f);
    float radY = job->rotation.y * (M_PI / 180.0f);
    float radZ = job->rotation.z * (M_PI / 180.0f);
    
    mat3 transform = matTransform(rotateX(radX), rotateY(radY), rotateZ(radZ));
    
    for (int i = job->start; i < job->end; i++) {
        vec3 temp = job->vertices[i];
        vec3 out;
        out.x = temp.x * transform.matrix[0][0] + temp.y * transform.matrix[0][1] + temp.z * transform.matrix[0][2];
        out.y = temp.x * transform.matrix[1][0] + temp.y * transform.matrix[1][1] + temp.z * transform.matrix[1][2];
        out.z = temp.x * transform.matrix[2][0] + temp.y * transform.matrix[2][1] + temp.z * transform.matrix[2][2];
        job->vertices[i] = out;
    }
    
    // 3. Translation
    for (int i = job->start; i < job->end; i++) {
        job->vertices[i].x += job->translation.x;
        job->vertices[i].y += job->translation.y;
        job->vertices[i].z += job->translation.z;
    }
    
    return 0;
}

// Parallel version of modelTransform
void parallelModelTransform(vec3 vert[], int size, float factor, vec3 deg, vec3 trans, int threadCount) {
    if (threadCount <= 1 || size < 100) {
        // For small vertex counts or single thread, use original function
        modelTransform(vert, size, factor, deg, trans);
        return;
    }
    
    // Limit thread count to reasonable maximum
    if (threadCount > 16) threadCount = 16;
    
    // Allocate thread handles and job structures
    HANDLE* threads = (HANDLE*)malloc(sizeof(HANDLE) * threadCount);
    VertexTransformJob* jobs = (VertexTransformJob*)malloc(sizeof(VertexTransformJob) * threadCount);
    
    if (!threads || !jobs) {
        // Fallback to single-threaded if allocation fails
        if (threads) free(threads);
        if (jobs) free(jobs);
        modelTransform(vert, size, factor, deg, trans);
        return;
    }
    
    // Calculate chunk size for each thread
    int chunkSize = size / threadCount;
    int remainder = size % threadCount;
    
    // Create and launch threads
    int currentStart = 0;
    for (int i = 0; i < threadCount; i++) {
        int currentChunkSize = chunkSize + (i < remainder ? 1 : 0);
        
        jobs[i].vertices = vert;
        jobs[i].start = currentStart;
        jobs[i].end = currentStart + currentChunkSize;
        jobs[i].scale = factor;
        jobs[i].rotation = deg;
        jobs[i].translation = trans;
        
        threads[i] = CreateThread(
            NULL,                           // Default security attributes
            0,                              // Default stack size
            vertexTransformThread,          // Thread function
            &jobs[i],                       // Thread parameter
            0,                              // Default creation flags
            NULL                            // Don't need thread ID
        );
        
        if (threads[i] == NULL) {
            // If thread creation fails, wait for already created threads
            for (int j = 0; j < i; j++) {
                WaitForSingleObject(threads[j], INFINITE);
                CloseHandle(threads[j]);
            }
            free(threads);
            free(jobs);
            // Complete remaining work in single-threaded mode
            modelTransform(vert + currentStart, size - currentStart, factor, deg, trans);
            return;
        }
        
        currentStart += currentChunkSize;
    }
    
    // Wait for all threads to complete
    WaitForMultipleObjects(threadCount, threads, TRUE, INFINITE);
    
    // Clean up
    for (int i = 0; i < threadCount; i++) {
        CloseHandle(threads[i]);
    }
    free(threads);
    free(jobs);
}

// Get optimal thread count for the system
int getOptimalThreadCount() {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    int numCPU = sysinfo.dwNumberOfProcessors;
    
    // Use 75% of available cores for better performance
    // Leave some cores for OS and other tasks
    int threadCount = (numCPU * 3) / 4;
    if (threadCount < 1) threadCount = 1;
    if (threadCount > 8) threadCount = 8;  // Cap at 8 threads
    
    return threadCount;
}

#endif
