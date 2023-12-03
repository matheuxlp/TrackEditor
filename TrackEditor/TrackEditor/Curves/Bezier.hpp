#ifndef Bezier_hpp
#define Bezier_hpp

#include "Curve.hpp"

class Bezier: public Curve {
    public:
        Bezier();
        void generateCurve(int pointsPerSegment);
};

#endif /* Bezier_hpp */
