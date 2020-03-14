#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "camera.h"

// Initializing all static members
GLboolean Camera::firstMouseEvent = true;
GLboolean Camera::keys[1024] = {0};

glm::vec3 Camera::cameraPos(3.0f, 3.0f, 3.0f);
glm::vec3 Camera::cameraFront(0.0f, -1.0f, -3.0f);
glm::vec3 Camera::cameraUp(0.0f, 1.0f, 0.0f);

GLfloat Camera::mouseSensitivity = 0.05f;
GLfloat Camera::movementSpeed = 0.1f;

GLfloat Camera::lastX= 800;
GLfloat Camera::lastY = 600;
GLfloat Camera::pitch = 0.0f;
GLfloat Camera::yaw = 0.0f;

GLfloat Camera::fov = 45.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void executeMovement();

Camera::Camera(GLFWwindow* window){
    this->window = window;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);

}
Camera::~Camera(){}

glm::mat4 Camera::getViewMatrix(){
    executeMovement(); // first execute movement using keys array before constructing view matrix
    return glm::lookAt(Camera::cameraPos, Camera::cameraPos + Camera::cameraFront, Camera::cameraUp);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    /*
    This function handles looking around the scene
    */
    if(Camera::firstMouseEvent){
        Camera::lastX = xpos;
        Camera::lastY = ypos;
        Camera::firstMouseEvent = false;
    }

    GLfloat xOffset = xpos - Camera::lastX;
    GLfloat yOffset = Camera::lastY - ypos; 
    Camera::lastX = xpos;
    Camera::lastY = ypos;

    xOffset *= Camera::mouseSensitivity;
    yOffset *= Camera::mouseSensitivity;
    Camera::yaw += xOffset;
    Camera::pitch += yOffset;

    Camera::pitch = std::min(std::max(Camera::pitch, -89.0f), 89.0f); 
    glm::vec3 front;
    front.x = cos(glm::radians(Camera::yaw)) * cos(glm::radians(Camera::pitch));
    front.y = sin(glm::radians(Camera::pitch));
    front.z = sin(glm::radians(Camera::yaw)) * cos(glm::radians(Camera::pitch));
    Camera::Camera::cameraFront = glm::normalize(front);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){
    /*
    Records keypress events in keys
    */
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if(key >=0 && key < 1024){
        if (action == GLFW_PRESS)
            Camera::keys[key] = true;
        else if (action == GLFW_RELEASE)
            Camera::keys[key] = false;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    GLfloat sensitivity = 0.05f;
    if(Camera::fov >= 1.0f && Camera::fov <= 45.0f) Camera::fov -= sensitivity * yoffset;
    Camera::fov = std::max(std::min(Camera::fov, 45.0f), 1.0f);
}

void executeMovement(){
    if(Camera::keys[GLFW_KEY_W]) Camera::cameraPos += Camera::movementSpeed * Camera::cameraFront;
    if(Camera::keys[GLFW_KEY_S]) Camera::cameraPos -= Camera::movementSpeed * Camera::cameraFront;
    if(Camera::keys[GLFW_KEY_A]) Camera::cameraPos -= glm::normalize(glm::cross(Camera::cameraFront, Camera::cameraUp)) * Camera::movementSpeed;
    if(Camera::keys[GLFW_KEY_D]) Camera::cameraPos += glm::normalize(glm::cross(Camera::cameraFront, Camera::cameraUp)) * Camera::movementSpeed;
}