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
    std::cerr << "Here init shaders" << std::endl;
    this->shaders.push_back(new Shader("resources/shaders/vertexShader.glsl", "resources/shaders/fragmentShader.glsl"));
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

void Editor::updateMouseInput() {
    glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

    int leftMouseButtonState = glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1);

    if (leftMouseButtonState == GLFW_PRESS && !leftMouseButtonPressed) {

        // this->clickPoints.push_back(glm::vec3(lastMouseX, lastMouseY, 0.f));
        this->points.push_back(new Point(mouseX, mouseY, 0.f));

        leftMouseButtonPressed = true;
    } else if (leftMouseButtonState == GLFW_RELEASE) {
        leftMouseButtonPressed = false;
    }
}

void Editor::updateKeyboardInput() {
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(this->window, GLFW_TRUE);
    }

    int aKeyState = glfwGetKey(this->window, GLFW_KEY_A);
    int bKeyState = glfwGetKey(this->window, GLFW_KEY_B);

    if (aKeyState == GLFW_PRESS && !aKeyPressed) {
        this->bezierCurve.printData();
        this->bezierCurve.setShader(this->shaders[0]);
        this->bezierCurve.generateCurve(10);
        aKeyPressed = true;
    } else if (aKeyState == GLFW_RELEASE) {
        aKeyPressed = false;
    }

    if (bKeyState == GLFW_PRESS && !bKeyPressed) {
        this->bezierCurve.printData();
        bKeyPressed = true;
    } else if (bKeyState == GLFW_RELEASE) {
        bKeyPressed = false;
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

    for (auto&point : this->points) {
        point->drawPoint(this->shaders[0]);
    }

    this->lineDrawer.drawLines(this->shaders[0], this->points);

    glfwSwapBuffers(window);
}

//Static functions
void Editor::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
};
