//
//  Editor.cpp
//  TrackEditor
//
//  Created by Matheus Polonia on 17/11/23.
//

#include "Editor.h"

void Editor::initGLFW() {
    if (glfwInit() == GLFW_FALSE) {
        std::cout << "Error glfw init" << "\n";
        glfwTerminate();
    }
}

void Editor::initWindow() {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, "TrackEditor", NULL, NULL);

    if (this->window == nullptr) {
        std::cout << "Error: Window ini" << "\n";
        glfwTerminate();
    }

    glfwGetFramebufferSize(this->window, &this->framebufferWidth, &this->framebufferHeight);
    glfwSetFramebufferSizeCallback(window, Editor::framebuffer_resize_callback);

    glfwMakeContextCurrent(this->window);
}

void Editor::initGLEW() {
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "Error: glew init" << "\n";
        glfwTerminate();
    }
}

void Editor::initOpenGLOptions() {
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Editor::initShaders() {
    // Base Shaders (White)
    this->shaders.push_back(new Shader("resources/shaders/baseVS.glsl", "resources/shaders/baseFS.glsl"));

    // B-Spline Shaders (Blue)
    this->shaders.push_back(new Shader("resources/shaders/baseVS.glsl", "resources/shaders/baseBSplineFS.glsl"));

    // Internal B-Spline Shaders (Red)
    this->shaders.push_back(new Shader("resources/shaders/baseVS.glsl", "resources/shaders/internalBSplineFS.glsl"));

    // External B-Spline Shaders (Green)
    this->shaders.push_back(new Shader("resources/shaders/baseVS.glsl", "resources/shaders/externalBSplineFS.glsl"));
}

void Editor::updateProjectionMatrix() {
    // Calculate the orthographic projection matrix with inverted Y
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->WINDOW_WIDTH),
                                      static_cast<float>(this->WINDOW_HEIGHT), 0.0f, -1.0f, 1.0f);

    // Pass the projection matrix to the shader
    for (auto& shader : this->shaders) {
        shader->use();
        shader->setMat4fv(projection, "projection");
    }
}

