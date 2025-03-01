#include "SearchAI.h"
#include <vector>


class SearchGreedy : public SearchAI
{
public:
    SearchGreedy(const Grid* grid, GridCell start, GridCell end);
    ~SearchGreedy() override;

    void step() override;
    void finish() override;
    bool done() override;

    const std::vector<Node*> getOpen() override;
    const std::vector<Node*> getClosed() override;
    const std::vector<Node*> getSolution() override;

private:
    GridCell start;
    GridCell end;
    std::vector<Node*> open;
    std::vector<Node*> closed;
    const Grid* grid = nullptr;
    Node* goal = nullptr;
    bool foundGoal = false;


    void init();
    void expand(Node* node, Action act);
};