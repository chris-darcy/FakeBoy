//
//  MBC_2.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 03/11/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "MBC_2.hpp"

MBC_2::MBC_2(){
    currentROMBank = 1; // 1st after 0th non switchable bank
}

MBC_2::MBC_2(uint16_t nROMBanks, uint8_t nRAMBanks)
:MBC(nROMBanks,nRAMBanks)
{
    currentROMBank = 1;
}

MBC_2::~MBC_2(){
    
}

void MBC_2::BankSwitching(uint16_t a, uint8_t data){
    
    // Enable RAM
    if (a < 0x2000){
        
        if(a & 0b10000) return;
        
        if((data & 0x0F) == 0x0A){
            RAMEnabled = true;
        }else{
            RAMEnabled = false;
        }
        
    }
    // Switch ROM Bank
    else if (a < 0x4000){
        
        if(a & 0b10000) return;
        
        currentROMBank = data & 0xF;
    }
}
