#include <fstream>
#include <sstream>
#include "ShaderProgram.h"
#include "glm/gtc/type_ptr.hpp"


unsigned int ShaderProgram::createShader(const std::string shaderPath,  GLuint shaderType, 
        const std::string typeName){

    const std::string shaderString = readShaderFile(shaderPath);
    const char* shaderCode = shaderString.c_str(); 
    unsigned int shaderID { glCreateShader(shaderType) };
    glShaderSource(shaderID, 1, &shaderCode, NULL);
    glCompileShader(shaderID);
    checkLinkStatus(shaderID, "SHADER::" + typeName);
    return shaderID;
}

ShaderProgram::ShaderProgram(const std::string vertexPath,const std::string fragmentPath){

    // To use the compiled shader we have to link them to a shader program object
    // The ative shader program's shader will be used when we issue render calls.
    // This is also where you'll get linking errors if your outputs and inputs don't match
    m_ID = glCreateProgram();

    unsigned int vShaderID = createShader(vertexPath, GL_VERTEX_SHADER, "VERTEX");
    unsigned int fShaderID = createShader(fragmentPath, GL_FRAGMENT_SHADER, "FRAGMENT");
    // not attach the compiled shaders to the program
    glAttachShader(m_ID, vShaderID);
    glAttachShader(m_ID, fShaderID);
    // This links all the attached shaders in one final shader m_ID object
    glLinkProgram(m_ID);

    checkLinkStatus(m_ID, "SHADERPROGRAM");
    glDeleteShader(vShaderID);
    glDeleteShader(fShaderID);
}

ShaderProgram::ShaderProgram(const std::string vertexPath, const std::string geoPath, const std::string fragmentPath){

    // To use the compiled shader we have to link them to a shader program object
    // The ative shader program's shader will be used when we issue render calls.
    // This is also where you'll get linking errors if your outputs and inputs don't match
    m_ID = glCreateProgram();

    unsigned int vShaderID = createShader(vertexPath, GL_VERTEX_SHADER, "VERTEX");
    unsigned int gShaderID = createShader(geoPath, GL_GEOMETRY_SHADER, "GEOMETRY");
    unsigned int fShaderID = createShader(fragmentPath, GL_FRAGMENT_SHADER, "FRAGMENT");
    // not attach the compiled shaders to the program
    glAttachShader(m_ID, vShaderID);
    glAttachShader(m_ID, gShaderID);
    glAttachShader(m_ID, fShaderID);
    // This links all the attached shaders in one final shader m_ID object
    glLinkProgram(m_ID);

    checkLinkStatus(m_ID, "SHADERPROGRAM");
    glDeleteShader(vShaderID);
    glDeleteShader(gShaderID);
    glDeleteShader(fShaderID);
    
}

const std::string ShaderProgram::readShaderFile(const std::string shaderFilePath) const{
    // First retriev the vertex/fragment source code from filePath
    std::string code;
    std::ifstream shaderFile;

    // ensure ifstream object can throw exceptions
    // more here https://www.cplusplus.com/reference/ios/ios/exceptions/
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    
    // guard for exceptions during file reading
    try{
        // open files
        shaderFile.open(shaderFilePath);

        // these are for streaming strings in memory
        std::stringstream shaderStream;

        // stream files into stringstreams
        shaderStream << shaderFile.rdbuf();

        // close the files
        shaderFile.close();
        
        // store in string
        code = shaderStream.str();

    }
    catch(std::ifstream::failure &e){
        std::cout << "ERROR::SHADER::FILE_COULD_NOT_BE_READ" << '\n';
    }
    return code; 
}

bool ShaderProgram::checkLinkStatus(const unsigned int ID, const std::string errorType="") const{ 
    // let's check if compilation was successful.
    int success;
    // this is the extact length of the shader info log
    char infoLog[1028];
    // This allows the developer to query a shader for information given an object 
    // parameter. 
    // You can also use GL_SHADER_TYPE, GL_DELETE_STATUS, GL_INFO_LOG_LENGTH, GL_SHADER_SOURCE_LENGTH/
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(ID, 1028, NULL, infoLog);
        std::cout << "ERROR::" << errorType << "::LINKING_FAILED\n" << infoLog << '\n';
    }
    return success;
}

void ShaderProgram::use() const{
    glUseProgram(m_ID);
}

void ShaderProgram::setBool(const std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string &name, float value) const{
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void ShaderProgram::setVec3(const std::string &name, const glm::vec3 &v) const{
    glUniform3f(glGetUniformLocation(m_ID, name.c_str()), v.x, v.y, v.z);
}

void ShaderProgram::setMatrix(const std::string &name, const glm::mat4 &xform)const {
    //The first argument is the name as usual
    // the second is how many matricies to send
    // the third is if we want to transpose the matrix - this means using column-major ordering
    // the fourth is the matrix
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(xform));
}

void ShaderProgram::setMatrix3(const std::string &name, const glm::mat3 &xform) const {
    glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(xform)); 
}

ShaderProgram::~ShaderProgram(){
    glDeleteProgram(m_ID);
}
