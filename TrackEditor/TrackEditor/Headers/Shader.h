#ifndef Shader_h
#define Shader_h

// Standard
#include<iostream>
#include<fstream>
#include<string>

// GLEW
#include<glew.h>

// GLFW
#include<glfw3.h>

// GLM
#include<glm.hpp>
#include<vec2.hpp>
#include<vec3.hpp>
#include<vec4.hpp>
#include<mat4x4.hpp>
#include<gtc/type_ptr.hpp>

// MacOS OpenGL
#include <OpenGL/OpenGL.h>

class Shader {
private:
    GLuint id;

    std::string loadShaderSource(const char* fileName) {
        std::string temp = "";
        std::string src = "";

        std::ifstream in_file;

        in_file.open(fileName);

        if (in_file.is_open()) {
            while (std::getline(in_file, temp))
                src += temp + "\n";
        }
        else {
            std::cout << "Error: Shader could not open file " << fileName << "\n";
        }

        in_file.close();

        std::string versionNr = std::to_string(4) + std::to_string(1) + "0";

        src.replace(src.find("#version"), 12, ("#version " + versionNr));

        return src;
    }

    GLuint loadShader(GLenum type, const char* fileName) {
        char infoLog[512];
        GLint success;

        GLuint shader = glCreateShader(type);
        std::string str_src = this->loadShaderSource(fileName);
        const GLchar* src = str_src.c_str();
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COULD_NOT_COMPILE_SHADER: " << fileName << "\n";
            std::cout << infoLog << "\n";
        }
        std::string versionNr = reinterpret_cast<const char*>(glGetString(GL_VERSION));

        return shader;
    }

    void linkProgram(GLuint vertexShader, GLuint fragmentShader) {
        char infoLog[512];
        GLint success;

        this->id = glCreateProgram();

        glAttachShader(this->id, vertexShader);


        glAttachShader(this->id, fragmentShader);

        glLinkProgram(this->id);

        glGetProgramiv(this->id, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(this->id, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COULD_NOT_LINK_PROGRAM" << "\n";
            std::cout << infoLog << "\n";
        }

        glUseProgram(0);
    }

public:
    Shader(const char* vertexFile, const char* fragmentFile) {
        GLuint vertexShader = 0;
        GLuint fragmentShader = 0;

        vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);

        fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);

        this->linkProgram(vertexShader, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        std::cerr << "Created Shaders" << std::endl;
    }

    ~Shader() {
        glDeleteProgram(this->id);
    }

    void use() {
        glUseProgram(this->id);
    }

    void unuse() {
        glUseProgram(0);
    }

    void set1i(GLint value, const GLchar* name) {
        this->use();

        glUniform1i(glGetUniformLocation(this->id, name), value);

        this->unuse();
    }

    void set1f(GLfloat value, const GLchar* name) {
        this->use();

        glUniform1f(glGetUniformLocation(this->id, name), value);

        this->unuse();
    }

    void setVec2f(glm::fvec2 value, const GLchar* name) {

        this->use();

        glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

        this->unuse();
    }

    void setVec3f(glm::fvec3 value, const GLchar* name) {
        this->use();

        glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

        this->unuse();
    }

    void setVec4f(glm::fvec4 value, const GLchar* name) {
        this->use();

        glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

        this->unuse();
    }

    void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE) {
        this->use();

        glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

        this->unuse();
    }

    void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE) {
        this->use();

        glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose, glm::value_ptr(value));

        this->unuse();
    }

    GLuint getID() const {
        return this->id;
    }

};


#endif /* Shader_h */
