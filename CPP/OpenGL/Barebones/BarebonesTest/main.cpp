#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

// settings
unsigned int scrWidth = 800;
unsigned int scrHeight = 600;
const char *title = "Pong";

/*
    VAO structure storing Array Object and its Buffer Objects
*/
struct VAO
{
    GLuint val;
    GLuint posVBO;
    GLuint offsetVBO;
    GLuint sizeVBO;
    GLuint EBO;
};

/*
    initialization methods
*/

// initialize GLFW
void initGLFW(unsigned int versionMajor, unsigned int versionMinor)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

// create window
void createWindow(GLFWwindow *&window, const char *title, unsigned int width, unsigned int height, GLFWframebuffersizefun framebufferSizeCallback)
{
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

// callback for window size change
void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    scrWidth = width;
    scrHeight = height;
}

// load GLAD library
bool loadGlad()
{
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

/*
    shader methods
*/

// read file at filepath
std::string readFile(const char *filename)
{
    std::ifstream file;
    std::stringstream buf;

    std::string ret = "";

    // open file
    file.open(filename);

    if (file.is_open())
    {
        // read buffer
        buf << file.rdbuf();
        ret = buf.str();
    }
    else
    {
        std::cout << "Could not open " << filename << std::endl;
    }

    // close file
    file.close();

    return ret;
}

// generate shader
int genShader(const char *filepath, GLenum type)
{
    std::string shaderSrc = readFile(filepath);
    const GLchar *shader = shaderSrc.c_str();

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    int shaderObj = glCreateShader(type);
    glShaderSource(shaderObj, 1, &shader, NULL);
    glCompileShader(shaderObj);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderObj, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        return -1;
    }

    return shaderObj;
}

// generate shader program
int genShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath)
{
    int shaderProgram = glCreateProgram();

    int vertexShader = genShader(vertexShaderPath, GL_VERTEX_SHADER);
    int fragmentShader = genShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    if (vertexShader == -1 || fragmentShader == -1)
    {
        return -1;
    }

    // link shaders
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
        return -1;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// bind shader
void bindShader(GLuint shaderProgram)
{
    glUseProgram(shaderProgram);
}

// set orthographic projection matrix
void setOrthographicProjection(GLuint shaderProgram, float left, float right, float bottom, float top, float near, float far)
{
    // https://en.wikipedia.org/wiki/Orthographic_projection
    float mat[4][4] = {
        {2.0f / (right - left), 0.0f, 0.0f, 0.0f},
        {0.0f, 2.0f / (top - bottom), 0.0f, 0.0f},
        {0.0f, 0.0f, -2.0f / (far - near), 0.0f},
        {-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1.0f}};

    bindShader(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &mat[0][0]);
}

// delete shader
void deleteShader(GLuint shaderProgram)
{
    glDeleteProgram(shaderProgram);
}

/*
    Array Object/Buffer Object methods
*/

// generate Vertex Array Object
void genVAO(VAO *vao)
{
    glGenVertexArrays(1, &vao->val);
    glBindVertexArray(vao->val);
}

// generate array containing vertices of circle

/*


    SKIP
    SKIP
    SKIP


*/
void gen2DCircleArrays(float *&vertices, unsigned int *&indices, unsigned int noTriangles, float radius = 0.5f)
{
    vertices = new float[(noTriangles + 1) * 2];
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;

    indices = new unsigned int[noTriangles * 3];

    float pi = 4 * atanf(1.0f);
    float noTrianglesF = (float)noTriangles;
    float theta;

    for (unsigned int i = 0; i < noTriangles; i++)
    {
        /*
            r = 0.5 (diameter = 1)
            theta = i / noTriangles * 2 * pi
            x = rcos(theta) = vertices[(i + 1) * 2]
            y = rsin(theta) = vertices[(i + 1) * 2 + 1]
        */

        // update theta
        theta = i / noTrianglesF * 2 * pi;

        // set vertices
        vertices[(i + 1) * 2] = radius * cosf(theta);
        vertices[(i + 1) * 2 + 1] = radius * sinf(theta);

        // add indices
        indices[i * 3 + 0] = 0;
        indices[i * 3 + 1] = i + 1;
        indices[i * 3 + 2] = i + 2;
    }

    // set last index to wrap around to start
    indices[(noTriangles - 1) * 3 + 2] = 1;
}

// generate Buffer object of certain type
template <typename T>
void genBufferObject(GLuint &VBO, GLenum type, GLuint noElements, T *data, GLenum usage)
{
    glGenBuffers(1, &VBO);
    glBindBuffer(type, VBO);
    glBufferData(type, noElements * sizeof(T), data, usage);
}

// update data (glBufferSubData)
template <typename T>
void updateData(GLuint &VBO, GLintptr offset, GLuint noElements, T *data)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, offset, noElements * sizeof(T), data);
}

