#include <GLFW/glfw3.h>

#include "loki.h"
#include "tools/log.h"


void init_engine_time(EngineTime* time) 
{
    time->last_frame_time = glfwGetTime();
    time->delta_time = 0.0;
    time->fixed_time_step = 1.0 / 60.0; // 60 FPS fixed update
    time->accumulator = 0.0;
    
    // FPS initialization
    time->fps_update_time = time->last_frame_time;
    time->frame_count = 0;
    time->fps = 0.0;
    time->fixed_fps = 0.0;
}

void update_delta_time(EngineTime* time) 
{
    double current_time = glfwGetTime();
    time->delta_time = current_time - time->last_frame_time;
    
    // Cap maximum delta time to prevent spiral of death
    if (time->delta_time > 0.25) {
        time->delta_time = 0.25;
    }
    
    time->last_frame_time = current_time;
    time->accumulator += time->delta_time;
    
    // Update FPS counter every second
    time->frame_count++;
    if (current_time - time->fps_update_time >= 1.0) {
        time->fps = (double)time->frame_count / (current_time - time->fps_update_time);
        time->fps_update_time = current_time;
        time->frame_count = 0;

    }
}

bool should_fixed_update(EngineTime* time) 
{
    if (time->accumulator >= time->fixed_time_step) {
        time->accumulator -= time->fixed_time_step;
        time->fixed_fps =  (double)time->frame_count / (glfwGetTime() - time->fixed_time_step);
        return true;
    }
    return false;
}