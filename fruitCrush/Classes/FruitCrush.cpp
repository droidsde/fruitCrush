//
//  FruitCrush.cpp
//  fruitCrush
//
//  Created by Tsai Empire on 10/22/13.
//
//

#include "FruitCrush.h"

static FruitCrush* s_fruitCrush = NULL;

FruitCrush* FruitCrush::shareSingleton(void)
{
    if (!s_fruitCrush) {
        s_fruitCrush = new FruitCrush();
        
    }
    return s_fruitCrush;
}

bool FruitCrush::canPlayMap(int mapNo)
{
    return true;
}

void FruitCrush::setMapCompleted(int mapNo, bool completed)
{
    
}

CCSize FruitCrush::getWinSize()
{
    return CCDirector::sharedDirector()->getWinSize();
}

const char* FruitCrush::getItemString(int value) const
{
    switch (value) {
        case 1:
            return "red.png";
            break;
        case 2:
            return "orange.png";
            break;
        case 3:
            return "blue.png";
            break;
        case 4:
            return "yellow.png";
            break;
        case 5:
            return "green.png";
            break;
        case 6:
            return "purple.png";
            break;
        case 11:
            return "polka_red_horisontal.png";
            break;
        case 21:
            return "polka_orange_horisontal.png";
            break;
        case 31:
            return "polka_blue_horisontal.png";
            break;
        case 41:
            return "polka_yellow_horisontal.png";
            break;
        case 51:
            return "polka_green_horisontal.png";
            break;
        case 61:
            return "polka_purple_horisontal.png";
            break;
        case 12:
            return "polka_red_vertical.png";
            break;
        case 22:
            return "polka_orange_vertical.png";
            break;
        case 32:
            return "polka_blue_vertical.png";
            break;
        case 42:
            return "polka_yellow_vertical.png";
            break;
        case 52:
            return "polka_green_vertical.png";
            break;
        case 62:
            return "polka_purple_vertical.png";
        case 13:
            return "red_wrapper.png";
            break;
        case 23:
            return "orange_wrapper.png";
            break;
        case 33:
            return "blue_wrapper.png";
            break;
        case 43:
            return "yellow_wrapper.png";
            break;
        case 53:
            return "green_wrapper.png";
            break;
        case 63:
            return "purple_wrapper.png";
            break;
        case 7:
            return "bomb.png";
            break;
        case 8:
            return "cherry.png";
            break;
        case 9:
            return "hazelnut.png";
            break;
        case 999:
            return "cake_bomb_bl1.png";
            break;
        default:
            break;
    }
    return NULL;
}