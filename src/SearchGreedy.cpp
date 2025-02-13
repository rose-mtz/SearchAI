#include "SearchGreedy.h"


int heuristicManhattan(GridCell start, GridCell end)
{
    int cost = 0;
    cost += (start.row > end.row) ? (start.row - end.row) : (end.row - start.row); 
    cost += (start.col > end.col) ? (start.col - end.col) : (end.col - start.col);

    return cost; 
}

SearchGreedy::SearchGreedy(const Grid* grid, GridCell start, GridCell end)
    : grid(grid), start(start), end(end)
{
    this->init();
}

SearchGreedy::~SearchGreedy()
{
}

void SearchGreedy::step()
{
    if (this->done())
    {
        return;
    }

    int indexOfLowestCostNode = 0;
    for (int i = 1; i < this->open.size(); i++)
    {
        if (this->open[i]->heuristicCost < this->open[indexOfLowestCostNode]->heuristicCost)
        {
            indexOfLowestCostNode = i;
        }
    }

    Node* toExplore = this->open[indexOfLowestCostNode];
    if (this->open.size() > 1)
    {
        this->open[indexOfLowestCostNode] = this->open[this->open.size() - 1];
    }
    this->open.pop_back();
    this->closed.push_back(toExplore);

    if (toExplore->state == this->end)
    {
        this->foundGoal = true;
        this->goal = toExplore;
        return;
    }

    this->expand(toExplore, UP);
    this->expand(toExplore, RIGHT);
    this->expand(toExplore, DOWN);
    this->expand(toExplore, LEFT);
}

void SearchGreedy::finish()
{
}

bool SearchGreedy::done()
{
    return this->foundGoal || this->open.size() == 0;
}

const std::vector<Node*> SearchGreedy::getOpen()
{
    std::vector<Node*> nodeVec(this->open.size());
    for (int i = 0; i < open.size(); i++)
    {
        nodeVec[i] = (Node*) this->open[i];
    }

    return nodeVec;
}

const std::vector<Node*> SearchGreedy::getClosed()
{
    std::vector<Node*> nodeVec(this->closed.size());
    for (int i = 0; i < closed.size(); i++)
    {
        nodeVec[i] = (Node*) this->closed[i];
    }

    return nodeVec;
}

const std::vector<Node*> SearchGreedy::getSolution()
{
    std::vector<Node*> solution;

    Node* current = this->goal;
    while (current != nullptr)
    {
        solution.push_back(current);
        current = current->parent;
    }

    return solution;
}

void SearchGreedy::init()
{
    Node* initial = new Node;
    initial->parent = nullptr;
    initial->state = this->start;
    initial->heuristicCost = heuristicManhattan(initial->state, this->end);
    this->open.push_back(initial);
}

void SearchGreedy::expand(Node* node, Action act)
{
    bool isIllegalAction = !isLegalAction(this->grid, node->state, act);
    if (isIllegalAction)
    {
        return;
    }

    GridCell childState = doAction(node->state, act);

    for (int i = 0; i < this->open.size(); i++)
    {
        bool foundDuplicateState = childState == this->open[i]->state;
        if (foundDuplicateState)
        {
            return;
        }
    }

    for (int i = 0; i < this->closed.size(); i++)
    {
        bool foundDuplicateState = childState == this->closed[i]->state;
        if (foundDuplicateState)
        {
            return;
        }
    }

    Node* child = new Node;
    child->state = childState;
    child->parent = node;
    child->heuristicCost = heuristicManhattan(childState, this->end);

    this->open.push_back(child);
}