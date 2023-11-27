//
//  Curve.cpp
//  TrackEditor
//
//  Created by Matheus Polonia on 22/11/23.
//

#include "Curve.hpp"

void Curve::setShader(Shader* shader) {
    this->shader = shader;
    shader->use();
}

void Curve::drawCurve(glm::vec4 color) {
    glm::fvec4 colorVec4(color.r, color.g, color.b, color.a);
    shader->setVec4f(colorVec4, "finalColor");

    glBindVertexArray(VAO);
    // Chamada de desenho - drawcall
    // CONTORNO e PONTOS - GL_LINE_LOOP e GL_POINTS
    glDrawArrays(GL_LINE_STRIP, 0, curvePoints.size());
    //glDrawArrays(GL_POINTS, 0, curvePoints.size());
    glBindVertexArray(0);
}

