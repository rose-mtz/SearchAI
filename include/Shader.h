#ifndef SHADER
#define SHADER

#include <glm/glm.hpp>

class Shader
{
public:
    Shader();
    
    void compile(const char* vsPath, const char* fsPath);
    void use();
    void setVector(const char* name, float x, float y, float z);
    void setVector(const char* name, const glm::vec3& vector);
    void setMat4(const char* name, const glm::mat4& matrix);

private:
    unsigned int ID;
};

#endif