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

void Editor::initVAO() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * clickPoints.size(), clickPoints.data(), GL_DYNAMIC_DRAW);

    // Specify the layout of the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Editor::initShaders() {
    std::cerr << "Here init shaders" << std::endl;
    this->shaders.push_back(new Shader("resources/shaders/vertexShader.glsl", "resources/shaders/fragmentShader.glsl"));
    this->initVAO();
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

    this->dt = 0.f;
    this->curTime = 0.f;
    this->lastTime = 0.f;

    this->lastMouseX = 0.0;
    this->lastMouseY = 0.0;
    this->mouseX = 0.0;
    this->mouseY = 0.0;
    this->mouseOffsetX = 0.0;
    this->mouseOffsetY = 0.0;
    this->firstMouse = true;

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

//Functions
void Editor::drawPoints() {
    glBindVertexArray(VAO);

    // Update VBO with new points
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * clickPoints.size(), clickPoints.data(), GL_DYNAMIC_DRAW);

    // Draw the points
    glUseProgram(shaders[0]->getID());  // Assuming the first shader in the vector is the one you want to use
    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(clickPoints.size()));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}


void Editor::updateDt() {
    this->curTime = static_cast<float>(glfwGetTime());
    this->dt = this->curTime - this->lastTime;
    this->lastTime = this->curTime;
}

void Editor::updateMouseInput() {
    glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

    this->lastMouseX = this->mouseX;
    this->lastMouseY = this->mouseY;

    int leftMouseButtonState = glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1);

    if (leftMouseButtonState == GLFW_PRESS && !leftMouseButtonPressed) {
        std::cout << "\nposition: " << "(" << lastMouseX << ", " << lastMouseY << ")";

        this->clickPoints.push_back(glm::vec3(lastMouseX, lastMouseY, 0.f));

        leftMouseButtonPressed = true;
    } else if (leftMouseButtonState == GLFW_RELEASE) {
        leftMouseButtonPressed = false;
    }
}

void Editor::updateKeyboardInput() {
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(this->window, GLFW_TRUE);
    }

    int aKeyState = glfwGetKey(this->window, GLFW_KEY_P);

    if (aKeyState == GLFW_PRESS && !aKeyPressed) {
        std::cout << "\nPoints" << std::endl;
        for (const auto& point : clickPoints) {
            std::cout << "Point: (" << point.x << ", " << point.y << ", " << point.z << ")" << std::endl;
        }
        aKeyPressed = true;
    } else if (aKeyState == GLFW_RELEASE) {
        aKeyPressed = false;
    }
}

void Editor::updateInput() {
    glfwPollEvents();

    this->updateKeyboardInput();
    this->updateMouseInput();
}

void Editor::update() {
    //UPDATE INPUT ---
    this->updateDt();
    this->updateInput();
}

void Editor::render() {
    this->updateProjectionMatrix();

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glPointSize(5);

    for (auto&point : this->clickPoints) {
        std::cout << "Point: (" << point.x << ", " << point.y << ", " << point.z << ")" << std::endl;
        this->drawPoints();
    }


    glfwSwapBuffers(window);
}

//Static functions
void Editor::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
};


