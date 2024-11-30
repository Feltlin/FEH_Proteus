#ifndef PLAYER
#define PLAYER

#include "simulator_libraries/FEHLCD.h"
#include "simulator_libraries/FEHImages.h"

class Player{
    public:
        FEHImage Image;
        int x, y;
        char role = 'K';
        char direction = 'R';

        Player(int x, int y){
            this->x = x;
            this->y = y;
        }

        void display(){
            switch(role){
                case 'K':
                    switch(direction){
                        case 'L':
                            Image.Open("./Image/Knight_L.png");
                            break;
                        
                        case 'R':
                            Image.Open("./Image/Knight_R.png");
                            break;

                    }
                    break;

                case 'W':
                    switch(direction){
                        case 'L':
                            Image.Open("./Image/Wizard_L.png");
                            break;
                        
                        case 'R':
                            Image.Open("./Image/Wizard_R.png");
                            break;
                    }
                    break;
            }
            Image.Draw(x*16, y*16);
            Image.Close();
        }

};

#endif