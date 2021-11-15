//
//  MBC_NONE.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 03/11/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "MBC_NONE.hpp"

MBC_NONE::MBC_NONE(){
    currentROMBank = 1; // 1st after 0th non switchable bank
}

MBC_NONE::~MBC_NONE(){
    
}

void MBC_NONE::BankSwitching(uint16_t a, uint8_t data){
    // No Switching Needed
}
