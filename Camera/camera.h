#ifndef _CAMERA_H_INCLUDED_
#define _CAMERA_H_INCLUDED_

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*
This class handles Moving Around, Looking Around and Zooming given a GLFWwindow instance 
Need to fix this class by changing the static variable approach to pimple idiom approach
*/
class Camera{
    private:
        GLFWwindow* window;

    public:
        static glm::vec3 cameraPos, cameraFront, cameraUp;
        static GLfloat lastX, lastY, yaw, pitch, mouseSensitivity, movementSpeed, fov; 
        static GLboolean firstMouseEvent, keys[1024];

        Camera(GLFWwindow*);
        ~Camera();
        glm::mat4 getViewMatrix(); // returns updated view matrix
};

#endif