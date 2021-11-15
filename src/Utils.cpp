//
//  Utils.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 16/09/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "Utils.hpp"

bool TestBit(uint8_t data, uint8_t b){
    return (data & (1<<b)) > 0;
}

uint8_t SetBit(uint8_t data, uint8_t b, bool v){
    if (v)
        return data |= (1<<b);
    else
        return data &= ~(1<<b);
}
