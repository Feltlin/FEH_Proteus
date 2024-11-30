#ifndef MAP
#define MAP

#include "simulator_libraries/FEHLCD.h"
#include "simulator_libraries/FEHImages.h"

#include <algorithm>
#include <vector>
#include <string>
#include <random>
#include <map>
#include <array>

#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"

#include "Player.h"

class Map{
    public:
        //Separate tile into a class? No do more 2d matrix: temperature, general type, item, mob.
        //Use temperature and general type to decide what tile will show on the map, what item and mob should be generated there.
        //draw more tiles
        //temperature for each tile: cold, cool, (temperate, )warm, hot
        //organize and separate tiles to group based on their type and temperature
        //height for water/river/ocean?
        //landscape item generation: flower, grass, tree
        //mob generation: mob AI
        //structure generation
        FEHImage Image;
        int height = 15, width = 20;
        int startRow = 0;
        int chunky = 0;
        int step = 0;
        int maxstep = 0;
        int randnum = rand();
        int tempnum = rand();
        //width=20, height=15
        std::vector<std::vector<std::array<float, 2>>> layer0;
        std::vector<std::vector<std::array<float, 2>>> moss;
        std::vector<std::vector<std::array<float, 2>>> decoration;
        std::vector<std::array<float, 2>> newRow;
        std::vector<int> collisionTile = {7, 8};
        std::map<int, std::vector<std::string>> tileIDMap;
        std::map<int, std::vector<std::string>> decorationIDMap;

        Map(){
            tileIDMap[-1] = {"Unknown", "./Image/PaleMoss.png"};
            tileIDMap[0] = {"Air", ""};
            tileIDMap[1] = {"Stone", "./Image/SnowStone.png", "./Image/CoolStone.png", "./Image/Stone.png", "./Image/WarmStone.png", "./Image/HotStone.png"};
            tileIDMap[2] = {"Grass", "./Image/ColdGrass.png", "./Image/CoolGrass.png", "./Image/Grass.png", "./Image/FloweryGrass.png", "./Image/Dirt.png"};
            tileIDMap[3] = {"Sand", "./Image/BlueSand.png", "./Image/GreenSand.png", "./Image/Sand.png", "./Image/Sand.png", "./Image/RedSand.png"};
            tileIDMap[7] = {"StoneBrick", "./Image/StoneBrick.png"};
            tileIDMap[8] = {"Water", "./Image/Water.png"};
            tileIDMap[9] = {"Lava", "./Image/Lava.png"};
            tileIDMap[10] = {"Shore", "./Image/Shore.png"};

            decorationIDMap[0] = {"./Image/Apple.png", "./Image/Watermelon.png"};
        }

        //Start a new map.
        void reset(){
            startRow = 0;
            chunky = 0;
            maxstep = std::max(step, maxstep);
            step = 0;
            randnum = rand();
            tempnum = rand();
            layer0.clear();
            moss.clear();
        }

