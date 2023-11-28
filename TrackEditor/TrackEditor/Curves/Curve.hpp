//
//  Curve.hpp
//  TrackEditor
//
//  Created by Matheus Polonia on 22/11/23.
//

#ifndef Curve_hpp
#define Curve_hpp

//GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include <vector>

#include "Shader.h"

#include <OpenGL/OpenGL.h>


using namespace std;

class Curve {
public:
    Curve() {}
    inline void setControlPoints(vector <glm::vec3> controlPoints) { this->controlPoints = controlPoints; }
    void setShader(Shader* shader);
    void generateCurve(int pointsPerSegment);
    void drawCurve(glm::vec4 color);
    int getNbCurvePoints() { return curvePoints.size(); }
    glm::vec3 getPointOnCurve(int i) { return curvePoints[i]; }

    bool hasCurvePoint() {
        if (this->curvePoints.size() != 0) {
            return true;
        } else {
            return false;
        }
    }
protected:
    vector <glm::vec3> controlPoints;
    vector <glm::vec3> curvePoints;
    glm::mat4 M; //Matriz de base
    GLuint VAO;
    Shader* shader;
};

#endif /* Curve_hpp */

