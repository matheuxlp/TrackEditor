//
//  Curve.cpp
//  TrackEditor
//
//  Created by Matheus Polonia on 22/11/23.
//

#include "Curve.hpp"

void Curve::drawCurve(Shader* shader, glm::vec4 color) {
    glm::fvec4 colorVec4(color.r, color.g, color.b, color.a);
    shader->use();
    //shader->setVec4f(colorVec4, "finalColor");

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, curvePoints.size());
    glBindVertexArray(0);
}