        void generate(std::vector<std::vector<std::array<float, 2>>> *layer0, std::vector<std::vector<std::array<float, 2>>> *layer1){
            //Generate water and stone.
            ++chunky;
            for(int i = height; i >= 0; --i){
                newRow.clear();
                for(int j = 0; j < width; ++j){
                    float noise = 256*stb_perlin_noise3((j)*0.05, (chunky * 15 - i)*0.05, randnum, 0, 0, 0);
                    newRow.push_back(std::array<float, 2>{noise, 1});
                }
                layer0->insert(layer0->begin(), newRow);
            }
            //Generate general tile based on height.
            for(int i = height; i >= 0; --i){
                newRow.clear();
                for(int j = 0; j < width; ++j){
                    //Fill the value lower than -100 with water.
                    if((*layer0)[i][j][0] < -100){
                        newRow.push_back(std::array<float, 2>{8, 1});
                    }
                    //Generate other tile on the land.
                    else{
                        if((*layer0)[i][j][0] < 20){
                            (*layer0)[i][j][0] = 3;
                        }
                        else if((*layer0)[i][j][0] < 100){
                            (*layer0)[i][j][0] = 2;
                        }
                        else{
                            (*layer0)[i][j][0] = 1;
                        }
                        
                        //Generate variation tile based on temperature.
                        float noise = 5*(stb_perlin_noise3((j)*0.05, (chunky * 15 - i)*0.05, tempnum, 0, 0, 0) + 1)/2 + 1;
                        newRow.push_back(std::array<float, 2>{(*layer0)[i][j][0], noise});
                    }
                }
                layer1->insert(layer1->begin(), newRow);
            }
            //Generate decoration based on tile.
            for(int i = height; i >= 0; --i){
                newRow.clear();
                for(int j = 0; j < width; ++j){
                    //Generate on grass tile.
                    if((*layer1)[i][j][0] == 2){
                        //1% chance generate an item.
                        if(rand()%100 == 1){
                            //10% chance generate watermelon.
                            if(rand()%10 == 1){
                                newRow.push_back(std::array<float, 2>{0, 1});
                            }
                            //90% chance generate apple.
                            else{
                                newRow.push_back(std::array<float, 2>{0, 0});
                            }
                        }
                        else{
                            newRow.push_back(std::array<float, 2>{-1, 0});
                        }
                    }
                    else{
                        newRow.push_back(std::array<float, 2>{-1, 0});
                    }
                }
                decoration.insert(decoration.begin(), newRow);
            }
            //Generate mob based on tile.
        }

        void display(int x, int y){
            if(moss.size() == 0){
                generate(&layer0, &moss);
            }
            for(int row = startRow; row < startRow + height; ++row){
                for(int column = 0; column < std::min(int(moss[row].size()), width); ++column){
                    //Draw regular tile.
                    if(moss[row][column][0] > 0){
                        Image.Open(tileIDMap[moss[row][column][0]][moss[row][column][1]].data());
                        Image.Draw(x + column * 16, y + (row - startRow) * 16);
                        Image.Close();
                    }
                    //Skip air tile.
                    else if(moss[row][column][0] == 0){
                        continue;
                    }
                    //Draw unknown tile.
                    else{
                        Image.Open(tileIDMap[-1][1].data());
                        Image.Draw(x + column * 16, y + (row - startRow) * 16);
                        Image.Close();
                    }
                    //Draw decoration.
                    if(decoration[row][column][0] == 0){
                        Image.Open(decorationIDMap[decoration[row][column][0]][decoration[row][column][1]].data());
                        Image.Draw(x + column * 16, y + (row - startRow) * 16);
                        Image.Close();
                    }
                }
            }
        }

        void tileAction(Player *player){
            //Detect the decoration/item on the tile.
            if(decoration[startRow + player->y][player->x][0] == 0){
                //Apple detected.
                if(decoration[startRow + player->y][player->x][1] == 0){
                    player->health += 5;
                }
                //Watermelon detected.
                else if(decoration[startRow + player->y][player->x][0] == 1){
                    player->health += 20;
                }
                decoration[startRow + player->y][player->x][0] = -1;
            }
        }

        void moveUp(Player *player){
            if(!collide(moss[startRow + player->y - 1][player->x][0])){
                ++step;
                if(player->y > height / 2){
                        --player->y;
                }
                else if(startRow == 0){
                    //Use Perlin noise to generate the new chunk of map.
                    generate(&layer0, &moss);

                    startRow += height;
                    --startRow;
                }
                else{
                    --startRow;
                }
            }
            
        }

        void moveLeft(Player *player){
            if(player->x > 0){
                if(!collide(moss[startRow + player->y][player->x - 1][0])){
                    --player->x;
                }
            }
            player->direction = 'L';
            player->display();
        }

        void moveRight(Player *player){
            if(player->x < 19){
                if(!collide(moss[startRow + player->y][player->x + 1][0])){
                    ++player->x;
                }
            }
            player->direction = 'R';
            player->display();
        }

        void moveDown(Player *player){
            if(player->y < height - 1){
                if(!collide(moss[startRow + player->y + 1][player->x][0])){
                    --step;
                    ++player->y;
                }
            }
        }

        //Collision detection.
        bool collide(int tile){
            for(int i = 0; i < collisionTile.size(); ++i){
                if(tile == collisionTile[i]){
                    return true;
                }
            }
            return false;
        }

};
#endif