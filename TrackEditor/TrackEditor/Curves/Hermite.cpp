//
//  Hermite.cpp
//  TrackEditor
//
//  Created by Matheus Polonia on 22/11/23.
//

#include "Hermite.hpp"

Hermite::Hermite() {}

void Hermite::generateCurve(int pointsPerSegment) {
    if (controlPoints.size() != 4) {
        std::cerr << "Less than 4 points" << std::endl;
        return;
    }

    float inc = 1.0 / static_cast<float>(pointsPerSegment);

    float WG = 0.5; // Fator atenuação das tangentes T1 e T2

    glm::vec3 P1 = controlPoints[0].getPosition();
    glm::vec3 P4 = controlPoints[3].getPosition();
    glm::vec3 R1 = WG * (controlPoints[1].getPosition() - P1);
    glm::vec3 R4 = WG * (controlPoints[2].getPosition() - P4);

    for (float t = 0.0; t <= 1.0; t += inc) {
        float x = ((2 * pow(t, 3) - 3 * pow(t, 2) + 0 * t + 1) * P1.x +
                   (-2 * pow(t, 3) + 3 * pow(t, 2) + 0 * t + 0) * P4.x +
                   (1 * pow(t, 3) - 2 * pow(t, 2) + 1 * t + 0) * R1.x +
                   (1 * pow(t, 3) - 1 * pow(t, 2) + 0 * t + 0) * R4.x);

        float y = ((2 * pow(t, 3) - 3 * pow(t, 2) + 0 * t + 1) * P1.y +
                   (-2 * pow(t, 3) + 3 * pow(t, 2) + 0 * t + 0) * P4.y +
                   (1 * pow(t, 3) - 2 * pow(t, 2) + 1 * t + 0) * R1.y +
                   (1 * pow(t, 3) - 1 * pow(t, 2) + 0 * t + 0) * R4.y);

        float z = ((2 * pow(t, 3) - 3 * pow(t, 2) + 0 * t + 1) * P1.z +
                   (-2 * pow(t, 3) + 3 * pow(t, 2) + 0 * t + 0) * P4.z +
                   (1 * pow(t, 3) - 2 * pow(t, 2) + 1 * t + 0) * R1.z +
                   (1 * pow(t, 3) - 1 * pow(t, 2) + 0 * t + 0) * R4.z);

        curvePoints.push_back(glm::vec3(x, y, z));
    }

    // OpenGL Setup
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(GLfloat) * 3, curvePoints.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

