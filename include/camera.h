#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>
#include "utils.h"

typedef struct{
    vec3 position;
    vec3 rotation;     // Euler degrees: pitch (x), yaw (y), roll (z)
    float fov_deg;
    float near_plane;
    float far_plane;
    float move_speed;
    float look_speed;
} Camera;

static inline float cam_deg2rad(float deg){
    return deg * (float)M_PI / 180.0f;
}

static inline vec3 camera_forward(const Camera *cam){
    float pitch = cam_deg2rad(cam->rotation.x);
    float yaw   = cam_deg2rad(cam->rotation.y);
    vec3 f = {
        cosf(pitch) * sinf(yaw),
        -sinf(pitch),
        cosf(pitch) * cosf(yaw)
    };
    return normalize(f);
}

static inline vec3 camera_right(const Camera *cam){
    vec3 up = {0.0f, 1.0f, 0.0f};
    vec3 f = camera_forward(cam);
    vec3 r = {
        f.y * up.z - f.z * up.y,
        f.z * up.x - f.x * up.z,
        f.x * up.y - f.y * up.x
    };
    return normalize(r);
}

static inline vec3 camera_up(const Camera *cam){
    vec3 f = camera_forward(cam);
    vec3 r = camera_right(cam);
    vec3 u = {
        r.y * f.z - r.z * f.y,
        r.z * f.x - r.x * f.z,
        r.x * f.y - r.y * f.x
    };
    return normalize(u);
}

static inline Camera camera_create(vec3 position, vec3 rotation, float fov_deg, float near_plane, float far_plane, float move_speed, float look_speed){
    Camera cam;
    cam.position = position;
    cam.rotation = rotation;
    cam.fov_deg = fov_deg;
    cam.near_plane = near_plane;
    cam.far_plane = far_plane;
    cam.move_speed = move_speed;
    cam.look_speed = look_speed;
    return cam;
}

static inline void camera_move(Camera *cam, float forward, float right, float up, float delta_time){
    vec3 f = camera_forward(cam);
    vec3 r = camera_right(cam);
    vec3 world_up = {0.0f, 1.0f, 0.0f};

    vec3 delta = {
        f.x * forward + r.x * right + world_up.x * up,
        f.y * forward + r.y * right + world_up.y * up,
        f.z * forward + r.z * right + world_up.z * up
    };

    float scale = cam->move_speed * delta_time;
    cam->position.x += delta.x * scale;
    cam->position.y += delta.y * scale;
    cam->position.z += delta.z * scale;
}

static inline void camera_rotate(Camera *cam, float pitch_deg, float yaw_deg, float roll_deg){
    cam->rotation.x += pitch_deg * cam->look_speed;
    cam->rotation.y += yaw_deg * cam->look_speed;
    cam->rotation.z += roll_deg * cam->look_speed;

    // clamp pitch to avoid gimbal lock extremes
    if (cam->rotation.x > 89.0f) cam->rotation.x = 89.0f;
    if (cam->rotation.x < -89.0f) cam->rotation.x = -89.0f;
}

#endif