#ifndef LineDrawer_h
#define LineDrawer_h

#include <vector>
#include "Shader.h"
#include "Point.h"

class LineDrawer {
private:
    GLuint VAO;

public:
    LineDrawer() {}

    ~LineDrawer() {}

    void drawLines(Shader* shader, const std::vector<Point>& points) {
        std::vector<glm::vec3> positions;
        for (const auto& point : points) {
            positions.push_back(point.getPosition());
        }

        shader->use();

        GLuint VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &(positions[0]), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(positions.size()));

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        glBindVertexArray(0);
        glUseProgram(0);
    }
};

#endif /* LineDrawer_h */
