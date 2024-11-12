#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "loki.h"
#include "tools/log.h"
#include "tools/res.h"
#include "shaders.h"

#include <stdio.h>
#include <stdlib.h>

EngineState engine;
Mouse mouse;
Camera *camera;


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

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void cursor_position_callback(GLFWwindow* window, double x_position, double y_position)
{
    if (engine.is_mouse_captured) {
        mouse.x = x_position;
        mouse.y = y_position;
    }
}

// Mouse button callback function
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) 
{
    if (button == GLFW_MOUSE_BUTTON_LEFT  && action == GLFW_PRESS) {
        // Capture mouse when clicking in window
        if ( ! engine.is_mouse_captured ) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, 0.0f, 0.0f);
            mouse.x = 0.0f;
            mouse.y = 0.0f;
            engine.is_mouse_captured = true;
        }
    }
}

// Mouse scroll callback
void mouse_scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
    if (engine.is_mouse_captured) {
        mouse.scroll_x = x_offset;
        mouse.scroll_y = y_offset;
        camera->update_zoom = true;
    }   
}

// Key callback to allow escaping from mouse capture
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        // Release mouse capture when pressing ESC
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetWindowShouldClose(window, true);  
        engine.is_mouse_captured = false;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


void update(EngineState* state)
{
    // Regular game updates using delta time
    // Example: Input processing
    // if (glfwGetKey(state->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    //     state->player_velocity = 100.0f; // units per second
    // }
    // else if (glfwGetKey(state->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    //     state->player_velocity = -100.0f;
    // }
    // else {
    //     state->player_velocity = 0.0f;
    // }
    // DEBUG("FPS update: %.2f\n", state->time.fps);
}

void fixed_update(EngineState* state)
{
    // Physics and other time-critical updates
    // This runs at a fixed time step (60 times per second)
    
    // Example: Basic physics update
    // state->player_x += state->player_velocity * state->time.fixed_time_step;
    DEBUG("FPS fixed update: %.2f\n", state->time.fixed_fps);
}

void render(EngineState* state)
{
    // DEBUG("FPS render update: %.2f\n", state->time.fps);
}


int main() 
{
    // initialize engine state
    log_init();
    engine.is_mouse_captured = false;
    mouse.x = 0.0f ;
    mouse.y = 0.0f ;
    mouse.scroll_x = 0.0f ;
    mouse.scroll_y = 0.0f ;
    engine.screen_height = SCR_HEIGHT;
    engine.screen_width = SCR_WIDTH;
    init_engine_time(&engine.time);
    // Camera
    if ( (camera = create_camera(NULL)) == NULL) {
        FATAL("Failed to create the default camera");
        return 0;
    };

    // Initialize GLFW
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    // Set the callbacks
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetScrollCallback(window, mouse_scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    
    // Start with normal cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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

    // Rotate the cube 90deg on Z and reduce the size
    mat4 model;
    glm_mat4_identity(model);

    glm_rotate(model, glm_rad(-55.0), (vec3){1.0f, 0.0f, 0.0f});
    glm_perspective(glm_rad(camera->fov), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f, camera->projection);

    // Use our shader program
    glUseProgram(shader_program);

    // send model matrix
    unsigned int model_loc = glGetUniformLocation(shader_program, "model");
    DEBUG("Model location  = %d\n", model_loc);
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model[0]);

    // send view matrix
    unsigned int view_loc = glGetUniformLocation(shader_program, "view");
    DEBUG("View location  = %d\n", view_loc);
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, camera->view[0]);

    // send projection matrix
    unsigned int projection_loc = glGetUniformLocation(shader_program, "projection");
    DEBUG("Projection location  = %d\n", projection_loc);
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, camera->projection[0]);

    // Set back-face culling
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);

    // Generate texture
    unsigned int texture = generate_texture("./res//container.jpg");
    if (texture == 0){
        FATAL("Failed to generate texture\n");
        goto CLEAN_UP;
    }


    // Render loop
    while (!glfwWindowShouldClose(window)) {
        
        // Poll for and process events
        glfwPollEvents();

        // Update timing
        update_delta_time(&engine.time);

        // Clear the color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Fixed update step for physics
        while (should_fixed_update(&engine.time)) {
            fixed_update(&engine);
        }

        // Variable update step
        update(&engine);
        
        // Render
        render(&engine);

        // Use our shader program
        glUseProgram(shader_program);
        update_camera(camera , window);
        if (engine.update_prospective) {
            glUniformMatrix4fv(projection_loc, 1, GL_FALSE, camera->projection[0]);
            engine.update_prospective = false;
        }
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, camera->view[0]);

        // Bind texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // Draw the cube
        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, 1);

        // Swap front and back buffers
        glfwSwapBuffers(window);


        
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