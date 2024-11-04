#include "camera.h"
#include "tools/log.h"
#include <stdlib.h>

const float MAX_PITCH = 89;

Camera *create_camera(vec3 position) 
{
    Camera *c = (Camera *) malloc(sizeof(Camera));
    if (c == NULL) {
        return NULL;
    }

    // Initial position and orientation
    if (position) {
        glm_vec3_copy(position, c->position);
    } else {
        glm_vec3_copy(CAMERA_POSITION, c->position);
    }
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
    glm_mat4_identity(c->view);
    glm_lookat(c->position, CAMERA_FRONT, CAMERA_UP, c->view);
    c->ons = true;
    return c;
}


void set_camera_position(Camera *camera, vec3 position)
{
        
    if (position){
        vec3 front;
        glm_vec3_copy(position, camera->position);
        glm_vec3_add(position, CAMERA_FRONT, front);
        glm_lookat(camera->position, front, CAMERA_UP, camera->view);
    }
}


void update_camera(Camera *camera, GLFWwindow *w)
{
    // keyboard management
    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
        camera->position[0] += camera->speed * camera->front[0];
        camera->position[1] += camera->speed * camera->front[1];
        camera->position[2] += camera->speed * camera->front[2];
    }
    if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
        camera->position[0] -= camera->speed * camera->front[0];
        camera->position[1] -= camera->speed * camera->front[1];
        camera->position[2] -= camera->speed * camera->front[2];
    }
    if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 right;
        glm_cross(camera->front, camera->up, right);
        glm_normalize(right);
        camera->position[0] -= camera->speed * right[0]; 
        camera->position[1] -= camera->speed * right[1]; 
        camera->position[2] -= camera->speed * right[2]; 
    }
    if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 right;
        glm_cross(camera->front, camera->up, right);
        glm_normalize(right);
        camera->position[0] += camera->speed * right[0]; 
        camera->position[1] += camera->speed * right[1]; 
        camera->position[2] += camera->speed * right[2]; 
    }
    
    // // Mouse management
    // double x_position, y_position;
    // glfwGetCursorPos(w, &x_position, &y_position);
    // if (camera->ons) {
    //     camera->last_mouse_x = (float) x_position;
    //     camera->last_mouse_y = (float) y_position;
    //     camera->ons = false;
    // }

    // if (camera->last_mouse_x != (float) x_position  && camera->last_mouse_y != (float) y_position) {
    //     float mouse_x_offset = (float) x_position - camera->last_mouse_x;
    //     float mouse_y_offset = camera->last_mouse_y - (float) y_position;
    //     camera->last_mouse_x = (float) x_position;
    //     camera->last_mouse_y = (float) y_position;


    //     mouse_x_offset *= camera->sensitivity;
    //     mouse_y_offset *= camera->sensitivity;

    //     camera->yaw += mouse_x_offset;
    //     camera->pitch += mouse_y_offset;

    //     if ( camera->pitch > MAX_PITCH ) camera->pitch = MAX_PITCH;
    //     if ( camera->pitch < MAX_PITCH ) camera->pitch = -MAX_PITCH;

    //     vec3 front;

    //     front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    //     front[1] = sin(glm_rad(camera->pitch));
    //     front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    //     glm_normalize(front);
        
    //     glm_vec3_add(camera->position, front, camera->front);
    //     DEBUG("IN\n");
    // }

    // glm_vec3_add(camera->position, camera->front, camera->front);
    glm_lookat(camera->position, camera->front, CAMERA_UP, camera->view);
}