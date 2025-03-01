#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

#include "GridRawData.h"
#include "SearchAI.h"
#include "SearchBFS.h"
#include "SearchDFS.h"
#include "SearchGreedy.h"
#include "SearchAStar.h"
#include "Renderer.h"


// Globals ------------------------------------------------------


// Colors
const glm::vec3 SOLUTION_COLOR = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 CLOSED_CELL = glm::vec3(0.725f, 0.024f, 0.035f);
const glm::vec3 OPEN_CELL = glm::vec3(0.976f, 0.792f, 0.012f);
const glm::vec3 MOUSE_COLOR = glm::vec3(0.0f, 0.75f, 1.0f);

glm::vec2 mouse_pos;
glm::vec2 path_start;
glm::vec2 path_end;
unsigned int selected_path_endpoints = 0;
bool reinitialize_ai = true;
SearchAI* path_finder_ai;
SearchAIType current_ai_type = (SearchAIType) 0;

float current_frame = 0.0f;
float last_animation = 0.0f;
const float ANIMATION_INTERVAL = 0.2f; // seconds
bool animate = false;

Grid grid;

bool map_creation_mode = false;
unsigned int selected_cell_value = 0;
const unsigned int COUNT_GRID_DENSITIES = 5;
const unsigned int GRID_DENSITIES[] = { 16, 32, 64, 128, 256 };
unsigned int selected_grid_density = 0;
unsigned int selected_grid_pre_made_map = 0;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;


// Function declarations -------------------------------------------------------------------------------------


bool initialize(GLFWwindow* &window, unsigned int width, unsigned int height);
void processInput(GLFWwindow *window);
void loadGrid(Grid& grid, const char* rawData);
GridCell getCellThatMouseIsOn(Grid& grid,const glm::vec2& mousePos, float gridWidth, float gridHeight);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void printGrid();
void updateAI();
void render(Renderer& renderer);


// Main ----------------------------------------------------------------------------------------------------------


int main()
{
    // Window
    GLFWwindow* window = nullptr;
    if (!initialize(window, SCR_WIDTH, SCR_HEIGHT))
    {
        return -1;
    }

    // Events
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetKeyCallback(window, key_callback);

    // Renderer
    Renderer renderer(SCR_WIDTH, SCR_HEIGHT);

    // Grid
    initializeGridData();
    grid.changeGrid(GRID_RAW_DATA[0]->rows, GRID_RAW_DATA[0]->cols);
    loadGrid(grid, GRID_RAW_DATA[0]->rawData);


    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        current_frame = glfwGetTime();

        // Input
        processInput(window);
        glfwPollEvents();

        // Update state
        updateAI();

        // Render
        render(renderer);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}


// Helper Functions -------------------------------------------------------------------------------


void updateAI()
{
    if (!map_creation_mode)
    {
        bool has_selected_start_and_end = selected_path_endpoints == 2;
        bool path_finder_not_done = (path_finder_ai != nullptr && !path_finder_ai->done());
        bool animate_next_step = animate && current_frame + ANIMATION_INTERVAL >= last_animation;

        // Animate path
        if (has_selected_start_and_end && path_finder_not_done && animate_next_step)
        {
            path_finder_ai->step();
            last_animation = current_frame;
        }

        // Restart AI
        if (reinitialize_ai && has_selected_start_and_end)
        {
            GridCell start = getCellThatMouseIsOn(grid, path_start, SCR_WIDTH, SCR_HEIGHT);
            GridCell end = getCellThatMouseIsOn(grid, path_end, SCR_WIDTH, SCR_HEIGHT);

            if (current_ai_type == ID_DFS)
            {
                path_finder_ai = new SearchDFS(&grid, start, end);
            }
            else if (current_ai_type == BFS)
            {
                path_finder_ai = new SearchBFS(&grid, start, end);
            }
            else if (current_ai_type == A_STAR)
            {
                path_finder_ai = new SearchAStar(&grid, start, end);
            }
            else if (current_ai_type == GREEDY)
            {
                path_finder_ai = new SearchGreedy(&grid, start, end);
            }

            reinitialize_ai = false;
        }
    }
}