// set attribute pointers
template <typename T>
void setAttPointer(GLuint &VBO, GLuint idx, GLint size, GLenum type, GLuint stride, GLuint offset, GLuint divisor = 0)
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(idx, size, type, GL_FALSE, stride * sizeof(T), (void *)(offset * sizeof(T)));
    glEnableVertexAttribArray(idx);
    if (divisor > 0)
    {
        // reset _idx_ attribute every _divisor_ iteration (instancing)
        glVertexAttribDivisor(idx, divisor);
    }
}

// draw VAO
void draw(VAO vao, GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount = 1)
{
    glBindVertexArray(vao.val);
    glDrawElementsInstanced(mode, count, type, (void *)indices, instanceCount);
}

// unbind buffer of type
void unbindBuffer(GLenum type)
{
    glBindBuffer(type, 0);
}

// unbind array object
void unbindVAO()
{
    glBindVertexArray(0);
}

// de-allocate VAO/VBO methods
void cleanup(VAO vao)
{
    glDeleteVertexArrays(1, &vao.val);
    glDeleteBuffers(1, &vao.posVBO);
    glDeleteBuffers(1, &vao.sizeVBO);
    glDeleteBuffers(1, &vao.offsetVBO);
    glDeleteBuffers(1, &vao.EBO);
}

/*
    main loop methods
*/

// process input
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// clear screen
void clearScreen()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// new frame
void newFrame(GLFWwindow *window)
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

/*
    cleanup methods
*/

// terminate glfw
void cleanup()
{
    glfwTerminate();
}

int main()
{
    // timing
    double dt = 0.0; // time between current frame and last frame
    double lastFrame = 0.0;

    // glfw: initialize and configure
    // ------------------------------
    initGLFW(3, 3);

    // glfw window creation
    // --------------------
    GLFWwindow *window = nullptr;
    createWindow(window, title, scrWidth, scrHeight, framebufferSizeCallback);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        cleanup();
        return -1;
    }

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!loadGlad())
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        cleanup();
        return -1;
    }

    GLuint shaderProgram = genShaderProgram("main.vs", "main.fs");
    setOrthographicProjection(shaderProgram, 0, scrWidth, 0, scrHeight, 0.0f, 1.0f);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float *circleVertices = nullptr;
    unsigned int *circleIndices = nullptr;
    unsigned int noTriangles = 50;
    gen2DCircleArrays(circleVertices, circleIndices, noTriangles);
    // IMPORTANT: REPLACE WITH RECTANGLE

    float circleOffset[2] = {200.0f, 200.0f};
    float circleSize[2] = {150.0f, 150.0f};

    VAO vao;
    genVAO(&vao);

    // pos VBO
    genBufferObject<float>(vao.posVBO, GL_ARRAY_BUFFER, 2 * (noTriangles + 1), circleVertices, GL_STATIC_DRAW);
    setAttPointer<float>(vao.posVBO, 0, 2, GL_FLOAT, 2, 0);

    // size VBO
    genBufferObject<float>(vao.sizeVBO, GL_ARRAY_BUFFER, 1 * 2, nullptr, GL_DYNAMIC_DRAW);
    setAttPointer<float>(vao.sizeVBO, 1, 2, GL_FLOAT, 2, 0, 1);

    // offset VBO
    genBufferObject<float>(vao.offsetVBO, GL_ARRAY_BUFFER, 1 * 2, nullptr, GL_DYNAMIC_DRAW);
    setAttPointer<float>(vao.offsetVBO, 2, 2, GL_FLOAT, 2, 0, 1);

    // EBO
    genBufferObject<GLuint>(vao.EBO, GL_ELEMENT_ARRAY_BUFFER, 3 * noTriangles, circleIndices, GL_STATIC_DRAW);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    unbindBuffer(GL_ARRAY_BUFFER);

    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    unbindVAO();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // update time
        double currentTime = glfwGetTime();
        dt = currentTime - lastFrame;
        lastFrame = currentTime;

        // orthographic projection matrix

        // input
        // -----
        processInput(window);

        //circleOffset[0] += dt;

        //// update circle size and offset
        //updateData(vao.sizeVBO, 0, 1 * 2, circleSize);
        //updateData(vao.offsetVBO, 0, 1 * 2, circleOffset);

        // render
        // ------
        clearScreen();

        // draw our first triangle
        bindShader(shaderProgram);
        draw(vao, GL_TRIANGLES, 3 * noTriangles, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        newFrame(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    cleanup(vao);
    deleteShader(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    cleanup();
    return 0;
}