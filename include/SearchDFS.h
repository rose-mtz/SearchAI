#include "SearchAI.h"
#include <vector>
#include <limits>


class SearchDFS : public SearchAI
{
public:
    SearchDFS(const Grid* grid, GridCell start, GridCell end);
    ~SearchDFS() override;

    void step() override;
    void finish() override;
    bool done() override;

    const std::vector<Node*> getOpen() override;
    const std::vector<Node*> getClosed() override;
    const std::vector<Node*> getSolution() override;

private:
    GridCell start;
    GridCell end;
    std::vector<Node*> open; // stack
    std::vector<Node*> closed;
    const Grid* grid = nullptr;
    Node* goal = nullptr;
    bool foundGoal = false;
    bool failure = false;
    const unsigned int MAX_DEPTH = std::numeric_limits<unsigned int>::max(); // exclusive
    unsigned int CURRENT_MAX_DEPTH = 0; // exclusive


    void init();
    void expand(Node* node, Action act);
};