void render(Renderer& renderer)
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (!map_creation_mode)
    {
        renderer.drawGrid(grid);

        if (selected_path_endpoints == 2 && path_finder_ai != nullptr)
        {
            std::vector<Node *> closedList = path_finder_ai->getClosed();
            for (const Node* aNode : closedList)
            {
                renderer.drawCell(aNode->state, grid, CLOSED_CELL);
            }
            std::vector<Node*> openList = path_finder_ai->getOpen();
            for (Node* aNode : openList)
            {
                renderer.drawCell(aNode->state, grid, OPEN_CELL);
            }
            std::vector<Node*> solution = path_finder_ai->getSolution();
            for (Node* aNode : solution)
            {
                renderer.drawCell(aNode->state, grid, SOLUTION_COLOR);
            }
        }

        GridCell mouseCell = getCellThatMouseIsOn(grid, mouse_pos, SCR_WIDTH, SCR_HEIGHT);
        renderer.drawCell(mouseCell, grid, MOUSE_COLOR);

        GridCell pathStartCell = getCellThatMouseIsOn(grid, path_start, SCR_WIDTH, SCR_HEIGHT);
        GridCell pathEndCell = getCellThatMouseIsOn(grid, path_end, SCR_WIDTH, SCR_HEIGHT);
        if (selected_path_endpoints == 1)
        {
            renderer.drawCell(pathStartCell, grid, SOLUTION_COLOR);
        }
        else if (selected_path_endpoints == 2)
        {
            renderer.drawCell(pathStartCell, grid, SOLUTION_COLOR);
            renderer.drawCell(pathEndCell, grid, SOLUTION_COLOR);
        }

        renderer.drawGridLines(grid);
    }
    else
    {
        renderer.drawGrid(grid);
        GridCell mouseCell = getCellThatMouseIsOn(grid, mouse_pos, SCR_WIDTH, SCR_HEIGHT);
        renderer.drawCell(mouseCell, grid, CELL_COLORS[selected_cell_value]);
        renderer.drawGridLines(grid);
    }
}

/**
 * Initializes window and GLAD.
 * 
 * Returns false if something went wrong, and logs error.
 */
bool initialize(GLFWwindow* &window, unsigned int width, unsigned int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "A1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }    
    glViewport(0, 0, width, height);

    return true;
}


void loadGrid(Grid& grid, const char* rawData)
{
    unsigned int i = 0;
    for (unsigned int row = 0; row < grid.getNumberOfRows(); row++)
    {
        for (unsigned int col = 0; col < grid.getNumberOfColumns(); col++)
        {
            unsigned int value = rawData[i] - '0';
            grid.set(row, col, value);

            i += 2;
        }
    }
}


GridCell getCellThatMouseIsOn(Grid& grid,const glm::vec2& mousePos, float gridWidth, float gridHeight)
{
    const float cellWidth = gridWidth / (float) grid.getNumberOfColumns();
    const float cellHeight = gridHeight / (float) grid.getNumberOfRows();

    GridCell cell;
    cell.row = std::floor(mousePos.y / cellHeight);
    cell.col = std::floor(mousePos.x / cellWidth);

    return cell;
}


