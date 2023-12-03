#ifndef Hermite_hpp
#define Hermite_hpp

#include "Curve.hpp"

class Hermite: public Curve {
    public:
        Hermite();
        void generateCurve(int pointsPerSegment);
};

#endif /* Hermite_hpp */
