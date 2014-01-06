//
//  GridItem.cpp
//  fruitCrush
//
//  Created by Tsai on 13-10-31.
//
//

#include "GridItem.h"
GridItem::~GridItem()
{
    setItemBG(NULL);
}

bool GridItem::init()
{
    bool bRet = false;
    do {
        setItemAct(true);
        bRet = true;
    } while (0);
    
    return bRet;
}