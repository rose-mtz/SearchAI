#ifndef SEARCH_AI_H
#define SEARCH_AI_H

#include <vector>
#include <queue>
#include <iostream>
#include <stack>
#include <limits>

#include "Grid.h"

struct Node
{
    GridCell state;
    Node* parent;
    unsigned int depth;
    bool cutoff = false;
    int heuristicCost = 0;
    int costToInitial = 0;
};

enum Action
{
    // up is -y, right is +x
    UP, DOWN, LEFT, RIGHT
};

enum SearchAIType
{
    ID_DFS, BFS, A_STAR, GREEDY, LAST
};

std::vector<GridCell> searchDumb(GridCell start, GridCell end);


class SearchAI
{
// private:
//     std::vector<Node*> closed;
//     StackOrQueue open;
//     GridCell start;
//     GridCell end;
//     Grid* grid;
//     Node* goal = nullptr;
//     bool foundGoal = false;
//     SearchAIType ai;
//     const unsigned int MAX_DEPTH = std::numeric_limits<unsigned int>::max();
//     unsigned int CUR_MAX_DEPTH = 0;
//     void expand(Node* node, Action act, const std::vector<Node*>& openVec);

// public:

//     SearchAI() {}

//     void init(GridCell start, GridCell end, Grid* grid, SearchAIType ai);
//     void step();

//     std::vector<GridCell> getOpen();
//     std::vector<GridCell> getClosed();
//     std::vector<GridCell> getSolution();
//     bool done();
public:
    virtual bool done() = 0;
    virtual void step() = 0;
    virtual void finish() = 0;
    virtual const std::vector<Node*> getOpen() = 0;
    virtual const std::vector<Node*> getClosed() = 0;
    virtual const std::vector<Node*> getSolution() = 0;
    virtual ~SearchAI() {}

protected:
    SearchAI() {}

private:

};

bool isLegalAction(const Grid* grid, GridCell state, Action act);
GridCell doAction(GridCell state, Action act);
Node expandHelper(Node* parent, Action act);
int heuristicManhattan(GridCell start, GridCell end);

#endif