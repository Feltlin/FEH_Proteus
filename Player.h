#ifndef PLAYER
#define PLAYER

#include "simulator_libraries/FEHLCD.h"
#include "simulator_libraries/FEHImages.h"

class Player{
    public:
        FEHImage Image;
        int x, y;
        int health;
        int damage;
        char role = 'K';
        char direction = 'R';

        Player(int x, int y){
            this->x = x;
            this->y = y;
            health = 100;
            damage = 0;
        }

        //This function was written by Junhao
        void gravityDisplay(){

            //Draw the player.
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
            Image.Draw(x, y);
            Image.Close();
        }

        void invDisplay(int x, int y){
            //Draw the player.
            switch(role){
                case 'K':
                    Image.Open("./Image/Knight_Lx64.png");
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
            Image.Draw(x, y);
            Image.Close();
        }

        //This function was written by Anthony
        void display(){

            //Draw the player.
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

            //Draw the health bar.
            if(health >= 100){
                Image.Open("./Image/FullHealth.png");
            }
            else if(health >= 90){
                Image.Open("./Image/90Health.png");
            }
            else if(health >= 80){
                Image.Open("./Image/80Health.png");
            }
            else if(health >= 70){
                Image.Open("./Image/70Health.png");
            }
            else if(health >= 60){
                Image.Open("./Image/60Health.png");
            }
            else if(health >= 50){
                Image.Open("./Image/50Health.png");
            }
            else if(health >= 40){
                Image.Open("./Image/40Health.png");
            }
            else if(health >= 30){
                Image.Open("./Image/30Health.png");
            }
            else if(health >= 20){
                Image.Open("./Image/20Health.png");
            }
            else if(health >= 10){
                Image.Open("./Image/10Health.png");
            }
            else if(health > 0){
                Image.Open("./Image/0Health.png");
            }
            //<= 0 dead.
            else{
                Image.Open("./Image/0Health.png");
            }
            
            Image.Draw(x*16, (y - 1)*16);
            Image.Close();
        }

};

#endif