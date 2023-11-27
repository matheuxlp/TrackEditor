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
    float step = 1.0 / (float)pointsPerSegment;

    float t = 0;

    int nControlPoints = controlPoints.size();

    for (int i = 0; i < nControlPoints - 3; i += 3) {

        for (float t = 0.0; t <= 1.0; t += step) {
            glm::vec3 p;

            glm::vec4 T(t * t * t, t * t, t, 1);

            glm::vec3 P0 = controlPoints[i];
            glm::vec3 P1 = controlPoints[i + 1];
            glm::vec3 P2 = controlPoints[i + 2];
            glm::vec3 P3 = controlPoints[i + 3];

            glm::mat4x3 G(P0, P1, P2, P3);

            p = G * M * T;  //---------

            curvePoints.push_back(p);
        }
    }

    //Gera o VAO
    GLuint VBO;

    //Gera��o do identificador do VBO
    glGenBuffers(1, &VBO);

    //Faz a conex�o (vincula) do buffer como um buffer de array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Envia os dados do array de floats para o buffer da OpenGl
    glBufferData(GL_ARRAY_BUFFER, curvePoints.size() * sizeof(GLfloat) * 3, curvePoints.data(), GL_STATIC_DRAW);

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
}

