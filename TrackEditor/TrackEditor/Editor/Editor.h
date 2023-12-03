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
#include "BSpline.hpp"
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
    glm::mat4 projectionMatrix;

    std::vector<Shader*> shaders;

    // Guide
    GLuint crossVAO, crossVBO;
    LineDrawer lineDrawer;
    std::vector<Point> guidePoints;

    // Keyboard and mouse Update
    bool leftMouseButtonPressed;

    bool key1Pressed;
    bool key2Pressed;
    bool key3Pressed;
    bool key4Pressed;
    bool key5Pressed;
    bool key6Pressed;
    bool key7Pressed;
    bool key9Pressed;
    bool key0Pressed;

    // Curves
    Hermite hermiteCurve;
    Bezier bezierCurve;
    BSpline bSplineCurve;
    BSpline internalBSplineCurve;
    BSpline externalBSplineCurve;


    // ----- //

    void initGLFW();
    void initWindow();
    void initGLEW();
    void initOpenGLOptions();
    void initShaders();
    void updateProjectionMatrix();
    void initCross();
    void renderCross();
    vector<glm::vec3> calculateSecondVector(const std::vector<glm::vec3>& points, float M, bool isInternal);
    void writeObjFile(vector<glm::vec3>& internalVertices, vector<glm::vec3>& externalVertices, const string& filename, float scale);
    vector<glm::vec3> invertVector(const vector<glm::vec3>& points);
    vector<glm::vec3> getShaderPosition(const vector<glm::vec3>& vertices);
    vector<glm::vec3> getOBJVertices(const vector<glm::vec3>& vertices);
    void writeAnimationFile(const std::vector<glm::vec3>& vectorToWrite, const std::string& filename, float scale);

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

enum ShaderType {
    BASE = 0,
    BASE_B_SPLINE = 1,
    INTERNAL_B_SPLINE = 2,
    EXTERNAL_B_SPLINE = 3
};

#endif /* Editor_h */