void Editor::initCross() {
    GLfloat crossVertices[] = {
        // Horizontal line
        0, (float)WINDOW_HEIGHT / 2, 0.0f,   // Endpoint 1
        (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT / 2, 0.0f,    // Endpoint 2

        // Vertical line
        (float)WINDOW_WIDTH / 2, 0, 0.0f,   // Endpoint 1
        (float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT, 0.0f     // Endpoint 2
    };

    glGenVertexArrays(1, &crossVAO);
    glGenBuffers(1, &crossVBO);

    glBindVertexArray(crossVAO);

    glBindBuffer(GL_ARRAY_BUFFER, crossVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crossVertices), crossVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}


//Constructors / Destructors
Editor::Editor() {
    this->window = nullptr;
    this->WINDOW_WIDTH = 600;
    this->WINDOW_HEIGHT = 600;
    this->framebufferWidth = this->WINDOW_WIDTH;
    this->framebufferHeight = this->WINDOW_HEIGHT;

    this->mouseX = 0.0;
    this->mouseY = 0.0;

    this->initGLFW();
    this->initWindow();
    this->initGLEW();
    this->initOpenGLOptions();
    this->initShaders();
    this->initCross();
}

Editor::~Editor() {
    glfwDestroyWindow(this->window);
    glfwTerminate();
}

//Accessor
int Editor::getWindowShouldClose() {
    return glfwWindowShouldClose(this->window);
}

//Modifier
void Editor::setWindowShouldClose() {
    glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

vector<glm::vec3> Editor::calculateSecondVector(const std::vector<glm::vec3>& points, float M, bool isInternal) {
    std::vector<glm::vec3> secondVector;

    for (size_t i = 0; i < points.size() - 1; ++i) {
        float W = points[i + 1].x - points[i].x;
        float H = points[i + 1].y - points[i].y;
        float teta = std::atan(H/W);

        float alpha;
        if (isInternal) {
            if (W < 0) {
                alpha = teta + (90.0 * M_PI / 180.0);
            } else {
                alpha = teta - (90.0 * M_PI / 180.0);
            }
        } else {
            if (W > 0) {
                alpha = teta + (90.0 * M_PI / 180.0);
            } else {
                alpha = teta - (90.0 * M_PI / 180.0);
            }
        }

        float Cx = std::cos(alpha) * M + points[i].x;
        float Cy = std::sin(alpha) * M + points[i].y;

        glm::vec3 pointC(Cx, Cy, points[i].z);
        secondVector.push_back(pointC);
    }

    return secondVector;
}

void Editor::updateMouseInput() {
    glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

    int leftMouseButtonState = glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1);

    if (leftMouseButtonState == GLFW_PRESS && !leftMouseButtonPressed) {

        // this->clickPoints.push_back(glm::vec3(lastMouseX, lastMouseY, 0.f));
        // std::cout << "Position: (" << mouseX << ", " << mouseY << ")"<< "\n";
        this->guidePoints.push_back(Point(mouseX, mouseY, 0.f));

        leftMouseButtonPressed = true;
    } else if (leftMouseButtonState == GLFW_RELEASE) {
        leftMouseButtonPressed = false;
    }
}

void Editor::updateKeyboardInput() {
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(this->window, GLFW_TRUE);
    }
    /// 1 - BEZIER CURVE
    if (glfwGetKey(this->window, GLFW_KEY_1) == GLFW_PRESS && !key1Pressed) {
        this->bezierCurve.setControlPoints(this->guidePoints);
        this->bezierCurve.generateCurve(10);
        key1Pressed = true;
    } else if (glfwGetKey(this->window, GLFW_KEY_1) == GLFW_RELEASE) {
        key1Pressed = false;
    }

    /// 2 - HERMITE CURVE
    if (glfwGetKey(this->window, GLFW_KEY_2) == GLFW_PRESS && !key2Pressed) {
        std::cout << "[Hermite Pressed]\n";
        key2Pressed = true;
    } else if (glfwGetKey(this->window, GLFW_KEY_2) == GLFW_RELEASE) {
        key2Pressed = false;
    }

    /// 3 - B-SPLINE CURVE
    if (glfwGetKey(this->window, GLFW_KEY_3) == GLFW_PRESS && !key3Pressed) {
        this->bSplineCurve.setControlPoints(this->guidePoints);
        this->bSplineCurve.generateCurve(10);
        this->guidePoints.clear();
        key3Pressed = true;
    } else if (glfwGetKey(this->window, GLFW_KEY_3) == GLFW_RELEASE) {
        key3Pressed = false;
    }

    /// 4 - INTERNAL B-SPLINE CURVE
    if (glfwGetKey(this->window, GLFW_KEY_4) == GLFW_PRESS && !key4Pressed) {
        vector<glm::vec3> middleCurvePoints = this->bSplineCurve.getCurvePoints();
        vector<glm::vec3> internalCurvePoints = this->calculateSecondVector(middleCurvePoints, 10, true);
        this->internalBSplineCurve.generateCurveWithPoints(internalCurvePoints);
        key4Pressed = true;
    } else if (glfwGetKey(this->window, GLFW_KEY_4) == GLFW_RELEASE) {
        key4Pressed = false;
    }

    /// 9 - CLEAR GUIDE POINTS
    if (glfwGetKey(this->window, GLFW_KEY_9) == GLFW_PRESS && !key9Pressed) {
        std::cout << "Clear guide points and line\n";
        this->guidePoints.clear();
        key9Pressed = true;
    } else if (glfwGetKey(this->window, GLFW_KEY_9) == GLFW_RELEASE) {
        key9Pressed = false;
    }

    /// 0 - CLEAR CURVES
    if (glfwGetKey(this->window, GLFW_KEY_0) == GLFW_PRESS && !key0Pressed) {
        std::cout << "Clear Curves\n";
        this->bezierCurve.clear();
        this->hermiteCurve.clear();
        this->bSplineCurve.clear();
        key0Pressed = true;
    } else if (glfwGetKey(this->window, GLFW_KEY_0) == GLFW_RELEASE) {
        key0Pressed = false;
    }
}


void Editor::update() {
    glfwPollEvents();

    this->updateKeyboardInput();
    this->updateMouseInput();
}

void Editor::render() {
    this->updateProjectionMatrix();

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLineWidth(5);
    glPointSize(10);

    this->renderCross();

    for (auto&point : this->guidePoints) {
        point.drawPoint(this->shaders[BASE]);
    }

    // this->bezierCurve.drawCurve(this->shaders[0], glm::vec4(0, 1, 0, 1));
    this->bSplineCurve.drawCurve(this->shaders[BASE_B_SPLINE], glm::vec4(0, 1, 0, 1));
    this->internalBSplineCurve.drawCurve(this->shaders[INTERNAL_B_SPLINE], glm::vec4(0, 1, 0, 1));

    this->lineDrawer.drawLines(this->shaders[BASE], this->guidePoints);

    glfwSwapBuffers(window);
}

//Static functions
void Editor::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
};

void Editor::renderCross() {
    this->updateProjectionMatrix();

    glUseProgram(shaders[0]->getID());

    glBindVertexArray(crossVAO);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);
}
