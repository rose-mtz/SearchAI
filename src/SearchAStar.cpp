#include "SearchAStar.h"


SearchAStar::SearchAStar(const Grid* grid, GridCell start, GridCell end)
    : grid(grid), start(start), end(end)
{
    this->init();
}

SearchAStar::~SearchAStar()
{
}

void SearchAStar::step()
{
    if (this->done())
    {
        return;
    }

    int indexOfLowestCostNode = 0;
    for (int i = 1; i < this->open.size(); i++)
    {
        if (this->open[i]->heuristicCost + this->open[i]->costToInitial  < this->open[indexOfLowestCostNode]->heuristicCost + this->open[indexOfLowestCostNode]->costToInitial)
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

void SearchAStar::finish()
{
}

bool SearchAStar::done()
{
    return this->foundGoal || this->open.size() == 0;
}

const std::vector<Node*> SearchAStar::getOpen()
{
    std::vector<Node*> nodeVec(this->open.size());
    for (int i = 0; i < open.size(); i++)
    {
        nodeVec[i] = (Node*) this->open[i];
    }

    return nodeVec;
}

const std::vector<Node*> SearchAStar::getClosed()
{
    std::vector<Node*> nodeVec(this->closed.size());
    for (int i = 0; i < closed.size(); i++)
    {
        nodeVec[i] = (Node*) this->closed[i];
    }

    return nodeVec;
}

const std::vector<Node*> SearchAStar::getSolution()
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

void SearchAStar::init()
{
    Node* initial = new Node;
    initial->parent = nullptr;
    initial->state = this->start;
    initial->heuristicCost = heuristicManhattan(initial->state, this->end);
    initial->costToInitial = 0;
    this->open.push_back(initial);
}

void SearchAStar::expand(Node* node, Action act)
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
    child->costToInitial = node->costToInitial + 1; // action cost is 1

    this->open.push_back(child);
}