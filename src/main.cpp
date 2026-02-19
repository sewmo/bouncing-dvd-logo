#include <iostream>
#include <assert.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h> 
#include <shader.hpp>
#include <texture.hpp>
#include <math.h>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void processMovement(float& xPos, float& yPos, float& xVelocity, float& yVelocity, int& activeTexture);

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_TITLE "Bouncing DVD Logo"
#define VERTEX_SHADER_RELATIVE_PATH "shaders/shader.vs"
#define FRAGMENT_SHADER_RELATIVE_PATH "shaders/shader.fs"

float vertices[] = {
    0.20f, 0.35f, 0.0f,   1.0f, 1.0f,  // Top Right
    0.20f, -0.35f, 0.0f,  1.0f, 0.0f,  // Bottom Right
    -0.20f, -0.35f, 0.0f, 0.0f, 0.0f,  // Bottom Left
    -0.20f, 0.35f, 0.0f,  0.0f, 1.0f   // Top Left
};

GLuint indices[] = {
    0, 1, 3,
    1, 2, 3
};

int main()
{
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW..." << std::endl;
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_TITLE, NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to open GLFW window..." << std::endl;
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD..." << std::endl;
        return EXIT_FAILURE;
    }
    
    GLuint VBO, VAO, EBO;       
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); 
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    
    Shader shader(VERTEX_SHADER_RELATIVE_PATH, FRAGMENT_SHADER_RELATIVE_PATH);

    std::vector<Texture> dvdTextures;

    int activeTexture = 0;

    for (int i = 1; i <= 7; i++) {
        char path[64];
        snprintf(path, sizeof(path), "assets/DVD%d.png", i);
        Texture texture(path, TextureFormat::PNG, true);
        dvdTextures.push_back(texture);
    }

    shader.use();
    shader.setInt("textureSampler", 0);

    float xPos = 0.0f;
    float yPos = 0.0f;

    float xVelocity = 0.001f;
    float yVelocity = 0.00075f;

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        processMovement(xPos, yPos, xVelocity, yVelocity, activeTexture);

        shader.setFloat("xPos", xPos);
        shader.setFloat("yPos", yPos);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dvdTextures[activeTexture].ID);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return EXIT_SUCCESS;
}

void processMovement(float& xPos, float& yPos, float& xVelocity, float& yVelocity, int& activeTexture) {
    float tolerance = 0.18f; // Account for the extra empty space in the textures, so it doesn't look like the DVD logo is bouncing too early.

    xPos += xVelocity;
    yPos += yVelocity;

    if (xPos >= 0.80f) {   
        xPos = 0.80f;
        xVelocity = -xVelocity;
        activeTexture += 1;
        printf("INFO: Cycling through DVD texture...\n");
    }

    else if (xPos <= -0.80f) {
        xPos = -0.80f;
        xVelocity = -xVelocity;
        activeTexture += 1;
        printf("INFO: Cycling through DVD texture...\n");
    }

    if (yPos >= 0.65f + tolerance) {
        yPos = 0.65f + tolerance;
        yVelocity = -yVelocity;
        activeTexture += 1;
        printf("INFO: Cycling through DVD texture...\n");
    }

    else if (yPos <= -0.65f - tolerance) {
        yPos = -0.65f - tolerance;
        yVelocity = -yVelocity;
        activeTexture += 1;
        printf("INFO: Cycling through DVD texture...\n");
    }

    if (activeTexture > 6) activeTexture = 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    } 
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
