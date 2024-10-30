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
#include <cglm/cglm.h>


#define PI 3.14159265359f
#define DEG2RAD(X) ((X) * (PI / 180.0f))

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
    float position[3];
    float texCoords[2];
    float normal[3];
    // unsigned int color;
    // unsigned int material_id;
    // float occlusion;
} Vertex;


#endif // _loki_H_