// Helper Functions: input/output ---------------------------------------------------------------------


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouse_pos.x = xpos;
    mouse_pos.y = ypos;

    if (map_creation_mode && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        const glm::vec2 mousePOS (xpos, ypos); // what a POS mouse!
        GridCell clickedCell = getCellThatMouseIsOn(grid, mousePOS, SCR_WIDTH, SCR_HEIGHT);

        if (
            clickedCell.row >= 0 && 
            clickedCell.row < grid.getNumberOfRows() &&
            clickedCell.col >= 0 &&
            clickedCell.col < grid.getNumberOfColumns()
        )
        {
            grid.set(clickedCell.row, clickedCell.col, selected_cell_value);
        }
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (!map_creation_mode)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            if (selected_path_endpoints == 0 || selected_path_endpoints == 2)
            {
                path_start = glm::vec2(xpos, ypos);
                selected_path_endpoints = 1;
            }
            else 
            {
                path_end = glm::vec2(xpos,ypos);
                selected_path_endpoints = 2;
                reinitialize_ai = true;
            }
        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        {
            if (animate)
            {
                float END_TIME = glfwGetTime() + 5.0f;
                while (!path_finder_ai->done() && END_TIME >= glfwGetTime())
                {
                    path_finder_ai->step();
                }
            }
            else
            {
                path_finder_ai->step();
            }
        }
    }
    else
    {
        const glm::vec2 mousePOS (xpos, ypos); // what a POS mouse!
        GridCell clickedCell = getCellThatMouseIsOn(grid, mousePOS, SCR_WIDTH, SCR_HEIGHT);
        if (
            clickedCell.row >= 0 && 
            clickedCell.row < grid.getNumberOfRows() &&
            clickedCell.col >= 0 &&
            clickedCell.col < grid.getNumberOfColumns()
        )
        {
            grid.set(clickedCell.row, clickedCell.col, selected_cell_value);
        }
    }
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_M && action == GLFW_PRESS)
    {
        map_creation_mode = !map_creation_mode;
        if (map_creation_mode)
        {
            selected_path_endpoints = 0;
        }
        std::cout << "Map creation mode: " << (map_creation_mode ? "ON" : "OFF") << '\n';
    }

    if (!map_creation_mode)
    {
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        {
            if (path_finder_ai->done() && selected_path_endpoints == 2)
            {
                reinitialize_ai = true;
                animate = true;
            }
            else
            {
                animate = !animate;
            }
        }
        if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
        {
            current_ai_type = (SearchAIType) (current_ai_type + 1);
            if (current_ai_type == LAST)
            {
                current_ai_type = (SearchAIType) 0;
            }
            std::cout << "AI Type: " << current_ai_type << '\n';

            reinitialize_ai = true;
        }
    }
    else
    {
        if (key == GLFW_KEY_N && action == GLFW_PRESS)
        {
            selected_cell_value++;
            
            if (selected_cell_value >= COUNT_CELL_COLORS)
            {
                selected_cell_value = 0;
            }
        }
        if (key == GLFW_KEY_UP && action == GLFW_PRESS)
        {
            selected_grid_density++;

            if (selected_grid_density >= COUNT_GRID_DENSITIES)
            {
                selected_grid_density = COUNT_GRID_DENSITIES - 1;
            }

            grid.changeGrid(GRID_DENSITIES[selected_grid_density], GRID_DENSITIES[selected_grid_density]);
        }
        if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
        {
            if (selected_grid_density > 0)
            {
                selected_grid_density--;
            }

            grid.changeGrid(GRID_DENSITIES[selected_grid_density], GRID_DENSITIES[selected_grid_density]);
        }
        if (key == GLFW_KEY_P && action == GLFW_PRESS)
        {
            printGrid();
        }
        if (key == GLFW_KEY_C && action == GLFW_PRESS)
        {
            grid.clear(selected_cell_value);
        }
        if (key == GLFW_KEY_B && action == GLFW_PRESS)
        {
            selected_grid_pre_made_map++;
            if (selected_grid_pre_made_map >= GRID_RAW_DATA_SIZE)
            {
                selected_grid_pre_made_map = 0;
            }

            grid.changeGrid(GRID_RAW_DATA[selected_grid_pre_made_map]->rows, GRID_RAW_DATA[selected_grid_pre_made_map]->cols);
            loadGrid(grid, GRID_RAW_DATA[selected_grid_pre_made_map]->rawData);
        }
    }
}


void printGrid()
{
    std::cout << "Rows: " << grid.getNumberOfRows() << '\n';
    std::cout << "Cols: " << grid.getNumberOfColumns() << '\n';

    for (unsigned int row = 0; row < grid.getNumberOfRows(); row++)
    {
        for (unsigned int col = 0; col < grid.getNumberOfColumns(); col++)
        {
            std::cout << grid.get(row, col) << ' ';
        }
        std::cout << '\n';
    }
}