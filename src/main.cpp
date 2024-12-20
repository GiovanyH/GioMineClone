#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "learnopengl/shader_m.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;
float pitch =  0.0f;
float lastX =  1280.0f / 2.0;
float lastY =  720.0 / 2.0;
float fov   =  90.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void renderQuad(GLuint textureID, float x, float y, float width, float height, float texWidth, float texHeight) {
    glBindTexture(GL_TEXTURE_2D, textureID);

    float texX = texWidth;
    float texY = texHeight;
    float texXMax = ((16.0f + 0.0f) / texWidth);
    float texYMax = 16.0f / texHeight;

    float vertices[] = {
        // Positions         // Texture Coords
        -0.5f, -0.5f, -0.5f,  texXMax * 0.0f, (texXMax * 0.0f) - texYMax,  // Bottom-left
        0.5f, -0.5f, -0.5f,   texXMax, (texXMax * 0.0f) - texYMax, // Bottom-right
        0.5f,  0.5f, -0.5f,   texXMax, (texXMax * 0.0f),           // Top-right
        0.5f,  0.5f, -0.5f,   texXMax, (texXMax * 0.0f),           // Top-right
        -0.5f,  0.5f, -0.5f,  texXMax * 0.0f, (texXMax * 0.0f),              // Top-left
        -0.5f, -0.5f, -0.5f,  texXMax * 0.0f, (texXMax * 0.0f) - texYMax,    // Bottom-left

        -0.5f, -0.5f,  0.5f,  texXMax * 0.0f, (texXMax * 0.0f) - texYMax,
         0.5f, -0.5f,  0.5f,  texXMax, (texXMax * 0.0f) - texYMax,
         0.5f,  0.5f,  0.5f,  texXMax, (texXMax * 0.0f),
         0.5f,  0.5f,  0.5f,  texXMax, (texXMax * 0.0f),
        -0.5f,  0.5f,  0.5f,  texXMax * 0.0f, (texXMax * 0.0f),
        -0.5f, -0.5f,  0.5f,  texXMax * 0.0f, (texXMax * 0.0f) - texYMax,

        -0.5f,  0.5f,  0.5f,  texXMax * 0.0f, (texXMax * 0.0f),
        -0.5f,  0.5f, -0.5f,  texXMax, (texXMax * 0.0f),
        -0.5f, -0.5f, -0.5f,  texXMax, (texXMax * 0.0f) - texYMax,
        -0.5f, -0.5f, -0.5f,  texXMax, (texXMax * 0.0f) - texYMax,
        -0.5f, -0.5f,  0.5f,  texXMax * 0.0f, (texXMax * 0.0f) - texYMax,
        -0.5f,  0.5f,  0.5f,  texXMax * 0.0f, (texXMax * 0.0f),

        0.5f,  0.5f,  0.5f,  texXMax * 0.0f, (texXMax * 0.0f),
         0.5f,  0.5f, -0.5f,  texXMax, (texXMax * 0.0f),
         0.5f, -0.5f, -0.5f,  texXMax, (texXMax * 0.0f) - texYMax,
         0.5f, -0.5f, -0.5f,  texXMax, (texXMax * 0.0f) - texYMax,
         0.5f, -0.5f,  0.5f,  texXMax * 0.0f, (texXMax * 0.0f) - texYMax,
         0.5f,  0.5f,  0.5f,  texXMax * 0.0f, (texXMax * 0.0f),

         -0.5f, -0.5f, -0.5f,  ((16.0f + 0.0f) / texWidth) * 2, (texXMax * 0.0f),
         0.5f, -0.5f, -0.5f,  ((16.0f + 0.0f) / texWidth) * 3, (texXMax * 0.0f),
         0.5f, -0.5f,  0.5f,  ((16.0f + 0.0f) / texWidth) * 3, (texXMax * 0.0f) - texYMax,
         0.5f, -0.5f,  0.5f,  ((16.0f + 0.0f) / texWidth) * 3, (texXMax * 0.0f) - texYMax,
        -0.5f, -0.5f,  0.5f,  ((16.0f + 0.0f) / texWidth) * 2, (texXMax * 0.0f) - texYMax,
        -0.5f, -0.5f, -0.5f,  ((16.0f + 0.0f) / texWidth) * 2, (texXMax * 0.0f),

        -0.5f,  0.5f, -0.5f,  ((16.0f + 0.0f) / texWidth) * 1, (texXMax * 0.0f),
         0.5f,  0.5f, -0.5f,  ((16.0f + 0.0f) / texWidth) * 2, (texXMax * 0.0f),
         0.5f,  0.5f,  0.5f,  ((16.0f + 0.0f) / texWidth) * 2, (texXMax * 0.0f) - texYMax,
         0.5f,  0.5f,  0.5f,  ((16.0f + 0.0f) / texWidth) * 2, (texXMax * 0.0f) - texYMax,
        -0.5f,  0.5f,  0.5f,  ((16.0f + 0.0f) / texWidth) * 1, (texXMax * 0.0f) - texYMax,
        -0.5f,  0.5f, -0.5f,  ((16.0f + 0.0f) / texWidth) * 1, (texXMax * 0.0f),
    };

    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "GioMineClone", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("shaders/camera.vs", "shaders/camera.fs");

    unsigned int texture1;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    float texX = 0.0f / width;
    float texY = 0.0f / height;
    float texXMax = 16.0f / width;
    float texYMax = 16.0f / height;
    
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f - texYMax,
         0.5f, -0.5f, -0.5f,  texXMax, 1.0f - texYMax,
         0.5f,  0.5f, -0.5f,  texXMax, 1.0f,
         0.5f,  0.5f, -0.5f,  texXMax, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f - texYMax,

        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f - texYMax,
         0.5f, -0.5f,  0.5f,  texXMax, 1.0f - texYMax,
         0.5f,  0.5f,  0.5f,  texXMax, 1.0f,
         0.5f,  0.5f,  0.5f,  texXMax, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f - texYMax,

        -0.5f,  0.5f,  0.5f,  texXMax, 1.0f - texYMax,
        -0.5f,  0.5f, -0.5f,  texXMax, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f - texYMax,
        -0.5f,  0.5f,  0.5f,  texXMax, 1.0f - texYMax,

         0.5f,  0.5f,  0.5f,  texXMax, 1.0f - texYMax,
         0.5f,  0.5f, -0.5f,  texXMax, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f - texYMax,
         0.5f,  0.5f,  0.5f,  texXMax, 1.0f - texYMax,

        -0.5f, -0.5f, -0.5f,  0.0f, texYMax,
         0.5f, -0.5f, -0.5f,  texXMax, 1.0f,
         0.5f, -0.5f,  0.5f,  texXMax, 1.0f - texYMax,
         0.5f, -0.5f,  0.5f,  texXMax, 1.0f - texYMax,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f - texYMax,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, texYMax,
         0.5f,  0.5f, -0.5f,  texXMax, 1.0f,
         0.5f,  0.5f,  0.5f,  texXMax, 1.0f - texYMax,
         0.5f,  0.5f,  0.5f,  texXMax, 1.0f - texYMax,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f - texYMax,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    ourShader.use();
    ourShader.setInt("texture1", 0);


    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        ourShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            renderQuad(texture1, 0.0f, 1.0f, 16.0f, 16.0f, width, height);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos.y += cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        cameraPos.y -= cameraSpeed;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 90.0f)
        fov = 90.0f;
}