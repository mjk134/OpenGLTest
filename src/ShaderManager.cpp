#include <string>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>

class ShaderManager {
public:
    ShaderManager(const std::string filepath) {
        this->shaderFilepath = filepath;
    }

    unsigned int CreateProgram()
    {
        std::fstream shaderFile(this->shaderFilepath);
        std::string line;
        unsigned int shaderType = GL_VERTEX_SHADER;
        std::string shaderFoundType = "vertex";
        std::string vertexShaderSource = "";
        std::string fragmentShaderSource = "";

        std::cout << "starting loop" << std::endl;

        while (std::getline(shaderFile, line)) {
            std::size_t found = line.find("#shader");
            if (found != std::string::npos) {
                shaderFoundType = line.substr(1, found);
                if (shaderFoundType == "vertex") {
                    shaderType = GL_VERTEX_SHADER;
                }
                else {
                    shaderType = GL_FRAGMENT_SHADER;
                }
            }
            else {
                switch (shaderType) {
                    case GL_VERTEX_SHADER:
                        vertexShaderSource += line;
                        break;
                    case GL_FRAGMENT_SHADER:
                        fragmentShaderSource += line;
                        break;
                }
            }
        }
        std::cout << fragmentShaderSource << std::endl;
        shaderFile.close();
        return 0;
    }


private:
    std::string shaderFilepath;

    static unsigned int CompileShader(const GLenum type, const std::string& shaderSource)
    {
        unsigned int buffer = glCreateShader(type);
        const char* source = shaderSource.c_str();

        glShaderSource(buffer, 1, &source, nullptr);
        glCompileShader(buffer);

        int compileResult;
        glGetShaderiv(buffer, GL_COMPILE_STATUS, &compileResult);
        if (compileResult == GL_FALSE)
        {
            int length;
            glGetShaderiv(buffer, GL_INFO_LOG_LENGTH, &length);
            char* message = (char*)_malloca(length * sizeof(char));
            glGetShaderInfoLog(buffer, length, &length, message);
            std::cout << "Failed to compiled shader: " << std::endl;
            std::cout << message << std::endl;
            glDeleteShader(buffer);
            return 0;
        }

        return buffer;
    }

    static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
    {
        unsigned int programBuffer = glCreateProgram();
        unsigned int vertexShaderBuffer = CompileShader(GL_VERTEX_SHADER, vertexShader);
        unsigned int fragmentShaderBuffer = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

        glAttachShader(programBuffer, vertexShaderBuffer);
        glAttachShader(programBuffer, fragmentShaderBuffer);
        glLinkProgram(programBuffer);
        glValidateProgram(programBuffer);

        glDeleteShader(vertexShaderBuffer);
        glDeleteShader(fragmentShaderBuffer);

        return programBuffer;
    }

    static std::string GetShaderSource(const GLenum type)
    {

    }
};