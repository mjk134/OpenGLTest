#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "ShaderManager.cpp"

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

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
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

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(480, 480, "Vinga is high", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum glewError = glewInit();

    if (GLEW_OK != glewError)
    {
        std::cout << glewGetErrorString(glewError) << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[6] = {
        -1.0f, -1.0f,
         0.0f,  1.0f,
         1.0f, -1.0f,
    };

    float squarePositions[8] = {
       -0.5f,  0.5f,
        0.5f,  0.5f,
        0.5f, -0.5f,
       -0.5f, -0.5f
    };

    // DrawCircle(3.0f);

    unsigned int buffer;
    glGenBuffers(1, &buffer); // assigns the buffer state id, given a pointer
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // binding is esentially selecting the state
    // size of data HAS to be in bytes: either sizeof(array) OR value * sizeof(type)
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), squarePositions, GL_STATIC_DRAW);
    /*
    * index: which vertex to start with
    * size: the actual size of the vertex position so 1, 2, 3, 4
    * type: type of the vertex in this case: float
    * stride: difference in bytes between each vertex
    * pointer: offset of the first attribute
    */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // outputs id of the buffer
    std::cout << buffer << std::endl;

    std::string vertexShader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    

    std::string fragmentShader =
        R"glsl(#version 330 core

        layout(location = 0) out vec4 fragColor;

        void main()
        {
            fragColor = vec4(255.0/255.0, 127.0/255.0, 80.0/255.0, 1.0);
        })glsl";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    ShaderManager testShader("res/shader/basic.shader");

    testShader.CreateProgram();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}