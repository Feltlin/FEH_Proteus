#ifndef MAP
#define MAP

#include "simulator_libraries/FEHLCD.h"
#include "simulator_libraries/FEHImages.h"

#include <algorithm>
#include <vector>
#include <string>
#include <random>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#include "Player.h"

class Map{
    public:
        FEHImage Image;
        int height = 15, width = 20;
        int startRow = 0;
        int chunky = 0;
        //width=20, height=15
        std::vector<std::vector<int>> moss;
        std::vector<int> newRow;
        std::vector<int> collisionTile = {7, 8};

        void display(std::vector<std::vector<int>> *map, int x, int y){
            if(map->size() == 0){
                ++chunky;
                for(int i = height; i >= 0; --i){
                    newRow.clear();
                    for(int j = 0; j < width; ++j){
                        newRow.push_back(10*(stb_perlin_noise3((j)*0.05, (chunky * 15 - i)*0.05, 0, 0, 0, 0) + 1)/2);
                    }
                    map->insert(map->begin(), newRow);
                }

            }
            for(int row = startRow; row < startRow + height; ++row){
                for(int column = 0; column < std::min(int((*map)[row].size()), width); ++column){
                    switch((*map)[row][column]){
                        case 0:
                            continue;
                        
                        case 1:
                            Image.Open("./Image/Grass.png");
                            break;

                        case 2:
                            Image.Open("./Image/FloweryGrass.png");
                            break;

                        case 3:
                            Image.Open("./Image/Dirt.png");
                            break;

                        case 4:
                            Image.Open("./Image/Sand.png");
                            break;

                        case 5:
                            Image.Open("./Image/RedSand.png");
                            break;

                        case 6:
                            Image.Open("./Image/Stone.png");
                            break;

                        case 7:
                            Image.Open("./Image/StoneBrick.png");
                            break;

                        case 8:
                            Image.Open("./Image/Water.png");
                            break;

                        case 9:
                            Image.Open("./Image/Lava.png");
                            break;

                        case 10:
                            Image.Open("./Image/Shore.png");
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
            if(!collide((*map)[startRow + player->y - 1][player->x])){
                if(player->y > height / 2){
                        --player->y;
                }
                else if(startRow == 0){
                    //Use Perlin noise to generate the new chunk of map.
                    ++chunky;
                    for(int i = height; i >= 0; --i){
                        newRow.clear();
                        for(int j = 0; j < width; ++j){
                            newRow.push_back(10*(stb_perlin_noise3((j)*0.05, (chunky * 15 - i)*0.05, 0, 0, 0, 0) + 1)/2);
                        }
                        map->insert(map->begin(), newRow);
                    }

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
                if(!collide((*map)[startRow + player->y][player->x - 1])){
                    --player->x;
                }
            }
            player->direction = 'L';
            player->display();
        }

        void moveRight(std::vector<std::vector<int>> *map, Player *player){
            if(player->x < 19){
                if(!collide((*map)[startRow + player->y][player->x + 1])){
                    ++player->x;
                }
            }
            player->direction = 'R';
            player->display();
        }

        void moveDown(std::vector<std::vector<int>> *map, Player *player){
            if(player->y < height - 1){
                if(!collide((*map)[startRow + player->y + 1][player->x])){
                    ++player->y;
                }
            }
        }

        bool collide(int tile){
            for(int i = 0; i < collisionTile.size(); ++i){
                if(tile == collisionTile[i]){
                    return true;
                }
                else{
                    return false;
                }
            }
        }

};
#endif