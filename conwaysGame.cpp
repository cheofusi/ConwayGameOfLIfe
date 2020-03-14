#include <cstdlib>
#include <iostream>

#include "conwaysGame.h"

ConwayGame::ConwayGame (const int gridWidth, const int gridHeight, const float thold)
            :GridWidth(gridWidth), GridHeight(gridHeight), threshold(thold)
{
    InitCellGrid();
}

ConwayGame::~ConwayGame(){
    std::vector<std::vector<bool>>().swap(cellGrid);
}

void ConwayGame::InitCellGrid(){
    for(int j = 0; j < GridHeight; ++j){
        std::vector<bool> v;
        for(int i = 0; i < GridWidth; ++i){
			v.push_back(0);//(((float)rand() / RAND_MAX) < threshold));
        }
        cellGrid.push_back(v);
    }
    cellGrid[25][25] = 1;
    cellGrid[25][26] = 1;
    cellGrid[24][25] = 1;
    cellGrid[26][26] = 0;
    cellGrid.shrink_to_fit();
}

void ConwayGame::updateCellStates(){
    int numNeighbors;
    for(int j = 0; j < GridHeight; ++j){
        for(int i = 0; i < GridWidth; ++i){
            numNeighbors = countNeighbors(i, j);
            if(cellGrid[j][i]){ // cell is alive
                if(numNeighbors < 2 || numNeighbors > 3) cellGrid[j][i] = 0;
                //else cell stays alive
            }
            else{ // cell's dead
                if(numNeighbors == 3) cellGrid[j][i] = 1;
            }
        }
    }
}

int ConwayGame::countNeighbors(const int x, const int y){
    // Counts number of neighbours alive
    // This code is shitty. Need to refactor
    int count = 0;
    if(x+1 < GridWidth) count += cellGrid[y][x+1] ? 1 : 0 ;
    if(x+1 < GridWidth && y+1 < GridHeight) count += cellGrid[y+1][x+1] ? 1 : 0 ;
    if(y+1 < GridWidth) count += cellGrid[y+1][x] ? 1 : 0 ;
    if(x-1 >= 0 && y+1 < GridHeight) count += cellGrid[y+1][x-1] ? 1 : 0 ;
    if(x-1 >= 0) count += cellGrid[y][x-1] ? 1 : 0 ;
    if(x-1 >= 0 && y-1 >= 0) count += cellGrid[y-1][x-1] ? 1 : 0 ;
    if(y-1 >= 0) count += cellGrid[y-1][x] ? 1 : 0 ;
    if(x+1 < GridWidth && y-1 >= 0) count += cellGrid[y-1][x+1] ? 1 : 0 ;

    return count;
}