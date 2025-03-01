#ifndef GRID_H
#define GRID_H

struct GridCell
{
    unsigned int row;
    unsigned int col;

    bool operator==(const GridCell& other) const
    {
        return (this->row == other.row) && (this->col == other.col);
    }
};

class Grid
{
public:
    Grid(unsigned int numberOfRows, unsigned int numberOfColumns);
    Grid();
    ~Grid();

    void changeGrid(unsigned int rows, unsigned int columns);
    unsigned int get(unsigned int i, unsigned int j) const;
    void set(unsigned int i, unsigned int j, unsigned int newValue);
    unsigned int getNumberOfRows() const;
    unsigned int getNumberOfColumns() const;
    void clear(unsigned int value);
    bool outOfBounds(GridCell cell) const;
    void print() const;

private:
    unsigned int** grid = nullptr;
    unsigned int rows = 0;
    unsigned int columns = 0; 

    void free();
};

#endif