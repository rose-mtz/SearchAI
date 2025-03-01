#include "SearchAI.h"

class StackOrQueue
{
public:
    StackOrQueue();
    StackOrQueue(bool isStack);

    Node* pop();
    void push(Node* node);
    std::vector<GridCell> getGridCells();
    bool isEmpty() const;
    unsigned int size();
    std::vector<Node*> getNodes();

private:
    std::queue<Node*> queue;
    std::stack<Node*> stack;
    bool isStack;
};