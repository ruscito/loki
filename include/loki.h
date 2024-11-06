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
    bool is_mouse_captured;
    Mouse mouse;
    Camera *camera;
    bool update_prospective;
    float screen_width;
    float screen_height;
} EngineState;


#endif // _loki_H_