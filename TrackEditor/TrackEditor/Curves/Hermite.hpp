//
//  Hermite.hpp
//  TrackEditor
//
//  Created by Matheus Polonia on 22/11/23.
//

#ifndef Hermite_hpp
#define Hermite_hpp

#include "Curve.hpp"

class Hermite :
    public Curve
{
public:
    Hermite();
    void generateCurve(int pointsPerSegment);
};

#endif /* Hermite_hpp */
