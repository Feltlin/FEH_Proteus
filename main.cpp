#include "simulator_libraries/FEHLCD.h"
#include "simulator_libraries/LCDColors.h"
#include "simulator_libraries/FEHImages.h"
#include "simulator_libraries/FEHUtility.h"

#include <vector>
#include <string>
#include <math.h>

#include "Text.h"

void fish(FEHImage Image, int* x);
void draw(std::vector<std::vector<int>> *image, int x, int y);

int main()
{
    //Screen max: w=320, h=240
    Text text;
    FEHImage Image;
    int state = 0;
    float tx, ty;

    int x = 320;
    int y = 50;
    int t = 0;
    int amplitude = 4;
    int period = 2;
    bool redraw = false;

    while (1) {
        if (redraw) {
            LCD.Clear();
            redraw = false;
        }
        switch(state){
            case 0:
                fish(Image, &x);
                text.display("W", 0x586994, 136, 50 + amplitude * sin(t / period));
                text.display("el", 0x7D869C, 142, 50 + amplitude * sin(t / period));
                text.display("c", 0xA2ABAB, 154, 50 + amplitude * sin(t / period));
                text.display("o", 0xB4C4AE, 160, 50 + amplitude * sin(t / period));
                text.display("me!", 0xE5E8B6, 166, 50 + amplitude * sin(t / period));
                ++t;
                redraw = true;

                if(LCD.Touch(&tx, &ty)){
                    state = 1;
                    redraw = true;
                }
                break;

            case 1:
                if(text.button("Touch me", 0xF2542D, 136, 100, 2, 0xffffff, 100, 0xffffff - 0xF2542D)){
                    text.display("YOU WILL GET TOUCHED.", 0xff0000, 100, 58);
                }
                if(text.button("Credit",  0xF2542D, 136, 120, 2, 0xffffff, 100, 0xffffff - 0xF2542D)){
                    state = 2;
                    redraw = true;
                }
                break;
            case 2:
                text.display("Creator:", 0x586994, 100, 50);
                text.display("Junhao Liu", 0x7D869C, 100, 58);
                text.display("Anthony Ezzone", 0xA2ABAB, 100, 66);
                if(text.button("Back", 0xB4C4AE, 100, 100, 2, 0xffffff, 100, 0xffffff - 0xB4C4AE)){
                    state = 1;
                    redraw = true;
                }
                break;
            case 10:
                text.display("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0xff00ff, 100, 100);
                text.display("abcdefghijklmnopqrstuvwxyz", 0xff00ff, 100, 108);
                text.display("0123456789", 0xff00ff, 100, 116);
                text.display("+-*/=%\"\'#$@&_(){}<>[]", 0xff00ff, 100, 124);
                text.display(",.;:?!^~`", 0xff00ff, 100, 132);
                break;
        }
        LCD.Update();
    }


    return 0;
}

void fish(FEHImage Image, int* x){
    Image.Open("./Image/cod.png");
    for(int i = 0; i < 20; ++i){
        Image.Draw(*x-50*i, 12*i);
    }
    Image.Close();
    (x != 0) ? --*x : *x = 320;
}
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