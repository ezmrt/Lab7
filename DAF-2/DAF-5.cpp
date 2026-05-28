#define GLFW_DLL
#define GLEW_DLL
#include "glew-2.1.0/include/GL/glew.h"
#include "glfw-3.4.bin.WIN64/include/GLFW/glfw3.h"
#include <iostream>
#include "Shader.h"
#include "Model.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// Параметры камеры
glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 256.0f;
float lastY = 256.0f;
bool firstMouse = true;


float deltaTime = 0.0f;
float lastFrame = 0.0f;


float angleMesh1 = 0.0f; 
float angleMesh2 = 0.0f; 
float angleMesh3 = 0.0f; 
float angleMesh4 = 0.0f; 

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 10.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPosition += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPosition -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    
    float rotationSpeed = 50.0f * deltaTime; 

    
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) angleMesh1 += rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) angleMesh1 -= rotationSpeed;

    
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) angleMesh2 += rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) angleMesh2 -= rotationSpeed;

    
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) angleMesh3 += rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) angleMesh3 -= rotationSpeed;

    
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) angleMesh4 += rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) angleMesh4 -= rotationSpeed;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    
    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;
    lastX = (float)xpos;
    lastY = (float)ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* Okno = glfwCreateWindow(800, 600, "LAB-7", NULL, NULL);
    if (!Okno) { glfwTerminate(); return -1; }

    glfwMakeContextCurrent(Okno);
    glewInit();
    glEnable(GL_DEPTH_TEST);

    Shader labShader("triangle.vert", "triangle.frag");
    if (labShader.ProgramID == 0) {
        std::cerr << "Ошибка компиляции шейдера!" << std::endl;
        return -1;
    }

    Model ourModel("model.obj");

    glfwSetInputMode(Okno, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(Okno, mouse_callback);

    glm::vec3 lightPos(5.0f, 5.0f, 10.0f);

    while (!glfwWindowShouldClose(Okno)) {
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(Okno);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        labShader.activate();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
        glm::mat4 model = glm::mat4(1.0f); // Базовая матрица всей модели

        labShader.setMat4("projection", projection);
        labShader.setMat4("view", view);

        labShader.setVec3("viewPos", cameraPosition);
        labShader.setVec3("light.position", lightPos);
        labShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        labShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
        labShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        labShader.setVec3("material.ambient", 0.6f, 0.2f, 0.9f);
        labShader.setVec3("material.diffuse", 0.6f, 0.2f, 0.9f);
        labShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        labShader.setFloat("material.shininess", 32.0f);

        
        ourModel.Draw(labShader, model, angleMesh1, angleMesh2, angleMesh3, angleMesh4);

        glfwSwapBuffers(Okno);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}