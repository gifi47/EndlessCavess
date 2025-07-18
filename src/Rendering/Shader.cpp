#include "Shader.h"

#ifdef DEBUG
#include <string>
std::map<long long, std::string> names;
int _____k = 1;
#endif //DEBUG

Rendering::Shader::Shader() {
#ifdef DEBUG
    THIS_NAME = "Shader " + NEW_NAME;
#endif
}

Rendering::Shader::Shader(const std::string& name) {
#ifdef DEBUG
    THIS_NAME = "Shader " + name;
#endif
    Initialize(name + ".vert", name + ".frag");
}

Rendering::Shader::Shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath){
#ifdef DEBUG
    THIS_NAME = "Shader " + vertexShaderFilePath + " " + fragmentShaderFilePath;
#endif
    Initialize(vertexShaderFilePath, fragmentShaderFilePath);
}

Rendering::Shader::Shader(const Shader& shader) {
#ifdef DEBUG
    THIS_NAME = "Shader " + NEW_NAME;
#endif
	this->programId = shader.programId;
}

Rendering::Shader::Shader(Shader&& shader){
#ifdef DEBUG
    THIS_NAME = "Shader " + NEW_NAME;
#endif
    this->programId = shader.programId;
}

Rendering::Shader& Rendering::Shader::operator=(const Shader& other){
    if (this != &other){
        IDisposable::operator=(other);
        this->programId = other.programId;
    }
    return *this;
}
Rendering::Shader& Rendering::Shader::operator=(Shader&& other){
    if (this != &other){
        IDisposable::operator=(std::move(other));
        this->programId = other.programId;
    }
    return *this;
}

void Rendering::Shader::Use() {
    glUseProgram(this->programId);
}

void Rendering::Shader::Initialize(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
    const GLchar* vertexShaderSourceCode{ ReadFile(vertexShaderFilePath) };

    GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderId, 1, &vertexShaderSourceCode, nullptr);
    glCompileShader(vertexShaderId);

    CheckShaderErrors(vertexShaderId);

    const GLchar* fragmentShaderSourceCode{ ReadFile(fragmentShaderFilePath) };

    GLuint fragmentShaderId{ glCreateShader(GL_FRAGMENT_SHADER) };
    glShaderSource(fragmentShaderId, 1, &fragmentShaderSourceCode, nullptr);
    glCompileShader(fragmentShaderId);

    CheckShaderErrors(fragmentShaderId);

    this->programId = glCreateProgram();
    glAttachShader(this->programId, vertexShaderId);
    glAttachShader(this->programId, fragmentShaderId);
    glLinkProgram(this->programId);

    CheckProgramErrors(this->programId);

    glDetachShader(this->programId, vertexShaderId);
    glDetachShader(this->programId, fragmentShaderId);

    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    delete[] vertexShaderSourceCode;
    delete[] fragmentShaderSourceCode;
}

void Rendering::Shader::OnDispose() {
#ifdef DEBUG
        std::cout << "Disposed Shader Program id:" << this->programId << " " << THIS_NAME << "\n";
#endif
    glDeleteProgram(this->programId);
}

void Rendering::Shader::CheckShaderErrors(const GLuint& shaderId) {
    GLint success;
    int infoLogLength;

    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (!success)
    {
        if (infoLogLength < 1) infoLogLength = 1000;
        std::vector<char> infoLog(infoLogLength + 1);
        glGetShaderInfoLog(shaderId, infoLogLength, nullptr, infoLog.data());
        std::cout << "Error: shader compilation error\n" << infoLog.data() << std::endl;
    }
}

void Rendering::Shader::CheckProgramErrors(const GLuint& programId) {
    GLint success;
    int infoLogLength;

    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
    if (!success)
    {
        if (infoLogLength < 1) infoLogLength = 1000;
        std::vector<char> infoLog(infoLogLength + 1);
        glGetProgramInfoLog(programId, infoLogLength, nullptr, infoLog.data());
        std::cout << "Error: shader program compilation error\n" << infoLog.data() << std::endl;
    }
}
