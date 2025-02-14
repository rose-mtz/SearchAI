#include "SearchAI.h"


// /**
//  * Very simple search AI. It just goes straight to the goal.
//  * 
//  * Solution returned backwards in vector.
//  */
// std::vector<GridCell> searchDumb(GridCell start, GridCell end)
// {
//     std::vector<GridCell> solution;

//     GridCell current = start;
//     while (true)
//     {
//         solution.push_back(current);

//         if (current == end) // goal found
//         {
//             return solution;
//         }

//         // Move to next grid cell

//         if (current.col < end.col)
//         {
//             current.col++;
//         }
//         else if (current.col > end.col)
//         {
//             current.col--;
//         }
//         else if (current.row < end.row)
//         {
//             current.row++;
//         }
//         else // current.row > end.row
//         {
//             current.row--;
//         }
//     }
// }

int heuristicManhattan(GridCell start, GridCell end)
{
    int cost = 0;
    cost += (start.row > end.row) ? (start.row - end.row) : (end.row - start.row); 
    cost += (start.col > end.col) ? (start.col - end.col) : (end.col - start.col);

    return cost; 
}

GridCell doAction(GridCell state, Action act)
{
    if (act == UP)
    {
        state.row--; 
    }
    else if (act == DOWN)
    {
        state.row++;
    }
    else if (act == LEFT)
    {
        state.col--;
    }
    else // act == RIGHT
    {
        state.col++;
    }
    return state;
}

Node expandHelper(Node* parent, Action act)
{
    Node child;
    child.state = doAction(parent->state, act);
    child.parent = parent;
    child.depth = parent->depth + 1;
    
    return child;
}

/**
 * Checks for out of bounds or moving from one cell to another cell with a different value.
 */
bool isLegalAction(const Grid* grid, GridCell state, Action act)
{
    GridCell nextState = doAction(state, act);

    if (grid->outOfBounds(nextState))
    {
        return false;
    }

    if (grid->get(state.row, state.col) != grid->get(nextState.row, nextState.col)) // same value/color
    {
        return false;
    }

    return true;
}

// void SearchAI::init(GridCell start, GridCell end, Grid* grid, SearchAIType ai)
// {
//     // Free memory
//     for (Node* n : closed)
//     {
//         delete n;
//     }
//     this->closed.clear();
//     while (!open.isEmpty())
//     {
//         Node* n = open.pop();

//         delete n;
//     }

//     // init.

//     this->start = start;
//     this->end = end;
//     this->grid = grid;
//     this->foundGoal = false;
//     this->goal = nullptr;
//     this->ai = ai;
//     this->CUR_MAX_DEPTH = 0;

//     if (ai == BFS)
//     {
//         this->open = StackOrQueue(false);
//     }
//     else if (ai == DFS || ai == ID_DFS)
//     {
//         this->open = StackOrQueue(true);
//     }

//     // init. state
//     Node* initial = new Node;
//     initial->parent = nullptr;
//     initial->state = start;
//     initial->depth = 0;
//     this->open.push(initial);
// }

// void SearchAI::expand(Node* node, Action act, const std::vector<Node*>& openVec)
// {
//     if (isLegalAction(this->grid, node->state, act))
//     {
//         Node child = expandHelper(node, act);

//         // Check if is in closed list already
//         for (int i = 0; i < this->closed.size(); i++)
//         {
//             if (this->closed[i]->state == child.state) // same state
//             {
//                 if (this->closed[i]->depth <= child.depth)
//                 {
//                     return;
//                 }
//             }
//         }

//         // Check if state already in open
//         for (int i = 0; i < openVec.size(); i++)
//         {
//             if (openVec[i]->state == child.state)
//             {
//                 return;
//             }
//         }

//         // Add to open.
//         Node * toAdd = new Node();
//         toAdd->depth = child.depth;
//         toAdd->parent = child.parent;
//         toAdd->state = child.state;

//         this->open.push(toAdd);
//     }
// }

// void SearchAI::step()
// {
//     if (this->done()) // Found solution or no possible solution
//     {
//         return;
//     }

//     if (this->open.isEmpty()) // increase depth for ID-DFS
//     {
//         unsigned int temp = this->CUR_MAX_DEPTH;
//         this->init(this->start, this->end, this->grid, ID_DFS); // reset
//         this->CUR_MAX_DEPTH = temp + 1;
//     }

//     Node * current = this->open.pop();
//     closed.push_back(current);

//     if (this->end == current->state) // at goal
//     {
//         this->foundGoal = true;
//         this->goal = current;
//         return;
//     }

//     if (this->ai == ID_DFS && current->depth == CUR_MAX_DEPTH) // at max depth
//     {
//         return;
//     }

//     // Expand node

//     const std::vector<Node*> openList = this->open.getNodes();
//     this->expand(current, UP, openList);
//     this->expand(current, DOWN, openList);
//     this->expand(current, LEFT, openList);
//     this->expand(current, RIGHT, openList);

//     std::cout << "Open size: " << this->open.size() << '\n';
//     std::cout << "Closed size: " << this->closed.size() << '\n';
// }

// std::vector<GridCell> SearchAI::getOpen()
// {
//     return this->open.getGridCells();
// }

// std::vector<GridCell> SearchAI::getClosed()
// {
//     std::vector<GridCell> toReturn;

//     for (Node* n : this->closed)
//     {
//         toReturn.push_back(n->state);
//     }

//     return toReturn;
// }

// std::vector<GridCell> SearchAI::getSolution()
// {
//     std::vector<GridCell> sol;

//     if (!foundGoal) // no solution 
//     {
//         return sol;
//     }

//     Node* cur = this->goal;
//     while (cur != nullptr)
//     {
//         sol.push_back(cur->state);
//         cur = cur->parent;
//     }

//     return sol;
// }

// // true if found goal, else false
// bool SearchAI::done()
// {
//     if (this->ai == ID_DFS)
//     {
//         return foundGoal || (this->CUR_MAX_DEPTH == this->MAX_DEPTH && this->open.isEmpty());
//     }
//     else
//     {
//         return foundGoal || this->open.isEmpty();
//     }
// }

