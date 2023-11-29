//
//  LineDrawer.h
//  TrackEditor
//
//  Created by Matheus Polonia on 29/11/23.
//

#ifndef LineDrawer_h
#define LineDrawer_h

#include <vector>
#include "Shader.h" // Include your Shader class definition
#include "Point.h"

class LineDrawer {
public:
    static void drawLines(Shader* program, const std::vector<Point*>& points) {
        // Convert Point pointers to glm::vec3
        std::vector<glm::vec3> positions;
        for (const auto& point : points) {
            positions.push_back(point->getPosition());
        }

        glUseProgram(program->getID());

        GLuint VBO, VAO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &(positions[0]), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        // Draw lines
        glBindVertexArray(VAO);
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(positions.size()));

        // Clean up
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        glBindVertexArray(0);
        glUseProgram(0);
    }
};

#endif /* LineDrawer_h */
