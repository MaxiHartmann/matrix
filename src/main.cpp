#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

const char* vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 position;

uniform float yOffset;

void main()
{
    vec3 movedPosition = position;
    movedPosition.y += yOffset;

    gl_Position = vec4(movedPosition, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core

uniform vec3 uColor;

out vec4 color;

void main()
{
    color = vec4(uColor, 1.0);
}
)";

void checkShader(unsigned int shader)
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[1024];
        glGetShaderInfoLog(
            shader,
            sizeof(infoLog),
            nullptr,
            infoLog
        );

        std::cerr << "Shader compilation failed:\n"
                  << infoLog << '\n';
    }
}

void framebufferSizeCallback(GLFWwindow* /*window*/, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(
            GLFW_OPENGL_PROFILE,
            GLFW_OPENGL_CORE_PROFILE
            );

    GLFWwindow* window =
        glfwCreateWindow(800, 600, "Matrix", nullptr, nullptr);

    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);


    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW\n";
        return 1;
    }

    glfwSetFramebufferSizeCallback(
        window,
        framebufferSizeCallback
    );

    int framebufferWidth;
    int framebufferHeight;

    glfwGetFramebufferSize(
        window,
        &framebufferWidth,
        &framebufferHeight
    );

    glViewport(
        0,
        0,
        framebufferWidth,
        framebufferHeight
    );

    float vertices[] = {
        // first triangle
        -0.2f, -0.3f, 0.0f,
         0.2f, -0.3f, 0.0f,
         0.2f,  0.3f, 0.0f

        // second triangle
        -0.2f, -0.3f, 0.0f,
         0.2f,  0.3f, 0.0f,
        -0.2f,  0.3f, 0.0f
    };
    
    unsigned int VAO;
    unsigned int VBO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );
    
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        nullptr
    );
    
    glEnableVertexAttribArray(0);

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    checkShader(vertexShader);
    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    checkShader(fragmentShader);
    
    unsigned int shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    glLinkProgram(shaderProgram);

    int yOffsetLocation = glGetUniformLocation(shaderProgram, "yOffset");
    int colorLocation = glGetUniformLocation(shaderProgram, "uColor");
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float lastTime = static_cast<float>(glfwGetTime());
    float yPosition = 1.0f;
    float speed = 0.5f;

    while (!glfwWindowShouldClose(window))
    {
        float currentTime = static_cast<float>(glfwGetTime());
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;


        yPosition -= speed * deltaTime;

        if (yPosition < - 1.3f)
        {
            yPosition = 1.3f;
        }

        glClearColor(0.02f, 0.02f, 0.02f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float r = 0.5f * std::sin(currentTime) + 0.5f;
        float g = 0.5f * std::sin(currentTime + 2.0f) + 0.5f;
        float b = 0.5f * std::sin(currentTime + 4.0f) + 0.5f;

        glUseProgram(shaderProgram);

        glUniform1f(yOffsetLocation, yPosition);
        glUniform3f(colorLocation, r, g, b);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
