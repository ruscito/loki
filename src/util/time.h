#ifndef _TIME_H_
#define _TIME_H_

#include <stdbool.h>

typedef struct {
    double last_frame_time;
    double delta_time;
    double fixed_time_step;
    double accumulator;
    
    // FPS calculation
    double fps_update_time;
    int frame_count;
    double fps;
    double fixed_fps;
} EngineTime;

void init_engine_time(EngineTime* time, double now);
void update_delta_time(EngineTime* time, double now);
bool should_fixed_update(EngineTime* time, double now);

#endif // _TIME_H_