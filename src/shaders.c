#include "shaders.h"
#include <stdlib.h>
#include <glad/glad.h>
#include <stdbool.h>
#include <string.h>


#define ERROR_MSG_SIZE 1024

static char error_string[ERROR_MSG_SIZE];

static bool check_compile_error(unsigned int shader)
{
    int params = -1;
    strcpy(error_string, "");
    glGetShaderiv(shader, GL_COMPILE_STATUS, &params);

    if (params != GL_TRUE) {
        // set error string
        glGetShaderInfoLog(shader, ERROR_MSG_SIZE, NULL, error_string);
        return true;
    }
    return false;
}

static bool check_link_error(unsigned int program)
{
    int params = -1;
    strcpy(error_string, "");
    glGetProgramiv(program, GL_LINK_STATUS, &params);

    if (params != GL_TRUE) {
        // set error string
        glGetProgramInfoLog(program, ERROR_MSG_SIZE, NULL, error_string);
        return true;
    }
    return false;
}


char* get_shader_error(void)
{
    return error_string;
}

unsigned int create_shader_program(const char *vs, const char* fs)
{
    // Create and compile vertex shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vs, NULL);
    glCompileShader(vertex_shader);
    if (check_compile_error(vertex_shader)) {
        glDeleteShader(vertex_shader);
        return 0;
    }

    // Create and compile fragment shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fs, NULL);
    glCompileShader(fragment_shader);
    if (check_compile_error(fragment_shader)) {
        glDeleteShader(fragment_shader);
        return 0;
    }

    // Create shader program
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    if (check_link_error(shader_program)) {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteProgram(shader_program);
        return 0;
    }

    // Delete shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);   

    return shader_program;
}

void destroy_shader_program(unsigned int sp)
{
    glDeleteProgram(sp);
}