//
//  Point.h
//  TrackEditor
//
//  Created by Matheus Polonia on 24/11/23.
//

#ifndef Point_h
#define Point_h

#include <glm.hpp>
#include <vec2.hpp>

class Point {
private:
    glm::vec2 position;

public:
    Point(float x, float y) {
        this->position.x = x;
        this->position.y = y;
    }

    ~Point() {}

    // Getter for position
    glm::vec2 getPosition() {
        return this->position;
    }

    // Setter for position
    void setPosition(float x, float y) {
        this->position.x = x;
        this->position.y = y;
    }

    // Function to send position to a shader
    void sendToShader(Shader& program) {
        program.setVec2f(this->position, "position");
    }
};


#endif /* Point_h */
