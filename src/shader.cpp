#include <shader.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath) 
{
    std::cout << "INFO: Constructing shader object..." << std::endl;

    // 0. Retrieve the vertex and fragment shader source code from the specified paths.
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // 0.5. Ensure created ifstream objects are able to throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        // 1. Open ifstream objects with specified paths.
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        // 1.5. Read the file's buffer contents into newly created streams.
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // 2. After reading, close ifstream file handler objects.
        vShaderFile.close();
        fShaderFile.close();

        // 2.5. Convert streams into strings.
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::ostringstream msg;
        msg << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what();
        throw std::runtime_error(msg.str());
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 3. We can begin compiling the shaders:
    GLuint vertexShader, fragmentShader;
    int success;
    char infoLog[512];

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 4. After linking, delete the shaders.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec3f(const std::string &name, float v0, float v1, float v2) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2);
}

void Shader::setVec4f(const std::string &name, float v0, float v1, float v2, float v3) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), v0, v1, v2, v3);
}

bool Shader::getBool(const std::string &name) const {
    int val;
    glGetUniformiv(ID, glGetUniformLocation(ID, name.c_str()), &val);
    return (val == 0) ? false : true;
}

int Shader::getInt(const std::string &name) const {
    int val;
    glGetUniformiv(ID, glGetUniformLocation(ID, name.c_str()), &val);
    return val;
}

float Shader::getFloat(const std::string &name) const {
    float val;
    glGetUniformfv(ID, glGetUniformLocation(ID, name.c_str()), &val);
    return val;
}