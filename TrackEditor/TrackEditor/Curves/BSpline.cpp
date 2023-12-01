//
//  BSpline.cpp
//  TrackEditor
//
//  Created by Matheus Polonia on 29/11/23.
//

#include "BSpline.hpp"

BSpline::BSpline() {
    M = glm::mat4(-1, 3, -3, 1,
        3, -6, 3, 0,
        -3, 3, 0, 0,
        1, 0, 0, 0
    );
}

void BSpline::generateCurve(int pointsPerSegment) {
    float inc = 1.0 / static_cast<float>(pointsPerSegment);

    int N = controlPoints.size();

    for (int i = 0; i < N; i++) {
        for (float t = 0.0; t <= 1.0; t += inc) {
            float x = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * controlPoints[i].getPosition().x +
                        (3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4) * controlPoints[(i + 1) % N].getPosition().x +
                        (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) * controlPoints[(i + 2) % N].getPosition().x +
                        (1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0) * controlPoints[(i + 3) % N].getPosition().x) / 6);

            float y = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * controlPoints[i].getPosition().y +
                        (3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4) * controlPoints[(i + 1) % N].getPosition().y +
                        (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) * controlPoints[(i + 2) % N].getPosition().y +
                        (1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0) * controlPoints[(i + 3) % N].getPosition().y) / 6);

            float z = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * controlPoints[i].getPosition().z +
                        (3 * pow(t, 3) - 6 * pow(t, 2) + 0 * t + 4) * controlPoints[(i + 1) % N].getPosition().z +
                        (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) * controlPoints[(i + 2) % N].getPosition().z +
                        (1 * pow(t, 3) + 0 * pow(t, 2) + 0 * t + 0) * controlPoints[(i + 3) % N].getPosition().z) / 6);

            curvePoints.push_back(glm::vec3(x, y, z));
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
