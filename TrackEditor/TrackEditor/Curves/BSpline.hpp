//
//  BSpline.hpp
//  TrackEditor
//
//  Created by Matheus Polonia on 29/11/23.
//

#ifndef BSpline_hpp
#define BSpline_hpp

#include "Curve.hpp"

class BSpline :
public Curve {
public:
    BSpline();
    void generateCurve(int pointsPerSegment);
    void generateCurveWithPoints(vector<glm::vec3> curvePoints);
};

#endif /* BSpline_hpp */
