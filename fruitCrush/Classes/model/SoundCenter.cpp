//
//  SoundCenter.cpp
//  fruitCrush
//
//  Created by Tsai on 13-11-17.
//
//

#include "SoundCenter.h"
static SoundCenter* _instance=NULL;
SoundCenter* SoundCenter::shareInstance()
{
    if (_instance == NULL) {
        _instance = new SoundCenter();
    }
    return _instance;
}