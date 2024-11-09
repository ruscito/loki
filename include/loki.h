/* loki.h */
#ifndef _loki_H_
#define _loki_H_

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#define CGLM_ALL_UNALIGNED
#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "camera.h"

#define PI 3.14159265359f
#define DEG2RAD(X) ((X) * (PI / 180.0f))
#define GL_SILENCE_DEPRECATION

typedef enum {
    AIR = 0,
    STONE,
    DIRT,
    SAND,
    GRASS,
    WATER,
    MAX_VOXEL,
} VoxelType;

typedef struct {
    vec3 position;
    VoxelType type;
} Voxel;

typedef struct {
    float x;
    float y;
    float scroll_x;
    float scroll_y;
} Mouse;


typedef struct {
    float position[3];
    float texCoords[2];
    float normal[3];
    // unsigned int color;
    // unsigned int material_id;
    // float occlusion;
} Vertex;

typedef struct {
    double last_frame_time;
    double delta_time;
    double fixed_time_step;
    double accumulator;
    
    // FPS calculation
    double fps_update_time;
    int frame_count;
    double fps;
} EngineTime;

typedef struct {
    bool is_mouse_captured;
    bool update_prospective;
    float screen_width;
    float screen_height;
    EngineTime time;
} EngineState;


void init_engine_time(EngineTime* time);
void update_delta_time(EngineTime* time);
bool should_fixed_update(EngineTime* time);

#endif // _loki_H_