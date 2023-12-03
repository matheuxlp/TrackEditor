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
    this->projectionMatrix = glm::ortho(0.0f, static_cast<float>(this->WINDOW_WIDTH),
                                      static_cast<float>(this->WINDOW_HEIGHT), 0.0f, -1.0f, 1.0f);

    for (auto& shader : this->shaders) {
        shader->use();
        shader->setMat4fv(projectionMatrix, "projection");
    }
}

void Editor::initCross() {
    GLfloat crossVertices[] = {
        0, (float)WINDOW_HEIGHT / 2, 0.0f,
        (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT / 2, 0.0f,

        (float)WINDOW_WIDTH / 2, 0, 0.0f,
        (float)WINDOW_WIDTH / 2, (float)WINDOW_HEIGHT, 0.0f
    };

    glGenVertexArrays(1, &crossVAO);
    glGenBuffers(1, &crossVBO);

    glBindVertexArray(crossVAO);

    glBindBuffer(GL_ARRAY_BUFFER, crossVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crossVertices), crossVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

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

int Editor::getWindowShouldClose() {
    return glfwWindowShouldClose(this->window);
}

void Editor::setWindowShouldClose() {
    glfwSetWindowShouldClose(this->window, GLFW_TRUE);
}

/// MARK: Vector Calculations

/// Calculate the position of the internal and external curves based on the base B-Spline curve
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

/// Invester the Y and Z coordinates of the vector to make the curve be shown in the horizontal plane
vector<glm::vec3> Editor::invertVector(const vector<glm::vec3>& points) {
    vector<glm::vec3> newVector;

    for (auto&point : points) {
        newVector.push_back(glm::vec3(point.x, point.z, point.y));
    }

    return newVector;
}

/// Gets the position of the points with the orthogonal value of the screen
vector<glm::vec3> Editor::getShaderPosition(const vector<glm::vec3>& vertices) {
    vector<glm::vec3> newVertices;

    for (auto&point : vertices) {
        glm::vec4 orthPoint = this->projectionMatrix * glm::vec4(point, 1);
        newVertices.push_back(glm::vec3(orthPoint.x, orthPoint.y, orthPoint.z));
    }

    return newVertices;
}

/// Get prepare the vectors of vertices from the curves to use in the writes
vector<glm::vec3> Editor::getOBJVertices(const vector<glm::vec3>& vertices) {
    vector<glm::vec3> newVertices;

    newVertices = this->getShaderPosition(vertices);
    newVertices = this->invertVector(newVertices);

    return newVertices;
}

/// Render the cross in the middle of the screen
void Editor::renderCross() {
    this->updateProjectionMatrix();

    glUseProgram(shaders[0]->getID());

    glBindVertexArray(crossVAO);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);
}

/// MARK: File Writers
/// Create the output.obj file
void Editor::writeObjFile(vector<glm::vec3>& internalVertices, vector<glm::vec3>& externalVertices, const string& filename, float scale) {
    if (internalVertices.empty() || externalVertices.empty()) {
        std::cout << "Vector is empty." << std::endl;
        return;
    } else {
        ofstream objFile(filename);

        if (!objFile.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return;
        }

        objFile << "# Atividade GB - Computacao Grafica\n";
        objFile << "# Alunos: Matheus Polonia e Pamela Santos\n";
        objFile << "mtllib track.mtl\n";
        objFile << "o Track\n";

        int N = (int)externalVertices.size();

        // Write internal vertices with scaling
        for (size_t i = 0; i < internalVertices.size(); ++i) {
            glm::vec3 scaledVertex = internalVertices[i] * scale;
            objFile << "v " << scaledVertex.x << " " << scaledVertex.y << " " << scaledVertex.z * -1<< "\n";
        }

        // Write external vertices with scaling
        for (size_t i = 0; i < externalVertices.size(); ++i) {
            glm::vec3 scaledVertex = externalVertices[i] * scale;
            objFile << "v " << scaledVertex.x << " " << scaledVertex.y << " " << scaledVertex.z * -1<< "\n";
        }

        // Write texture
        objFile << "vt " << 0.0 << " " << 0.0 << "\n";
        objFile << "vt " << 1.0 << " " << 0.0 << "\n";
        objFile << "vt " << 1.0 << " " << 1.0 << "\n";
        objFile << "vt " << 0.0 << " " << 1.0 << "\n";


        // Write normal;
        vector<glm::vec3> vn1;
        vector<glm::vec3> vn2;
        for (size_t i = 0; i <= N; ++i) {
            glm::vec3 A = externalVertices[i];
            glm::vec3 B = externalVertices[i+1];
            glm::vec3 C = internalVertices[i];
            glm::vec3 D = internalVertices[i+1];
            vn1.push_back(glm::cross(A - B, A - C));
            vn2.push_back(glm::cross(B - C, B - D));
        }
        for (size_t i = 0; i < vn1.size(); ++i) {
            objFile << "vn " << vn1[i].x << " " << vn1[i].y << " " << vn1[i].z << "\n";
        }
        for (size_t i = 0; i < vn2.size(); ++i) {
            objFile << "vn " << vn2[i].x << " " << vn2[i].y << " " << vn2[i].z << "\n";
        }

        objFile << "g Track_Material\n";
        objFile << "usemtl Material\n";

        // Write faces
        for (size_t i = 0; i <= N; ++i) {
            if (i == N) {
                objFile << "f " << i << "/1/1 " << i + 1 << "/2/1 " << i + N << "/4/1\n";
            } else {
                objFile << "f " << i << "/1/1 " << i + 1 << "/2/1 " << i + N << "/4/1\n";
                objFile << "f " << i + N << "/4/1 " << i + 1 << "/2/1 " << i + 1 + N << "/3/1\n";
            }
        }

        if (objFile.fail()) {
            std::cerr << "Error writing to file: " << filename << std::endl;
        } else {
            std::cout << "File written successfully: " << filename << std::endl;
        }

        objFile.close();
    }
}


/// Create the animaiton.txt file
void Editor::writeAnimationFile(const std::vector<glm::vec3>& vectorToWrite, const std::string& filename, float scale) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    for (const auto& vertex : vectorToWrite) {
        glm::vec3 scaledVertex = vertex * scale;
        file << scaledVertex.x << " " << scaledVertex.y << " " << scaledVertex.z * -1.f<< "\n";
    }

    file.close();

    std::cout << "Vector written to file: " << filename << std::endl;
}

