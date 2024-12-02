#ifndef KEY
#define KEY

#include "simulator_libraries/FEHLCD.h"

class Key{
    public:
        bool press[2] = {false, false};

        //Only detect a single press.
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

        bool bind(int letter){
            press[0] = press[1];
            if (tigrKeyHeld(LCD.screen, letter)) {
                press[1] = true;
            }
            else{
                press[1] = false;
            }
            return !press[0] && press[1];
        }

        //Keep return true if the key is pressed.
        bool continueBind(char letter){
            if (tigrKeyHeld(LCD.screen, letter)) {
                return true;
            }
            return false;
        }

        bool continueBind(int letter){
            if (tigrKeyHeld(LCD.screen, letter)) {
                return true;
            }
            return false;
        }
};

#endif