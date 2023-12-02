//
//  Curve.cpp
//  TrackEditor
//
//  Created by Matheus Polonia on 22/11/23.
//

#include "Curve.hpp"

void Curve::drawCurve(Shader* shader) {
    shader->use();

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINE_LOOP, 0, curvePoints.size());
    glBindVertexArray(0);
}

