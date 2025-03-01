#include "SquareRenderer.h"
#include "LineRenderer.h"
#include "Grid.h"
#include <glm/glm.hpp>

const unsigned int COUNT_CELL_COLORS = 4;
const glm::vec3 CELL_COLORS[] = 
{
    glm::vec3(0.2f, 0.329f, 1.0f), // 0
    glm::vec3(0.259f, 0.988f, 0.008f),  // 1 
    glm::vec3(0.467f, 0.467f, 0.467f), // 2
    glm::vec3(0.592, 0.349, 0.251),
};

class Renderer
{
public:
    Renderer(float grid_width, float grid_height);

    SquareRenderer squareRenderer;
    LineRenderer lineRenderer;
    float GRID_WIDTH;
    float GRID_HEIGHT;

    void drawGrid(const Grid& grid);
    void drawGridLines(const Grid& grid);
    void drawCell(const GridCell& cell, const Grid& grid, const glm::vec3& color);

private:
};