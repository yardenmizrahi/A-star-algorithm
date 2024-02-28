#pragma once
#include <math.h>
class Cell 
{
private:
    int row;
    int col;
    Cell* parent;

public:
    Cell();
    Cell(int r, int c, Cell* p);

    int getRow() { return row; }
    int getCol() { return col; }
    Cell* getParent() { return parent; }

    // Euclidean distance heuristic
    double getHeuristic(const Cell* target) {
        int dx = target->row - row;
        int dy = target->col - col;
        return sqrt(dx * dx + dy * dy);
    }

};
