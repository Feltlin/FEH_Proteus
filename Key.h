#ifndef KEY
#define KEY

#include "simulator_libraries/FEHLCD.h"

class Key{
    public:
        bool press[2] = {false, false};

        bool bind(char letter){
            press[0] = press[1];
            if (tigrKeyHeld(LCD.screen, letter)) {
                press[1] = true;
            }
            else{
                press[1] = false;
            }
            return !press[0] && press[1];
        }
};

#endif