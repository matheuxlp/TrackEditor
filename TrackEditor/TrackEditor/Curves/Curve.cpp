#include "Curve.hpp"

void Curve::drawCurve(Shader* shader) {
    shader->use();

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, (int)curvePoints.size());
    glBindVertexArray(0);
}

