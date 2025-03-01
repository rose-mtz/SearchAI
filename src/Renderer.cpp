#include "Renderer.h"
#include "Shader.h"

#include <iostream>

const glm::vec3 GRID_LINES_COLOR = glm::vec3(0.1f);

Renderer::Renderer(float grid_width, float grid_height)
{
    // Load shaders
    Shader squareShader;
    Shader lineShader;
    squareShader.compile("./shaders/square.vs", "./shaders/square.fs");
    lineShader.compile("./shaders/line.vs", "./shaders/line.fs");

    // Set projection matrix
    struct ProjectionConfig
    {
        float top = 800.0f;
        float bottom = 0.0f;
        float left = 0.0f;
        float right = 800.0f;
        float near = 1.0f;
        float far = -1.0f;
    } projConfig;
    glm::mat4 projection = glm::ortho(projConfig.left, projConfig.right, projConfig.bottom, projConfig.top, projConfig.near, projConfig.far);
    squareShader.use();
    squareShader.setMat4("projection", projection);
    lineShader.use();
    lineShader.setMat4("projection", projection);

    // init. renderers
    this->squareRenderer.init(squareShader);
    this->lineRenderer.init(lineShader);

    this->GRID_WIDTH = grid_width;
    this->GRID_HEIGHT = grid_height;
}

void Renderer::drawGrid(const Grid& grid)
{
    const float cellWidth = GRID_WIDTH / (float) grid.getNumberOfColumns();
    const float cellHeight = GRID_HEIGHT / (float) grid.getNumberOfRows();

    for (unsigned int row = 0; row < grid.getNumberOfRows(); row++)
    {
        for (unsigned int col = 0; col < grid.getNumberOfColumns(); col++)
        {
            float xTranslation = ((float) col) * cellWidth;
            float yTranslation = ((float) ((grid.getNumberOfRows() - 1) - row)) * cellHeight;

            glm::vec3 color = CELL_COLORS[grid.get(row,col)];

            this->squareRenderer.draw(color, glm::vec2(xTranslation, yTranslation), glm::vec2(cellWidth, cellHeight));
        }
    }
}

void Renderer::drawGridLines(const Grid& grid)
{
    const float cellWidth = GRID_WIDTH / (float) grid.getNumberOfColumns();
    const float cellHeight = GRID_HEIGHT / (float) grid.getNumberOfRows();
    const glm::vec3 color (0.1f, 0.01f, 0.9f);

    // Draw horizontal lines
    for (unsigned int i = 0; i < grid.getNumberOfRows() + 1; i++)
    {
        float y = cellHeight * i;
        glm::vec2 start (0, y);
        glm::vec2 end   (GRID_WIDTH, y);

        this->lineRenderer.draw(GRID_LINES_COLOR, start, end);
    }

    // Draw vertical lines
    for (unsigned int i = 0; i < grid.getNumberOfColumns() + 1; i++)
    {
        float x = cellWidth * i;
        glm::vec2 start (x, 0);
        glm::vec2 end   (x, GRID_HEIGHT);

        this->lineRenderer.draw(GRID_LINES_COLOR, start, end);
    }
}

void Renderer::drawCell(const GridCell& cell, const Grid& grid, const glm::vec3& color)
{
    const float cellWidth = GRID_WIDTH / (float) grid.getNumberOfColumns();
    const float cellHeight = GRID_HEIGHT / (float) grid.getNumberOfRows();
    float xTranslation = ((float) cell.col) * cellWidth;
    float yTranslation = ((float) ((grid.getNumberOfRows() - 1) - cell.row)) * cellHeight;

    this->squareRenderer.draw(color, glm::vec2(xTranslation, yTranslation), glm::vec2(cellWidth, cellHeight));
}