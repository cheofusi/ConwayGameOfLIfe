#ifndef _CONWAYS_GAME_H_INCLUDE_
#define _CONWAYS_GAME_H_INCLUDE_

#include <vector>

class ConwayGame{
    private:
        const int GridWidth, GridHeight;
        const float threshold;
        int countNeighbors(const int x, const int y);
    
    public:
        std::vector<std::vector<bool>> cellGrid;
        ConwayGame(const int, const int, const float);
        ~ConwayGame();
        void InitCellGrid();
        void updateCellStates();

};

#endif