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
unsigned int selected_path_endpoints = 0; // [0,2]
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
void loadGrid(Grid& grid, const char* raw_data);
GridCell getCell(Grid& grid,const glm::vec2& pos, float grid_width, float grid_height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
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

    // Events callbacks
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
            GridCell start = getCell(grid, path_start, SCR_WIDTH, SCR_HEIGHT);
            GridCell end = getCell(grid, path_end, SCR_WIDTH, SCR_HEIGHT);

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

    if (!map_creation_mode) // Mode: search
    {
        // Map
        renderer.drawGrid(grid);

        if (selected_path_endpoints == 2 && path_finder_ai != nullptr)
        {
            // Closed list
            std::vector<Node *> closedList = path_finder_ai->getClosed();
            for (Node* aNode : closedList)
            {
                renderer.drawCell(aNode->state, grid, CLOSED_CELL);
            }

            // Open list
            std::vector<Node*> openList = path_finder_ai->getOpen();
            for (Node* aNode : openList)
            {
                renderer.drawCell(aNode->state, grid, OPEN_CELL);
            }

            // Solution path
            std::vector<Node*> solution = path_finder_ai->getSolution();
            for (Node* aNode : solution)
            {
                renderer.drawCell(aNode->state, grid, SOLUTION_COLOR);
            }
        }

        // Mouse
        GridCell mouseCell = getCell(grid, mouse_pos, SCR_WIDTH, SCR_HEIGHT);
        renderer.drawCell(mouseCell, grid, MOUSE_COLOR);

        // Start and end grid cells
        if (selected_path_endpoints > 0)
        {
            GridCell pathStartCell = getCell(grid, path_start, SCR_WIDTH, SCR_HEIGHT);
            renderer.drawCell(pathStartCell, grid, SOLUTION_COLOR);
        }
        if (selected_path_endpoints == 2)
        {
            GridCell pathEndCell = getCell(grid, path_end, SCR_WIDTH, SCR_HEIGHT);
            renderer.drawCell(pathEndCell, grid, SOLUTION_COLOR);
        }

        // Grid lines
        renderer.drawGridLines(grid);
    }
    else // Mode: map creation
    {
        // Map
        renderer.drawGrid(grid);

        // Mouse
        GridCell mouseCell = getCell(grid, mouse_pos, SCR_WIDTH, SCR_HEIGHT);
        renderer.drawCell(mouseCell, grid, CELL_COLORS[selected_cell_value]);

        // Grid lines
        renderer.drawGridLines(grid);
    }
}


// Initializes window and GLAD.
// Returns false if something went wrong, and logs error.
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


// Loads grid with data.
void loadGrid(Grid& grid, const char* raw_data)
{
    unsigned int i = 0;
    for (unsigned int row = 0; row < grid.getNumberOfRows(); row++)
    {
        for (unsigned int col = 0; col < grid.getNumberOfColumns(); col++)
        {
            grid.set(row, col, raw_data[i] - '0');
            i += 2;
        }
    }
}


// (x,y) --> GridCell(i,j)
GridCell getCell(Grid& grid, const glm::vec2& pos, float grid_width, float grid_height)
{
    const float cell_width = grid_width / (float) grid.getNumberOfColumns();
    const float cell_height = grid_height / (float) grid.getNumberOfRows();

    GridCell cell;
    cell.row = std::floor(pos.y / cell_height);
    cell.col = std::floor(pos.x / cell_width);

    return cell;
}


// Helper Functions: input/output ---------------------------------------------------------------------


