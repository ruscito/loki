#include "camera.h"
#include "loki.h"
#include "tools/log.h"
#include <stdlib.h>

const float MAX_PITCH = 89;
extern EngineState engine; 

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
    vec3 target;
    glm_vec3_add(c->position, c->front, target);
    glm_lookat(c->position, target, c->up, c->view);

    glm_mat4_identity(c->projection);
    glm_perspective(glm_rad(c->fov), engine.screen_width / engine.screen_height, 0.1f, 100.0f, c->projection);
    c->update_zoom = false;
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
    // Keyboard management
    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) {
        vec3 move;
        glm_vec3_scale(camera->front, camera->speed, move);
        glm_vec3_add(camera->position, move, camera->position);
    }
    if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) {
        vec3 move;
        glm_vec3_scale(camera->front, camera->speed, move);
        glm_vec3_sub(camera->position, move, camera->position);
    }
    if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 right, move;
        glm_cross(camera->front, camera->up, right);
        glm_normalize(right);
        glm_vec3_scale(right, camera->speed, move);
        glm_vec3_sub(camera->position, move, camera->position);
    }
    if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 right, move;
        glm_cross(camera->front, camera->up, right);
        glm_normalize(right);
        glm_vec3_scale(right, camera->speed, move);
        glm_vec3_add(camera->position, move, camera->position);
    }
    
    // Mouse management
    float x_offset = ((float)engine.mouse.x - camera->last_mouse_x) * camera->sensitivity;
    float y_offset = (camera->last_mouse_y - (float)engine.mouse.y) * camera->sensitivity;
    camera->last_mouse_x = (float)engine.mouse.x;
    camera->last_mouse_y = (float)engine.mouse.y;

    camera->yaw += x_offset;
    camera->pitch += y_offset;

    // Constrain pitch
    if (camera->pitch > 89.0f) camera->pitch = 89.0f;
    if (camera->pitch < -89.0f) camera->pitch = -89.0f;

    // Calculate the new front vector
    camera->front[0] = cosf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
    camera->front[1] = sinf(glm_rad(camera->pitch));
    camera->front[2] = sinf(glm_rad(camera->yaw)) * cosf(glm_rad(camera->pitch));
    glm_normalize(camera->front);

    // Zoom handling
    if (camera->update_zoom) {
        camera->fov -= engine.mouse.scroll_y;
        if (camera->fov < 1.0f) {
            camera->fov = 1.0f;
        }
        if (camera->fov > 45.0f){
            camera->fov = CAMERA_FOV;
        }
        glm_perspective(glm_rad(camera->fov), engine.screen_width / engine.screen_height, 0.1f, 100.0f, camera->projection);
        camera->update_zoom = false;
        engine.update_prospective = true;
    }


    // Calculate matrix view
    vec3 target;
    glm_vec3_add(camera->position, camera->front, target);
    glm_lookat(camera->position, target, camera->up, camera->view);
}