#include "simulator_libraries/FEHLCD.h"
#include "simulator_libraries/FEHImages.h"

#include <vector>
#include <string>
#include <math.h>

#include "Text.h"
#include "Map.h"
#include "LargeMap.h"
#include "Player.h"
#include "Key.h"
#include "Item.h"

//Function prototype.
void fish(FEHImage Image, int* x);
void draw(std::vector<std::vector<int>> *image, int x, int y);

int main(){   

    //Predefined object and variable.
    //Screen max: w=320=16*20, h=240=16*15
    Text text;
    Text newMap, moveUp, moveLeft, moveRight, moveDown;
    Key keyW, keyA, keyD, keyS;
    Map map;
    LargeMap largeMap;
    Player player(10, 14), largePlayer(0, 0);
    Item item(10, 16);
    FEHImage Image;
    int state = 0, menuState = 0, keyState = 0, statState = 0;
    float tx, ty;

    //Color gradient from light to dark.
    std::vector<int> springBuds = {0xD9ED92, 0xB5E48C, 0x99D98C, 0x76C893, 0x52B69A, 0x34A0A4, 0x168AAD, 0x1A759F, 0x1E6091, 0x184E77};
    std::vector<int> paleMoss = {0xCAD2C5, 0x84A98C, 0x52796F, 0x354F52, 0x2F3E46};
    std::vector<int> lava = {0xFFBA08, 0xFAA307, 0xF48C06, 0xE85D04, 0xDC2F02, 0xD00000, 0x9D0208, 0x6A040F, 0x370617, 0x03071E};
    std::vector<int> alpenglow = {0xFFCDB2, 0xFFB4A2, 0xE5989B, 0xB5838D, 0x6D6875};
    std::vector<int> foggyValley = {0xE5E8B6, 0xB4C4AE, 0xA2ABAB, 0x7D869C, 0x586994};

    int x = 320;
    int y = 50;
    int t = 0;
    int amplitude = 4;
    int period = 2;
    bool keyChose = false;
    int score;

    //Game loop.
    while (1){

        //Clear the screen after each new frame update.
        LCD.Clear();

        //State machine for deciding the window showing on the screen.
        switch(state){

            //Start screen.
            case 0:

                //Draw fish mvoing on the screen.
                fish(Image, &x);

                //Multicolor text with oscillatory motion.
                text.display("W", foggyValley[4], 136, 50 + amplitude * sin(t / period));
                text.display("el", foggyValley[3], 142, 50 + amplitude * sin(t / period));
                text.display("c", foggyValley[2], 154, 50 + amplitude * sin(t / period));
                text.display("o", foggyValley[1], 160, 50 + amplitude * sin(t / period));
                text.display("me!", foggyValley[0], 166, 50 + amplitude * sin(t / period));
                ++t;

                //Touch anywhere to switch to the next screen.
                if(LCD.Touch(&tx, &ty, false)){
                    state = 1;
                }
                break;

            //Main screen.
            case 1:

                //Fun button, may get removed or change place in the future.
                if(text.button("Touch me", lava[0], 136, 100, 2, lava[0], -1, 0xffffff - lava[0])){
                    text.display("YOU WILL GET TOUCHED.", 0xff0000, 100, 58);
                }

                //Start the main game.
                if(text.button("Start",  lava[1], 136, 120, 2, lava[1], -1, 0xffffff - lava[1])){
                    if(!keyChose){
                        state = 5;
                    }
                    else{
                        state = 10;
                    }
                }

                //Generate a new map.
                if(newMap.button("New Map",  lava[1], 175, 120, 2, lava[1], -1, 0xffffff - lava[1])){
                    if(!newMap.click[0] && newMap.click[1]){
                        map.reset();
                        player.x = 10;
                        player.y = 14;
                    }
                }

                //Switch to credit screen.
                if(text.button("Credit", lava[2], 136, 140, 2, lava[2], -1, 0xffffff - lava[2])){
                    state = 2;
                }

                //Switch to instruction screen.
                if(text.button("Instruction", lava[3], 136, 160, 2, lava[3], -1, 0xffffff - lava[3])){
                    state = 3;
                }

                //Switch to stats screen.
                if(text.button("Stats", lava[4], 136, 180, 2, lava[4], -1, 0xffffff - lava[4])){
                    state = 4;
                }

                //Switch to large map.
                if(text.button("?(Experimental)", lava[5], 136, 200, 2, lava[5], -1, 0xffffff - lava[5])){
                    state = 20;
                }

                //Keyboard 'A' press detection.
                if (tigrKeyHeld(LCD.screen, 'A')) {
                    text.display("A pressed!", foggyValley[0], 166, 50);
                }
                break;

            //Credit screen.
            case 2:

                //Display the credit.
                text.display("Creator:", foggyValley[4], 100, 50);
                text.display("Junhao Liu", foggyValley[3], 100, 58);
                text.display("Anthony Ezzone", foggyValley[2], 100, 66);
                text.display("Font: Mojang", 0x157A6E, 100, 82);
                text.display("https://www.dafont.com/mojang.font", 0x499F68, 100, 90);
                text.display("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0x77B28C, 100, 98);
                text.display("abcdefghijklmnopqrstuvwxyz", 0x77B28C, 100, 106);
                text.display("0123456789", 0x77B28C, 100, 114);
                text.display("+-*/=%\"\'#$@&_(){}<>[]", 0xC2C5BB, 100, 122);
                text.display(",.;:?!^~`", 0xC2C5BB, 100, 130);

                //Back to the main screen.
                if(text.button("Back", 0xB4C4AE, 100, 150, 2, 0xffffff, -1, 0xffffff - 0xB4C4AE)){
                    state = 1;
                }
                break;

            //Instruction screen.
            case 3:

                //Display the game background.
                text.display("Instruction", 0x586994, 100, 50);
                text.display("In this game you are a hero fighting his way through ", foggyValley[0], 5, 60);
                text.display("the land of malpatria, which has been taken over by ", foggyValley[1], 5, 68);
                text.display("evil mobs that roam throughout the world. You are ", foggyValley[2], 5, 76);
                text.display("set on your journey with a sword in hand and a ", foggyValley[3], 5, 84);
                text.display("simple goal to travel as far as you can. The only ", foggyValley[4], 5, 92);
                text.display("instructions for this game are simple, you must ", foggyValley[0], 5, 100);
                text.display("travel upward to progress the game. Kill as many ", foggyValley[1], 5, 108);
                text.display("enemies as possible and upgrade your sword along ", foggyValley[2], 5, 116);
                text.display("the way. You will take damage from mobs if you let ", foggyValley[3], 5, 124);
                text.display("them hit you, but mobs can drop healing items to ", foggyValley[4], 5, 132);
                text.display("restore health along the way. Points can be earned ", foggyValley[0], 5, 140);
                text.display("by defeating mobs and traveling a greater distance. ", foggyValley[1], 5, 148);
                text.display("The score will be totaled up and put on the ", foggyValley[2], 5, 156);
                text.display("leaderboard in the statistics section. Have fun!", foggyValley[3], 5, 164);
                
                //Back to the main screen.
                if(text.button("Back", 0xB4C4AE, 100, 180, 2, 0xffffff, -1, 0xffffff - 0xB4C4AE)){
                    state = 1;
                }
                break;

            //Stats screen.
            case 4:
                text.display("Stats", 0x586994, 100, 20);
                if(text.button("Step Stats", 0xB4C4AE, 10, 40, 2, 0xffffff, -1, 0xffffff - 0xB4C4AE)){
                    statState = 0;
                }
                if(text.button("Kill Stats", 0xB4C4AE, 80, 40, 2, 0xffffff, -1, 0xffffff - 0xB4C4AE)){
                    statState = 1;
                }
                if(text.button("Score Leaderboard", 0xB4C4AE, 150, 40, 2, 0xffffff, -1, 0xffffff - 0xB4C4AE)){
                    statState = 2;
                }
                switch(statState){
                    case 0:
                        text.display("Max Step: " + std::to_string(map.maxstep), foggyValley[1], 100, 60);
                        break;

                    case 1:
                        text.display("Total Kill Count: " + std::to_string(map.totalKillCount), foggyValley[2], 100, 60);
                        text.display("Top Kill Count Board: ", foggyValley[2], 100, 68);
                        for(int i = 0; i < map.singleKillCountBoard.size(); ++i){
                            text.display(std::to_string(map.singleKillCountBoard[i]), lava[i], 100, 76 + 8*i);
                        }
                        break;
                    
                    case 2:
                        text.display("Score Leaderboard: ", foggyValley[2], 100, 60);
                        for(int i = 0; i < map.ScoreBoard.size(); ++i){
                            text.display(std::to_string(map.ScoreBoard[i]), lava[i], 100, 68 + 8*i);
                        }
                        break;
                }


                //Back to the main screen.
                if(text.button("Back", 0xB4C4AE, 100, 150, 2, 0xffffff, -1, 0xffffff - 0xB4C4AE)){
                    state = 1;
                }
                break;

            case 5:
                text.display("Choose movement feature", 0x586994, 100, 50);
                if(text.button("WASD Keys", 0x586994, 50, 100)){
                    keyState = 0;
                    keyChose= true;
                    state = 10;
                }
                if(text.button("Arrow buttons", 0x586994, 125, 100)){
                    keyState = 1;
                    keyChose= true;
                    state = 10;
                }
                if(text.button("Both", 0x586994, 225, 100)){
                    keyState = 2;
                    keyChose= true;
                    state = 10;
                }
                break;
            //Main game.
            case 10:
                //Move the mob.
                map.mobMove();

                //Display the map.
                map.display(0, 0);

                //Display the player.
                player.display();

                //Detect what happened in this tile.
                map.tileAction(&player);

                //Display the item.
                item.display();

                //Display the steps that player have traveled.
                text.button(std::to_string(map.step), lava[3], 150, 16, 1, 0xffffff, lava[0]);

                if(player.health <= 0){
                    state = 11;
                }

                //Open in-game menu.
                switch(menuState){

                    //When menu is not open.
                    case 0:

                        //Back to the main screen..
                        if(text.button("Back", 0xB4C4AE, 10, 200, 2, paleMoss[2], paleMoss[3], 0xffffff - 0xB4C4AE)){
                            state = 1;
                        }

                        //Open the menu.
                        if(text.button("Inventory", 0xB4C4AE, 10, 220, 2, paleMoss[2], paleMoss[3], 0xffffff - 0xB4C4AE)){
                            menuState = 1;
                        }

                        //Key press and button press detection. Future may let the player choose to use only button or only key press or both.
                        if(keyState == 0){
                            if(keyW.bind('W')){
                                map.moveUp( &player);
                            }
                            else if(keyA.bind('A')){
                                map.moveLeft(&player);
                            }
                            else if(keyD.bind('D')){
                                map.moveRight(&player);
                            }
                            else if(keyS.bind('S')){
                                map.moveDown(&player);
                            }
                        }
                        else if(keyState == 1){
                            if(moveUp.button("^", paleMoss[0], 270, 200, 2, paleMoss[2], paleMoss[3], 0xffffff - 0xB4C4AE)){
                                if(!moveUp.click[0] && moveUp.click[1]){
                                map.moveUp(&player);
                                }
                            }
                            if(moveLeft.button("<", paleMoss[0], 240, 210, 2, paleMoss[2], paleMoss[3], 0xffffff - 0xB4C4AE)){
                                if(!moveLeft.click[0] && moveLeft.click[1]){
                                map.moveLeft(&player);
                                }
                            }
                            if(moveRight.button(">", paleMoss[0], 300, 210, 2, paleMoss[2], paleMoss[3], 0xffffff - 0xB4C4AE)){
                                if(!moveRight.click[0] && moveRight.click[1]){
                                    map.moveRight(&player);
                                }
                            }
                            if(moveDown.button("v", paleMoss[0], 270, 220, 2, paleMoss[2], paleMoss[3], 0xffffff - 0xB4C4AE)){
                                if(!moveDown.click[0] && moveDown.click[1]){
                                    map.moveDown(&player);
                                }
                            }
                        }
                        else if(keyState == 2){
                            if(keyW.bind('W')){
                                map.moveUp( &player);
                            }
                            else if(keyA.bind('A')){
                                map.moveLeft(&player);
                            }
                            else if(keyD.bind('D')){
                                map.moveRight(&player);
                            }
                            else if(keyS.bind('S')){
                                map.moveDown(&player);
                            }
                            if(moveUp.button("^", paleMoss[0], 270, 200, 2, paleMoss[2], paleMoss[3], 0xffffff - 0xB4C4AE)){
                                if(!moveUp.click[0] && moveUp.click[1]){
                                map.moveUp(&player);
                                }
                            }
                            if(moveLeft.button("<", paleMoss[0], 240, 210, 2, paleMoss[2], paleMoss[3], 0xffffff - 0xB4C4AE)){
                                if(!moveLeft.click[0] && moveLeft.click[1]){
                                map.moveLeft(&player);
                                }
                            }
                            if(moveRight.button(">", paleMoss[0], 300, 210, 2, paleMoss[2], paleMoss[3], 0xffffff - 0xB4C4AE)){
                                if(!moveRight.click[0] && moveRight.click[1]){
                                    map.moveRight(&player);
                                }
                            }
                            if(moveDown.button("v", paleMoss[0], 270, 220, 2, paleMoss[2], paleMoss[3], 0xffffff - 0xB4C4AE)){
                                if(!moveDown.click[0] && moveDown.click[1]){
                                    map.moveDown(&player);
                                }
                            }
                        }
                        break;

                    //When the in-game menu is open.
                    case 1:
                        text.drawBorder(foggyValley[0], 10, 10, 310, 230);
                        text.drawBox(foggyValley[1], 10, 10, 310, 230);
                        player.invDisplay(200, 100);
                        text.display("HP: " + std::to_string(player.health), lava[3], 20, 200);
                        text.display("Damage: " + std::to_string(player.damage), lava[4], 20, 208);
                        if(text.button("Back", 0xB4C4AE, 150, 170, 2, 0xffffff, -1, 0xffffff - 0xB4C4AE)){
                            menuState = 0;
                        }
                        
                        if(text.imageButton("./Image/WoodenSword.png", 20, 20, 0)){
                            if(!text.click[0] && text.click[1]){
                                    player.damage = 3;
                            }
                        }
                        if(map.totalKillCount >= 50){
                            if(text.imageButton("./Image/StoneSword.png", 40, 20, 0)){
                                if(!text.click[0] && text.click[1]){
                                    player.damage = 5;
                                }
                            }
                        }
                        if(map.totalKillCount >= 100){
                            if(text.imageButton("./Image/GoldSword.png", 60, 20, 0)){
                                if(!text.click[0] && text.click[1]){
                                    player.damage = 8;
                                }
                            }
                        }
                        if(map.totalKillCount >= 150){
                            if(text.imageButton("./Image/EmeraldSword.png", 80, 20, 0)){
                                if(!text.click[0] && text.click[1]){
                                    player.damage = 10;
                                }
                            }
                        }
                        if(map.totalKillCount >= 200){
                            if(text.imageButton("./Image/RubySword.png", 100, 20, 0)){
                                if(!text.click[0] && text.click[1]){
                                    player.damage = 13;
                                }
                            }
                        }
                        if(map.totalKillCount >= 500){
                            if(text.imageButton("./Image/DiamondSword.png", 120, 20, 0)){
                                if(!text.click[0] && text.click[1]){
                                    player.damage = 100;
                                }
                            }
                        }    
                }
                break;

            case 11:
                text.display("You died", lava[4], 150, 100);
                score = map.step + map.snakeKillCount*5 + map.robotKillCount*100 + map.wizardKillCount*20;
                text.display("Score " + std::to_string(score), lava[5], 150, 150);
                if(text.button("Restart", 0xB4C4AE, 50, 200, 2, paleMoss[2], paleMoss[3], 0xffffff - 0xB4C4AE)){
                    if(!text.click[0] && text.click[1]){
                        map.reset();
                        player.x = 10;
                        player.y = 14;
                    }
                    player.health = 100;
                    state = 1;

                }

                break;
                
            case 20:
                largeMap.displayPixel(largePlayer.x, largePlayer.y);
                if(text.button("Back", 0xB4C4AE, 10, 200, 2, paleMoss[2], paleMoss[3], 0xffffff - 0xB4C4AE)){
                    state = 1;
                }
                if(keyW.bind('W')){
                    largePlayer.y -= 16;
                }
                else if(keyA.bind('A')){
                    largePlayer.x -= 16;
                }
                else if(keyD.bind('D')){
                    largePlayer.x += 16;
                }
                else if(keyS.bind('S')){
                    largePlayer.y += 16;
                }
                break;
        }

        //Update the frame.
        LCD.Update();
    }

    return 0;
}

//Draw the fish on the start screen.
void fish(FEHImage Image, int* x){
    Image.Open("./Image/cod.png");
    for(int i = 0; i < 20; ++i){
        int tempx = *x-50*i;
        while(tempx <= 0){
            tempx += 320;
        }
        Image.Draw(tempx, 12*i);
    }
    Image.Close();
    (*x > 0) ? --*x : *x = 320;
}

//Not used.
//Draw the 2D vector of Hex RGB image on the screen.
//vector of pixel vectors <color, x, y>
void draw(std::vector<std::vector<int>> *image, int x, int y){
    for(int row = 0; row < image->size(); ++row){
        for(int column = 0; column < (*image)[row].size(); ++column){
            if((*image)[row][column] != -1){
                LCD.SetFontColor((*image)[row][column]);
                LCD.DrawPixel(column + x, row + y);
            }
        }
    }
}