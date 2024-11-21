#ifndef PLAYER
#define PLAYER

#include "simulator_libraries/FEHLCD.h"
#include "simulator_libraries/FEHImages.h"

class Player{
    public:
        FEHImage Image;
        int x, y;

        Player(int x, int y){
            this->x = x;
            this->y = y;
        }

        void display(){
            Image.Open("./Image/Knight.png");
            Image.Draw(x*16, y*16);
            Image.Close();
        }

        void moveLeft(){
            if(x > 0){
                --x;
            }
            display();
        }

        void moveRight(){
            if(x < 19){
                ++x;
            }
            display();
        }
};

#endif