//
//  Editor.h
//  TrackEditor
//
//  Created by Matheus Polonia on 17/11/23.
//

#ifndef Editor_hpp
#define Editor_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>

#include <OpenGL/OpenGL.h>

#include "Shader.h"
#include "Curve.hpp"
#include "Point.h"

class Editor {
private:
//Variables
    //Window
    GLFWwindow* window;
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    int framebufferWidth;
    int framebufferHeight;

    std::vector<Shader*> shaders;

    float dt;
    float curTime;
    float lastTime;

    double lastMouseX;
    double lastMouseY;
    double mouseX;
    double mouseY;
    double mouseOffsetX;
    double mouseOffsetY;
    bool firstMouse;

    // Track
    bool leftMouseButtonPressed;
    bool aKeyPressed;

    GLuint VAO;
    GLuint VBO;

    std::vector<glm::vec3> clickPoints;

//Private functions
    void initGLFW();
    void initWindow();
    void initGLEW(); //AFTER CONTEXT CREATION!!!
    void initOpenGLOptions();
    void initShaders();
    void initVAO();
    void updateProjectionMatrix();

//Static variables

public:
//Constructors / Destructors
    Editor();
    virtual ~Editor();

//Accessors
    int getWindowShouldClose();

//Modifiers
    void setWindowShouldClose();

//Functions
    void updateDt();
    void updateMouseInput();
    void updateKeyboardInput();
    void updateInput();
    void update();
    void render();

    void drawPoints();

//Static functions
    static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
};


#endif /* Editor_h */
