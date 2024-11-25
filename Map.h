#ifndef MAP
#define MAP

#include "simulator_libraries/FEHLCD.h"
#include "simulator_libraries/FEHImages.h"

#include <algorithm>
#include <vector>
#include <string>
#include <random>

#include "PerlinNoise/PerlinNoise.hpp"

#include "Player.h"

class Map{
    public:
        FEHImage Image;
        siv::PerlinNoise perlin;
        int height = 15, width = 20;
        int startRow = 0;
        //width=20, height=15
        std::vector<std::vector<int>> moss;

        void display(std::vector<std::vector<int>> *map, int x, int y){
            if(moss.size() == 0){
                moss.resize(height, std::vector<int>(width, 1));
            }
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
                        default:
                            Image.Open("./Image/PaleMoss.png");
                        
                    }
                    Image.Draw(x + column * 16, y + (row - startRow) * 16);
                    Image.Close();
                }
            }
        }

        void moveUp(std::vector<std::vector<int>> *map, Player *player){
            std::vector<int> newRow = {0, 1, 2, 1, 2, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
            switch((*map)[startRow + player->y - 1][player->x]){
                case 2:
                    break;
                default:
                    if(player->y > height / 2){
                        --player->y;
                    }
                    else if(startRow == 0){
                        //Use a random generator to generate the new row of map.
                        for(int i = 0; i < height; ++i){
                            newRow.clear();
                            for(int j = 0; j < width; ++j){
                                newRow.push_back(2*perlin.octave2D_01(j, i, 4));
                            }
                            map->insert(map->begin(), newRow);
                        }

                        // for(int i = 0; i < height; ++i){
                        //     map->insert(map->begin(), newRow);
                        // }
                        startRow += height;
                        --startRow;
                    }
                    else{
                        --startRow;
                    }
            }
            
        }

        void moveLeft(std::vector<std::vector<int>> *map, Player *player){
            if(player->x > 0){
                switch((*map)[startRow + player->y][player->x - 1]){
                    case 2:
                        break;
                    default:
                        --player->x;
                }
            }
            player->direction = 'L';
            player->display();
        }

        void moveRight(std::vector<std::vector<int>> *map, Player *player){
            if(player->x < 19){
                switch((*map)[startRow + player->y][player->x + 1]){
                    case 2:
                        break;
                    default:
                        ++player->x;
                }
            }
            player->direction = 'R';
            player->display();
        }

        void moveDown(std::vector<std::vector<int>> *map, Player *player){
            if(player->y < height - 1){
                switch((*map)[startRow + player->y + 1][player->x]){
                    case 2:
                        break;
                    default:
                        ++player->y;
                }
            }
        }

};
#endif