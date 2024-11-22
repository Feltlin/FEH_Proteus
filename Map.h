#ifndef MAP
#define MAP

#include "simulator_libraries/FEHLCD.h"
#include "simulator_libraries/FEHImages.h"

#include <algorithm>
#include <vector>
#include <string>

class Map{
    public:
        FEHImage Image;
  
        int height = 15, width = 20;
        int startRow = 0;

        void display(std::vector<std::vector<int>> *map, int x, int y){
            for(int row = startRow; row < startRow + height; ++row){
                for(int column = 0; column < std::min(int((*map)[row].size()), width); ++column){
                    switch((*map)[row][column]){
                        case 0:
                            continue;
                        
                        case 1:
                            Image.Open("./Image/PaleMoss.png");
                            break;

                        case 2:
                            Image.Open("./Image/BrickRoad.png");
                            break;
                        
                    }
                    Image.Draw(x + column * 16, y + (row - startRow) * 16);
                    Image.Close();
                }
            }
        }

        void moveUp(std::vector<std::vector<int>> *map){
            
            std::vector<int> newRow = {0, 1, 2, 1, 2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
            if(startRow == 0){
                //Use a random generator to generate the new row of map.
                for(int i = 0; i < height; ++i){
                    map->insert(map->begin(), newRow);
                }
                startRow += height;
            }
            --startRow;
        }

        //width=20, height=15
        std::vector<std::vector<int>> moss = 
        {
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},

        };
};
#endif