#include "camera.h"
#include <stdlib.h>

Camera *create_camera() 
{
    Camera *c = (Camera *) malloc(sizeof(Camera));
    if (c == NULL) {
        return NULL;
    }

    // Initial position and orientation
    glm_vec3_copy(CAMERA_POSITION, c->position);
    glm_vec3_copy(CAMERA_FRONT, c->front);
    glm_vec3_copy(CAMERA_UP, c->up);
    glm_vec3_copy(CAMERA_RIGHT, c->right);
    
    // Default angles
    c->yaw = CAMERA_YAW;
    c->pitch = CAMERA_PITCH;
    
    // Default parameters
    c->speed = CAMERA_SPEED;
    c->sensitivity = CAMERA_SENSITIVITY;
    c->fov = CAMERA_FOV;
    
    // FPS mode defaults
    c->height = CAMERA_HEIGHT;
    c->spectator_mode = false;
    c->is_jumping = false;
    c->jump_velocity = 0.0f;

    return c;
}

void get_camera_view(Camera *c, vec3 position, mat4 m)
{
    if (position) {
        glm_vec3_copy(position, c->position);
    }
    glm_lookat(c->position, CAMERA_FRONT, CAMERA_UP, m);
}