//
//  Bezier.cpp
//  TrackEditor
//
//  Created by Matheus Polonia on 22/11/23.
//

#include "Bezier.hpp"

Bezier::Bezier() {
    M = glm::mat4(-1, 3, -3, 1,
        3, -6, 3, 0,
        -3, 3, 0, 0,
        1, 0, 0, 0
    );
}

void Bezier::printData() {
    // Print the first element
    if (!this->curvePoints.empty()) {
        std::cout << "First element: (" << this->curvePoints[0].x << ", " << this->curvePoints[0].y << ", " << this->curvePoints[0].z << ")" << std::endl;
    } else {
        std::cout << "Vector is empty." << std::endl;
    }

    // Print the last element
    if (!this->curvePoints.empty()) {
        std::cout << "Last element: (" << this->curvePoints[this->curvePoints.size() - 1].x << ", " << this->curvePoints[this->curvePoints.size() - 1].y << ", " << this->curvePoints[this->curvePoints.size() - 1].z << ")" << std::endl;
    } else {
        std::cout << "Vector is empty." << std::endl;
    }
    std::cout << "Size" << this->curvePoints.size() << std::endl;

}

void Bezier::generateCurve(int pointsPerSegment) {
    float step = 1.0 / (float)pointsPerSegment;

    float t = 0;

    int nControlPoints = controlPoints.size();

    for (int i = 0; i < nControlPoints - 3; i += 3) {

        for (float t = 0.0; t <= 1.0; t += step) {
            glm::vec3 p;

            glm::vec4 T(t * t * t, t * t, t, 1);

            glm::vec3 P0 = controlPoints[i].getPosition();
            glm::vec3 P1 = controlPoints[i + 1].getPosition();
            glm::vec3 P2 = controlPoints[i + 2].getPosition();
            glm::vec3 P3 = controlPoints[i + 3].getPosition();

            glm::mat4x3 G(P0, P1, P2, P3);

            p = G * M * T;  //---------

            curvePoints.push_back(p);
        }
    }

    //Gera o VAO
    GLuint VBO;

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(GLfloat) * 3, curvePoints.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(GLfloat),
                          (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

