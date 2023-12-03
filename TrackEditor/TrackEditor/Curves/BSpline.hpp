#ifndef BSpline_hpp
#define BSpline_hpp

#include "Curve.hpp"

class BSpline: public Curve {
    public:
        BSpline();
        void generateCurve(int pointsPerSegment);
        void generateCurveWithPoints(vector<glm::vec3> curvePoints);
};

#endif /* BSpline_hpp */
