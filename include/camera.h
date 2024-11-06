#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <stdbool.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>


// Default initial setup
#define CAMERA_POSITION     (vec3){0.0f, 0.0f, 3.0f}
#define CAMERA_FRONT        (vec3){0.0f, 0.0f,-1.0f}
#define CAMERA_UP           (vec3){0.0f, 1.0f, 0.0f}
#define CAMERA_RIGHT        (vec3){1.0f, 0.0f, 0.0f}
    
// Default angles
#define CAMERA_YAW          -90.0f;   // Looking forward
#define CAMERA_PITCH        0.0f;
    
// Default parameters
#define CAMERA_SPEED        0.05f;
#define CAMERA_SENSITIVITY  0.1f;
#define CAMERA_FOV          45.0f; //70.0f;
#define CAMERA_HEIGHT       1.8f;  //1.7f;  // Typical FPS eye height




typedef enum {
    FPS = 0,
    SPECTATOR
} CameraType;

typedef struct {
    vec3 position;      // Camera position
    vec3 front;         // Direction camera is facing
    vec3 up;            // Up vector
    vec3 right;         // Right vector
    float yaw;          // Rotation around Y axis
    float pitch;        // Rotation around X axis
    
    // Camera parameters
    float speed;        // Movement speed
    float sensitivity;  // Mouse sensitivity
    float fov;          // Field of view
    
    // Camera modes
    bool spectator_mode; // True for spectator, false for FPS
    float height;        // Eye height for FPS mode
    bool is_jumping;     // Jump state for FPS mode
    float jump_velocity; // Current jump velocity
    float last_mouse_x;
    float last_mouse_y;
    mat4 view;
    mat4 projection;
    bool ons;
} Camera;

Camera *create_camera(vec3 position);
void update_camera(Camera *camera, GLFWwindow *w);
void set_camera_position(Camera *camera, vec3 position);

#endif // _CAMERA_H_