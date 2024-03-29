#ifndef Point_h
#define Point_h

#include <glm.hpp>
#include <vec2.hpp>
#include "Shader.h"

class Point {
private:
    glm::vec3 position;
    GLuint VAO;

public:
    Point(float x, float y, float z) {
        this->position.x = x;
        this->position.y = y;
        this->position.z = z;
    }

    ~Point() {}

    void printPosition() {
        std::cout << "Point(" << this->position.x <<  ", " << this->position.y << ")"<< "\n";
    }

    glm::vec3 getPosition() const {
        return this->position;
    }

    void setPosition(float x, float y, float z) {
        this->position.x = x;
        this->position.y = y;
        this->position.z = z;
    }

    void sendToShader(Shader& program) const {
        program.setVec3f(this->position, "position");
    }

    void drawPoint(Shader* shader) {
        shader->use();
        
        glBindVertexArray(0);

        GLuint VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3), &(this->position), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, 1);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        glBindVertexArray(0);
        glUseProgram(0);
    }
};

#endif /* Point_h */
