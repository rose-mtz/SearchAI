#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class SquareRenderer
{
private:
    unsigned int VAO;
    Shader shader;

public:
    SquareRenderer();
    void init(Shader shader);

    void draw(const glm::vec3& color, const glm::vec2& translate, const glm::vec2& scale);
};