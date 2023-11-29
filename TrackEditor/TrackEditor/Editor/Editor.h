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
#include "Bezier.hpp"
#include "Hermite.hpp"
#include "Point.h"
#include "LineDrawer.h"


class Editor {
private:
//Variables
    //Window
    GLFWwindow* window;
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    int framebufferWidth;
    int framebufferHeight;

    double mouseX;
    double mouseY;
    
    // Track
    std::vector<Shader*> shaders;

    bool leftMouseButtonPressed;
    bool aKeyPressed;
    bool bKeyPressed;

    LineDrawer lineDrawer;

    std::vector<Point*> points;

    Hermite hermiteCurve;
    Bezier bezierCurve;

    // ----- //

    void initGLFW();
    void initWindow();
    void initGLEW();
    void initOpenGLOptions();
    void initShaders();
    void updateProjectionMatrix();

public:
    Editor();
    virtual ~Editor();

    int getWindowShouldClose();

    void setWindowShouldClose();

    void updateMouseInput();
    void updateKeyboardInput();
    void update();
    void render();

//Static functions
    static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
};

#endif /* Editor_h */