/// MARK: Updates

/// Update Mouse
void Editor::updateMouseInput() {
    glfwGetCursorPos(this->window, &this->mouseX, &this->mouseY);

    int leftMouseButtonState = glfwGetMouseButton(this->window, GLFW_MOUSE_BUTTON_1);

    if (leftMouseButtonState == GLFW_PRESS && !leftMouseButtonPressed) {

        Point clickPoint = Point(mouseX, mouseY, 0.f);
        std::cout << "Position On Screen: (" << clickPoint.getPosition().x << ", " << clickPoint.getPosition().y << ")"<< "\n";
        glm::vec4 orthPosition = this->projectionMatrix * glm::vec4(clickPoint.getPosition(), 1);
        std::cout << "Position with orth: (" << orthPosition.x << ", " << orthPosition.y << ")"<< "\n";

        this->guidePoints.push_back(clickPoint);
        leftMouseButtonPressed = true;
    } else if (leftMouseButtonState == GLFW_RELEASE) {
        leftMouseButtonPressed = false;
    }
}

/// Update Keyboard
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

    /// 5 - EXTERNAL B-SPLINE CURVE
    if (glfwGetKey(this->window, GLFW_KEY_5) == GLFW_PRESS && !key5Pressed) {
        vector<glm::vec3> middleCurvePoints = this->bSplineCurve.getCurvePoints();
        vector<glm::vec3> externalCurvePoints = this->calculateSecondVector(middleCurvePoints, 10, false);
        this->externalBSplineCurve.generateCurveWithPoints(externalCurvePoints);
        key5Pressed = true;
    } else if (glfwGetKey(this->window, GLFW_KEY_5) == GLFW_RELEASE) {
        key5Pressed = false;
    }

    /// 6 - CLEAR GUIDE POINTS
    if (glfwGetKey(this->window, GLFW_KEY_6) == GLFW_PRESS && !key6Pressed) {
        this->guidePoints.clear();
        key6Pressed = true;
    } else if (glfwGetKey(this->window, GLFW_KEY_6) == GLFW_RELEASE) {
        key6Pressed = false;
    }

    /// 7 - CLEAR CURVES
    if (glfwGetKey(this->window, GLFW_KEY_7) == GLFW_PRESS && !key7Pressed) {
        this->bezierCurve.clear();
        this->hermiteCurve.clear();
        this->bSplineCurve.clear();
        key7Pressed = true;
    } else if (glfwGetKey(this->window, GLFW_KEY_7) == GLFW_RELEASE) {
        key7Pressed = false;
    }

    /// 9 - Generate OBJ
    if (glfwGetKey(this->window, GLFW_KEY_9) == GLFW_PRESS && !key9Pressed) {
        vector<glm::vec3> animationPath = this->getOBJVertices(this->bSplineCurve.getCurvePoints());
        // Change the path name
        string pathName = "/Users/matheuxlp/Documents/College/2023-2/ComputacaoGrafica/GB/TrabalhoGB/Editor/TrackEditor/TrackEditor/resources/output/animation.txt";
        this->writeAnimationFile(animationPath, pathName, 10);
        key9Pressed = true;
    } else if (glfwGetKey(this->window, GLFW_KEY_9) == GLFW_RELEASE) {
        key9Pressed = false;
    }

    /// 0 - Generate OBJ
    if (glfwGetKey(this->window, GLFW_KEY_0) == GLFW_PRESS && !key0Pressed) {
        vector<glm::vec3> externalCurvePoints = this->getOBJVertices(this->externalBSplineCurve.getCurvePoints());
        vector<glm::vec3> internalCurvePoints = this->getOBJVertices(this->internalBSplineCurve.getCurvePoints());
        // Change the path name
        string pathName = "/Users/matheuxlp/Documents/College/2023-2/ComputacaoGrafica/GB/TrabalhoGB/Editor/TrackEditor/TrackEditor/resources/output/output.obj";
        this->writeObjFile(externalCurvePoints, internalCurvePoints, pathName, 10);
        key0Pressed = true;
    } else if (glfwGetKey(this->window, GLFW_KEY_0) == GLFW_RELEASE) {
        key0Pressed = false;
    }
}
/// Update function
void Editor::update() {
    glfwPollEvents();

    this->updateKeyboardInput();
    this->updateMouseInput();
}

/// MARK: Render Function
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

    this->bSplineCurve.drawCurve(this->shaders[BASE_B_SPLINE]);
    this->internalBSplineCurve.drawCurve(this->shaders[INTERNAL_B_SPLINE]);
    this->externalBSplineCurve.drawCurve(this->shaders[EXTERNAL_B_SPLINE]);

    this->lineDrawer.drawLines(this->shaders[BASE], this->guidePoints);

    glfwSwapBuffers(window);
}

/// MARK: Static Functions
void Editor::framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH) {
    glViewport(0, 0, fbW, fbH);
};
