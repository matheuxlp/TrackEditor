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

    int aKeyState = glfwGetKey(this->window, GLFW_KEY_A);
    int bKeyState = glfwGetKey(this->window, GLFW_KEY_B);

    if (aKeyState == GLFW_PRESS && !aKeyPressed) {
        this->bezierCurve.printData();
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

void Editor::updateInput() {
    glfwPollEvents();

    this->updateKeyboardInput();
    this->updateMouseInput();
}

void Editor::update() {
    this->updateDt();
    this->updateInput();
}

void Editor::render() {
    this->updateProjectionMatrix();

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLineWidth(5);
    glPointSize(10);
//    glPointSize(5);

    for (auto&point : this->clickPoints) {
        this->drawPoints();
    }
//
//    this->hermiteCurve.setControlPoints(this->clickPoints);
//    this->hermiteCurve.setShader(this->shaders[0]);
//    this->hermiteCurve.generateCurve(5);
//    this->hermiteCurve.drawCurve(glm::vec4(1, 0, 0, 1));

    if (this->clickPoints.size() >= 4) {
        this->bezierCurve.setControlPoints(this->clickPoints);
        this->clickPoints.clear();
    }
    this->bezierCurve.setShader(this->shaders[0]);
    this->bezierCurve.generateCurve(10);
    this->bezierCurve.drawCurve(glm::vec4(0, 1, 0, 1));

//    glm::vec3 pointOnCurve = bezier.getPointOnCurve(i);
//    vector <glm::vec3> aux;
//    aux.push_back(pointOnCurve);
//    GLuint VAOPoint = generateControlPointsBuffer(aux);
//
//    glBindVertexArray(VAOPoint);
//
//    shader.setVec4("finalColor", 0, 0, 0, 1);
//    // Chamada de desenho - drawcall
//    // CONTORNO e PONTOS - GL_LINE_LOOP e GL_POINTS
//    glDrawArrays(GL_POINTS, 0, aux.size());
//    //glDrawArrays(GL_LINE_STRIP, 0, controlPoints.size());
//    glBindVertexArray(0);
//
//    i = (i + 1) % nbCurvePoints;

    glfwSwapBuffers(window);
}

//Static functions
void Editor::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
};


GLuint Editor::generateControlPointsBuffer(vector <glm::vec3> controlPoints) {
    GLuint VBO, VAO;

    //Gera��o do identificador do VBO
    glGenBuffers(1, &VBO);

    //Faz a conex�o (vincula) do buffer como um buffer de array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Envia os dados do array de floats para o buffer da OpenGl
    glBufferData(GL_ARRAY_BUFFER, controlPoints.size() * sizeof(GLfloat)* 3, controlPoints.data(), GL_STATIC_DRAW);

    //Gera��o do identificador do VAO (Vertex Array Object)
    glGenVertexArrays(1, &VAO);

    // Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
    // e os ponteiros para os atributos
    glBindVertexArray(VAO);

    //Atributo posi��o (x, y, z)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice
    // atualmente vinculado - para que depois possamos desvincular com seguran�a
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
    glBindVertexArray(0);

    return VAO;
}
