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
#include "Point.h"
#include "Hermite.hpp"
#include "Curve.hpp"
#include "Bezier.hpp"
#include "BSpline.hpp"

#include "LineDrawer.h"


class Editor {
private:
    /// MARK: Window Varaibles
    GLFWwindow* window;
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
    int framebufferWidth;
    int framebufferHeight;

    /// MARK: Shader and Visual Variables
    glm::mat4 projectionMatrix;
    std::vector<Shader*> shaders;

    /// MARK: Visual Guide Variables
    GLuint crossVAO, crossVBO;
    std::vector<Point> guidePoints;
    LineDrawer lineDrawer;

    /// MARK: Mouse Varaibles
    double mouseX;
    double mouseY;
    bool leftMouseButtonPressed;

    /// MARK: Mouse Varaibles
    bool key1Pressed; // Hermit Curve
    bool key2Pressed; // Bezier Curve
    bool key3Pressed; // B-Spline Curve
    bool key4Pressed; // External B-Spline Curve
    bool key5Pressed; // Internal B-Spline Curve
    bool key6Pressed; // Clean
    bool key7Pressed; // Clean
    bool key9Pressed; // Generate OBJ
    bool key0Pressed; // Generate Animation

    /// MARK: Curve Variables
    Hermite hermiteCurve;
    Bezier bezierCurve;
    BSpline bSplineCurve;
    BSpline internalBSplineCurve;
    BSpline externalBSplineCurve;

    /// MARK: Init Function
    void initGLFW();
    void initWindow();
    void initGLEW();
    void initOpenGLOptions();
    void initShaders();
    void initCross();

    /// MARK: Calculation and Vector Function
    vector<glm::vec3> calculateSecondVector(const std::vector<glm::vec3>& points, float M, bool isInternal);
    vector<glm::vec3> invertVector(const vector<glm::vec3>& points);
    vector<glm::vec3> getShaderPosition(const vector<glm::vec3>& vertices);
    vector<glm::vec3> getOBJVertices(const vector<glm::vec3>& vertices);

    ///  MARK: Object Writing Functions
    void writeObjFile(vector<glm::vec3>& internalVertices, vector<glm::vec3>& externalVertices, const string& filename, float scale);
    void writeAnimationFile(const std::vector<glm::vec3>& vectorToWrite, const std::string& filename, float scale);

public:
    Editor();
    virtual ~Editor();
    int getWindowShouldClose();
    void setWindowShouldClose();

    /// MARK: Update Function
    void updateProjectionMatrix();
    void updateMouseInput();
    void updateKeyboardInput();
    void update();

    /// MARK: Rendering Function
    void render();
    void renderCross();

    static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
};

/// Enum to diferenciate the shaders
enum ShaderType {
    BASE = 0,
    BASE_B_SPLINE = 1,
    INTERNAL_B_SPLINE = 2,
    EXTERNAL_B_SPLINE = 3
};

#endif /* Editor_h */
