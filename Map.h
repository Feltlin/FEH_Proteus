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
        int killCount = 0;
        int mobAction = false;
        //width=20, height=15
        std::vector<std::vector<std::array<float, 2>>> layer0;
        std::vector<std::vector<std::array<float, 2>>> moss;
        std::vector<std::vector<std::array<float, 2>>> decoration;
        std::vector<std::vector<std::array<int, 2>>> mob;
        std::vector<int> collisionTile = {7, 8};
        std::map<int, std::vector<std::string>> tileIDMap;
        std::map<int, std::vector<std::string>> decorationIDMap;
        std::map<int, std::vector<std::string>> mobIDMap;

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

            mobIDMap[0] = {"./Image/Snake.png"};
            mobIDMap[1] = {"./Image/Robot.png"};
            mobIDMap[2] = {"./Image/Wizard_L.png"};
            mobIDMap[3] = {"./Image/Cod.png"};

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
            std::vector<std::array<float, 2>> newRow;
            //Generate water and stone.
            ++chunky;
            if((*layer0).size() >= 100){
                layer0->erase(layer0->begin() + 100, layer0->end());
                layer1->erase(layer1->begin() + 100, layer1->end());
            }
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
                        //1% chance generate an item on grass.
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
            std::vector<std::array<int, 2>> mobRow;
            for(int i = height; i >= 0; --i){
                mobRow.clear();
                for(int j = 0; j < width; ++j){
                    //Generate on sand tile.
                    if((*layer1)[i][j][0] == 3){
                        //0.05% generate a snake.
                        if(rand()%200 == 1){
                            mobRow.push_back({0, 10});
                        }
                        else{
                            mobRow.push_back({-1, 0});
                        }
                    }
                    //Generate on stone tile.
                    else if((*layer1)[i][j][0] == 1){
                        //0.01% generate a robot.
                        if(rand()%1000 == 1){
                            mobRow.push_back({1, 100});
                        }
                        else{
                            mobRow.push_back({-1, 0});
                        }
                    }
                    else if((*layer1)[i][j][0] == 2){
                        if(rand()%600 == 1){
                            mobRow.push_back({2, 25});
                        }
                        else{
                            mobRow.push_back({-1, 0});
                        }
                    }
                    else{
                        mobRow.push_back({-1, 0});
                    }
                }
                mob.insert(mob.begin(), mobRow);
            }
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
                    //Draw mob.
                    if(mob[row][column][0] != -1){
                        Image.Open(mobIDMap[mob[row][column][0]][0].data());
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
            if(mob[startRow + player->y - 1][player->x][0] != -1){
                fight(startRow + player->y - 1, player->x, &(player->damage), &(player->health));
            }
            else if(!collide(moss[startRow + player->y - 1][player->x][0])){
                ++step;
                mobAction = true;
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
            if(mob[startRow + player->y][player->x - 1][0] != -1){
                fight(startRow + player->y, player->x - 1, &(player->damage), &(player->health));
            }
            else if(player->x > 0){
                if(!collide(moss[startRow + player->y][player->x - 1][0])){
                    --player->x;
                    mobAction = true;
                }
            }
            player->direction = 'L';
            player->display();
        }

        void moveRight(Player *player){
            if(mob[startRow + player->y][player->x + 1][0] != -1){
                fight(startRow + player->y, player->x + 1, &(player->damage), &(player->health));
            }
            else if(player->x < 19){
                if(!collide(moss[startRow + player->y][player->x + 1][0])){
                    ++player->x;
                    mobAction = true;
                }
            }
            player->direction = 'R';
            player->display();
        }

        void moveDown(Player *player){
            if(mob[startRow + player->y + 1][player->x][0] != -1){
                fight(startRow + player->y + 1, player->x, &(player->damage), &(player->health));
            }
            else if(player->y < height - 1){
                if(!collide(moss[startRow + player->y + 1][player->x][0])){
                    --step;
                    ++player->y;
                    mobAction = true;
                }
            }
        }

        //Mob combat.
        void fight(int x, int y, int *damage, int *health){
            mobAction = false;
            if(mob[x][y][1] > 0){
                mob[x][y][1] -= *damage;
                if(mob[x][y][0] == 0){
                    *health -= 5;
                }
                else if(mob[x][y][0] == 1){
                    *health -= 50;
                }
                else if(mob[x][y][0] == 2){
                    *health -= 15;
                }
                if(mob[x][y][1] < 0){
                    mob[x][y][0] = -1;
                    ++killCount;
                }
            }
            else{
                mob[x][y][0] = -1;
            }
        }

        //Move the mob.
        void mobMove(){
            if(mobAction == true){
                mobAction = false;
                for(int i = startRow + height; i >= startRow; --i){
                    for(int j = 0; j < width; ++j){
                        if(mob[i][j][0] != -1){
                            int direction = rand() % 9;
                            switch(direction){
                                case 0: case 1: case 2: case 3:
                                    if(j + 1 < width && mob[i][j + 1][0] == -1){
                                        mob[i][j + 1] = mob[i][j];
                                        mob[i][j] = {-1, 0};
                                    }
                                    break;
                                case 4:
                                    if(i - 1 >= 0 && mob[i - 1][j][0] == -1){
                                        mob[i - 1][j] = mob[i][j];
                                        mob[i][j] = {-1, 0};
                                    }
                                    break;
                                case 5: case 6: case 7: case 8:
                                    if(j - 1 >= 0 && mob[i][j - 1][0] == -1){
                                        mob[i][j - 1] = mob[i][j];
                                        mob[i][j] = {-1, 0};
                                    }
                                    break;
                                case 9:
                                    if(i + 1 < mob.size() && mob[i + 1][j][0] == -1){
                                        mob[i + 1][j] = mob[i][j];
                                        mob[i][j] = {-1, 0};
                                    }
                                    break;
                            }
                        }
                    }
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