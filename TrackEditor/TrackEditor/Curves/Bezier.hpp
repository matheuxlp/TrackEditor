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

/*

 void Bezier::printData() {
     // Print the first element
     if (!this->curvePoints.empty()) {
         std::cout << "First element: (" << this->curvePoints[0].x << ", " << this->curvePoints[0].y << ", " << this->curvePoints[0].z << ")" << std::endl;
     } else {
         std::cout << "Vector is empty." << std::endl;
     }

     // Print the last element
     if (!this->curvePoints.empty()) {
         std::cout << "Last element: (" << this->curvePoints[this->curvePoints.size() - 1].x << ", " << this->curvePoints[this->curvePoints.size() - 1].y << ", " << this->curvePoints[this->curvePoints.size() - 1].z << ")" << std::endl;
     } else {
         std::cout << "Vector is empty." << std::endl;
     }
     std::cout << "Size" << this->curvePoints.size() << std::endl;

 }
 */
