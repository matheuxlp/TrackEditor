//
//  Bezier.hpp
//  TrackEditor
//
//  Created by Matheus Polonia on 22/11/23.
//

#ifndef Bezier_hpp
#define Bezier_hpp

#include "Curve.hpp"

class Bezier :
    public Curve {
public:
    Bezier();
    void generateCurve(int pointsPerSegment);
};

#endif /* Bezier_hpp */

