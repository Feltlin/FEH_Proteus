#ifndef ITEM
#define ITEM

#include "simulator_libraries/FEHLCD.h"
#include "simulator_libraries/FEHImages.h"

class Item{
    public:
        FEHImage Image;
        int x, y;
        char kind = 'W';

        Item(int x, int y){
            this->x = x;
            this->y = y;
        }

        void display(){
            switch(kind){
                case 'W':
                    Image.Open("./Image/WoodenSword.png");
                    break;
                case 'S':
                    Image.Open("./Image/StoneSword.png");
                    break;
            }
            Image.Draw(x*16, y*16);
            Image.Close();
        }

};

#endif