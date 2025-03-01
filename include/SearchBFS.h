#include "SearchAI.h"
#include "StackOrQueue.h"

class SearchBFS : public SearchAI
{
public:
    SearchBFS(const Grid* grid, GridCell start, GridCell end);
    ~SearchBFS() override;

    void step() override;
    void finish() override;
    bool done() override;

    const std::vector<Node*> getOpen() override;
    const std::vector<Node*> getClosed() override;
    const std::vector<Node*> getSolution() override;

private:
    GridCell start;
    GridCell end;
    StackOrQueue open; // always queue
    std::vector<Node*> closed;
    const Grid* grid = nullptr;
    Node* goal = nullptr;

    void init();
    void expand(Node* node, Action act);
};