void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    // Update mouse pos
    mouse_pos.x = xpos;
    mouse_pos.y = ypos;

    bool left_click = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if (map_creation_mode && left_click) // Mode: map creation
    {
        GridCell clicked_cell = getCell(grid, mouse_pos, SCR_WIDTH, SCR_HEIGHT);

        if (!grid.outOfBounds(clicked_cell))
        {
            // Update cell value
            grid.set(clicked_cell.row, clicked_cell.col, selected_cell_value);
        }
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (!map_creation_mode) // Mode: search
    {
        bool left_click = button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS;
        if (left_click)
        {
            // User is selecting path endpoints
            if (selected_path_endpoints == 0 || selected_path_endpoints == 2)
            {
                path_start = mouse_pos;
                selected_path_endpoints = 1;
            }
            else 
            {
                path_end = mouse_pos;
                selected_path_endpoints = 2;
                reinitialize_ai = true;
            }

            return;
        }

        bool right_click = button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS;
        if (right_click)
        {
            if (animate) // finish / run search for extened period of time
            {
                // BUG: this causes program to freeze up while running this loop!
                // Maybe just switch to taking like 10-100 steps instead.

                float END_TIME = glfwGetTime() + 2.0f;
                while (!path_finder_ai->done() && END_TIME >= glfwGetTime())
                {
                    path_finder_ai->step();
                }
            }
            else // take 1 step in search
            {
                path_finder_ai->step();
            }

            return;
        }
    }
    else // Mode: map creation
    {
        GridCell clicked_cell = getCell(grid, mouse_pos, SCR_WIDTH, SCR_HEIGHT);

        if (!grid.outOfBounds(clicked_cell))
        {
            // Update cell value
            grid.set(clicked_cell.row, clicked_cell.col, selected_cell_value);
        }
    }
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    bool m_press = key == GLFW_KEY_M && action == GLFW_PRESS;

    if (m_press) // Change mode
    {
        map_creation_mode = !map_creation_mode;
        std::cout << "Map creation mode: " << (map_creation_mode ? "ON" : "OFF") << '\n';

        // Clear selected endpoints
        if (map_creation_mode)
        {
            selected_path_endpoints = 0;
        }

        return;
    }

    if (!map_creation_mode) // Mode: search
    {
        bool space_press = key == GLFW_KEY_SPACE && action == GLFW_PRESS;

        if (space_press)
        {
            bool restart_search = path_finder_ai->done() && selected_path_endpoints == 2;

            if (restart_search)
            {
                // Search is done, restart it
                reinitialize_ai = true;
                animate = true;
            }
            else 
            {
                // Toggle animation
                animate = !animate;
            }

            return;
        }

        bool enter_press = key == GLFW_KEY_ENTER && action == GLFW_PRESS;

        if (enter_press)
        {
            // Change search ai type
            current_ai_type = (SearchAIType) (current_ai_type + 1);
            if (current_ai_type == LAST)
            {
                current_ai_type = (SearchAIType) 0;
            }
            std::cout << "AI Type: " << current_ai_type << '\n';

            reinitialize_ai = true;

            return;
        }
    }
    else // Mode: map creation
    {
        bool n_press = key == GLFW_KEY_N && action == GLFW_PRESS;

        if (n_press)
        {
            // Update "draw" color
            selected_cell_value++;
            if (selected_cell_value >= COUNT_CELL_COLORS)
            {
                selected_cell_value = 0;
            }

            return;
        }

        bool up_press = key == GLFW_KEY_UP && action == GLFW_PRESS;

        if (up_press)
        {
            if (selected_grid_density != COUNT_GRID_DENSITIES - 1)
            {
                // Increase grid density
                selected_grid_density++;
                grid.changeGrid(GRID_DENSITIES[selected_grid_density], GRID_DENSITIES[selected_grid_density]);
            }

            return;
        }

        bool down_press = key == GLFW_KEY_DOWN && action == GLFW_PRESS; 

        if (down_press)
        {
            if (selected_grid_density != 0)
            {
                // Decrease grid density
                selected_grid_density--;
                grid.changeGrid(GRID_DENSITIES[selected_grid_density], GRID_DENSITIES[selected_grid_density]);
            }

            return;
        }

        bool p_press = key == GLFW_KEY_P && action == GLFW_PRESS;

        if (p_press)
        {
            // Print grid
            grid.print();
            return;
        }

        bool c_press = key == GLFW_KEY_C && action == GLFW_PRESS;

        if (c_press)
        {
            // Clear grid
            grid.clear(selected_cell_value);
            return;
        }

        bool b_press = key == GLFW_KEY_B && action == GLFW_PRESS;

        if (b_press)
        {
            // Change selected pre made map
            selected_grid_pre_made_map++;
            if (selected_grid_pre_made_map == GRID_RAW_DATA_SIZE)
            {
                selected_grid_pre_made_map = 0;
            }

            grid.changeGrid(GRID_RAW_DATA[selected_grid_pre_made_map]->rows, GRID_RAW_DATA[selected_grid_pre_made_map]->cols);
            loadGrid(grid, GRID_RAW_DATA[selected_grid_pre_made_map]->rawData);
            return;
        }
    }
}