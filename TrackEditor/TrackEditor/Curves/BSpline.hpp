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
    void printData();
};

#endif /* BSpline_hpp */
