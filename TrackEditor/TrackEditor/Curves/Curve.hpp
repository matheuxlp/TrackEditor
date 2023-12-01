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
#include "Point.h"

#include <OpenGL/OpenGL.h>


using namespace std;

class Curve {
public:
    Curve() {}
    void setControlPoints(vector<Point> controlPoints) {
        this->controlPoints = controlPoints;
        for (auto&point : this->controlPoints) {
            point.printPosition();
        }
    }
    
    void addControlPoint(Point controlPoint) {
        this->controlPoints.push_back(controlPoint);
    }

    void clearControlPoints() {
        this->controlPoints.clear();
    }

    void clearCurvePoints() {
        this->curvePoints.clear();
    }

    void clear() {
        this->clearControlPoints();
        this->clearCurvePoints();
    }

    void generateCurve(int pointsPerSegment);
    void drawCurve(Shader* shader, glm::vec4 color);

    int getNbCurvePoints() {
        return static_cast<int>(curvePoints.size());
    }

    glm::vec3 getPointOnCurve(int i) {
        return curvePoints[i];
    }

    bool hasCurvePoint() {
        if (this->curvePoints.size() != 0) {
            return true;
        } else {
            return false;
        }
    }
protected:
    vector <Point> controlPoints;
    vector <glm::vec3> curvePoints;
    glm::mat4 M; //Matriz de base
    GLuint VAO;
    Shader* shader;
};

#endif /* Curve_hpp */

