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

void Bezier::generateCurve(int pointsPerSegment) {
    float step = 1.0 / static_cast<float>(pointsPerSegment);

    int nControlPoints = controlPoints.size();

    // Check if there are at least 4 control points
    if (nControlPoints < 4) {
        // Handle the case when there are not enough control points
        // You can add error handling, logging, or return early as needed.
        std::cerr << "Minus then 4 points" << std::endl;
        return;
    }

    float x, y, z;

    // Iterate over the control points
    for (int i = 0; i < nControlPoints - 3; i += 3) {
        float t = 0.0;

        while (t <= 1.0) {
            x = ((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * controlPoints[i].getPosition().x +
                 (3 * pow(t, 3) - 6 * pow(t, 2) + 3 * t) * controlPoints[i + 1].getPosition().x +
                 (-3 * pow(t, 3) + 3 * pow(t, 2)) * controlPoints[i + 2].getPosition().x +
                 pow(t, 3) * controlPoints[i + 3].getPosition().x);

            y = ((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * controlPoints[i].getPosition().y +
                 (3 * pow(t, 3) - 6 * pow(t, 2) + 3 * t) * controlPoints[i + 1].getPosition().y +
                 (-3 * pow(t, 3) + 3 * pow(t, 2)) * controlPoints[i + 2].getPosition().y +
                 pow(t, 3) * controlPoints[i + 3].getPosition().y);

            z = ((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * controlPoints[i].getPosition().z +
                 (3 * pow(t, 3) - 6 * pow(t, 2) + 3 * t) * controlPoints[i + 1].getPosition().z +
                 (-3 * pow(t, 3) + 3 * pow(t, 2)) * controlPoints[i + 2].getPosition().z +
                 pow(t, 3) * controlPoints[i + 3].getPosition().z);

            curvePoints.push_back(glm::vec3(x, y, z));

            t += step;
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

