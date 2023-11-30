//
//  Hermite.cpp
//  TrackEditor
//
//  Created by Matheus Polonia on 22/11/23.
//

#include "Hermite.hpp"

Hermite::Hermite() {
    M = glm::mat4(2, -2, 1, 1,
                  -3, 3, -2, -1,
                  0, 0, 1, 0,
                  1, 0, 0, 0
    );
}

void Hermite::generateCurve(int pointsPerSegment) {
    if (controlPoints.size() != 4) {
        std::cout << "Less then 4 pointsn";
        // Only execute the generateCurve if there are exactly four control points
        return;
    }

    float step = 1.0 / (float)pointsPerSegment;

    for (float t = 0.0; t <= 1.0; t += step) {
        glm::vec3 p;

        glm::vec4 T(t * t * t, t * t, t, 1);

        glm::vec3 P0 = controlPoints[0].getPosition();
        glm::vec3 P1 = controlPoints[3].getPosition();
        glm::vec3 T0 = controlPoints[1].getPosition() - P0;
        glm::vec3 T1 = controlPoints[2].getPosition() - P1;

        glm::mat4x3 G(P0, P1, T0, T1);

        p = G * M * T;  //---------

        curvePoints.push_back(p);
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
