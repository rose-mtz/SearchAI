#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class LineRenderer
{
private:
    unsigned int VAO;
    Shader shader;

public:
    LineRenderer();
    void init(Shader shader);

    void draw(const glm::vec3& color, const glm::vec2& start, const glm::vec2& end);
};