#include "loki.h"
#include "camera.h"
#include "tools/log.h"
#include "tools/res.h"
#include "shaders.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

// Vertex shader
const char* vertex_shader_src = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "layout (location = 2) in vec3 aNormal;\n"
    "out vec2 TexCoord;\n"
    "out vec3 Normal;\n" 
    "uniform mat4 model;\n"    
    "uniform mat4 view;\n"    
    "uniform mat4 projection;\n"    
    "void main()\n"
    "{\n"
    "   gl_Position = projection* view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "   TexCoord = aTexCoord;\n"
    // "   Normal = aNormal;\n"
    "}\0";

// Fragment shader
const char* fragment_shader_src = "#version 330 core\n"
    "in vec2 TexCoord;\n"
    "in vec3 Normal;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D Texture;\n"
    "void main()\n"
    "{\n"
    "   FragColor = texture(Texture, TexCoord);\n"
    "}\n\0";

Camera *camera = NULL;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main() 
{
    // Initialize GLFW
    log_init();
    if (!glfwInit()) {
        FATAL("Failed to initialize GLFW\n");
        return -1;
    }

    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Voxel Engine", NULL, NULL);
    if (!window) {
        FATAL("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        FATAL("Failed to initialize GLAD\n");
        return -1;
    }

    // Create shader program
    unsigned int shader_program = create_shader_program(vertex_shader_src,  fragment_shader_src);
    DEBUG("Program creation returned : %d\n", shader_program);
    if (shader_program == 0) {
        FATAL("Failed to generate shader program :\n\t%s\n", get_shader_error());
        return -1;
    }


    // Set up vertex data (a cube)
    Vertex vertices[] = {
        // Front face             Texture Coord. U,V    Normal Vector
        {{-0.5f, -0.5f,  0.5f},     {0.0f, 0.0f},       { 0.0f, 0.0f, 1.0f},},
        {{ 0.5f, -0.5f,  0.5f},     {1.0f, 0.0f},       { 0.0f, 0.0f, 1.0f},},
        {{ 0.5f,  0.5f,  0.5f},     {1.0f, 1.0f},       { 0.0f, 0.0f, 1.0f},},
        {{-0.5f,  0.5f,  0.5f},     {0.0f, 1.0f},       { 0.0f, 0.0f, 1.0f},},
        // Back face            
        {{-0.5f, -0.5f, -0.5f},     {0.0f, 0.0f},       { 0.0f, 0.0f,-1.0f},}, 
        {{ 0.5f, -0.5f, -0.5f},     {1.0f, 0.0f},       { 0.0f, 0.0f,-1.0f},},
        {{ 0.5f,  0.5f, -0.5f},     {1.0f, 1.0f},       { 0.0f, 0.0f,-1.0f},},
        {{-0.5f,  0.5f, -0.5f},     {0.0f, 1.0f},       { 0.0f, 0.0f,-1.0f},},
        // Left face            
        {{-0.5f, -0.5f, -0.5f},     {0.0f, 0.0f},       {-1.0f, 0.0f, 0.0f},},
        {{-0.5f, -0.5f,  0.5f},     {1.0f, 0.0f},       {-1.0f, 0.0f, 0.0f},},
        {{-0.5f,  0.5f,  0.5f},     {1.0f, 1.0f},       {-1.0f, 0.0f, 0.0f},},
        {{-0.5f,  0.5f, -0.5f},     {0.0f, 1.0f},       {-1.0f, 0.0f, 0.0f},},
        // Right face           
        {{ 0.5f, -0.5f, -0.5f},     {0.0f, 0.0f},       { 1.0f, 0.0f, 0.0f},},
        {{ 0.5f, -0.5f,  0.5f},     {1.0f, 0.0f},       { 1.0f, 0.0f, 0.0f},},
        {{ 0.5f,  0.5f,  0.5f},     {1.0f, 1.0f},       { 1.0f, 0.0f, 0.0f},},
        {{ 0.5f,  0.5f, -0.5f},     {0.0f, 1.0f},       { 1.0f, 0.0f, 0.0f},},
        // Top face         
        {{-0.5f,  0.5f, -0.5f},     {0.0f, 0.0f},       { 0.0f, 1.0f, 0.0f},},
        {{ 0.5f,  0.5f, -0.5f},     {1.0f, 0.0f},       { 0.0f, 1.0f, 0.0f},},
        {{ 0.5f,  0.5f,  0.5f},     {1.0f, 1.0f},       { 0.0f, 1.0f, 0.0f},},
        {{-0.5f,  0.5f,  0.5f},     {0.0f, 1.0f},       { 0.0f, 1.0f, 0.0f},},
        // Bottom face          
        {{-0.5f, -0.5f, -0.5f},     {0.0f, 0.0f},       { 0.0f,-1.0f, 0.0f},},
        {{ 0.5f, -0.5f, -0.5f},     {1.0f, 0.0f},       { 0.0f,-1.0f, 0.0f},},
        {{ 0.5f, -0.5f,  0.5f},     {1.0f, 1.0f},       { 0.0f,-1.0f, 0.0f},},
        {{-0.5f, -0.5f,  0.5f},     {0.0f, 1.0f},       { 0.0f,-1.0f, 0.0f},},
    };  
    

    unsigned int indices[] = {
        0, 1, 2, 0, 2, 3,       // Front face      7--------6 
        4, 5, 6, 4, 6, 7,       // Back face      /|       /|
        8, 9, 10, 8, 10, 11,    // Left face    3---------2 | 
        12, 13, 14, 12, 14, 15, // Right face   |  |      | |   
        16, 17, 18, 16, 18, 19, // Top face     |  4------|-5
        20, 21, 22, 20, 22, 23  // Bottom face  | /       |/
    };                          //              0---------1

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // Normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Camera
    if ( (camera = create_camera()) == NULL) {
        FATAL("Failed to create the default camera");
        goto CLEAN_UP;
    };

    // Rotate the cube 90deg on Z and reduce the size
    mat4 model;
    mat4 view;
    mat4 projection;
    glm_mat4_identity(model);
    glm_mat4_identity(view);
    glm_mat4_identity(projection);

    glm_rotate(model, glm_rad(-55.0), (vec3){1.0f, 0.0f, 0.0f});
    glm_translate(view, (vec3){0.0f, 0.0f, -3.0});
    glm_perspective(glm_rad(45.0), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f, projection);



    // Use our shader program
    glUseProgram(shader_program);
    unsigned int model_loc = glGetUniformLocation(shader_program, "model");
    DEBUG("Model location  = %d\n", model_loc);
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model[0]);
    unsigned int view_loc = glGetUniformLocation(shader_program, "view");
    DEBUG("View location  = %d\n", view_loc);
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, view[0]);
    unsigned int projection_loc = glGetUniformLocation(shader_program, "projection");
    DEBUG("Projection location  = %d\n", projection_loc);
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, projection[0]);

    // Set back-face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Generate texture
    unsigned int texture = generate_texture("./res//container.jpg");
    if (texture == 0){
        FATAL("Failed to generate texture\n");
        goto CLEAN_UP;
    }

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader program
        glUseProgram(shader_program);

        // Bind texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // Draw the cube
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, 1);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }
    free(camera);

    // Clean up
CLEAN_UP:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader_program);
    glfwTerminate();
    return 0;
}