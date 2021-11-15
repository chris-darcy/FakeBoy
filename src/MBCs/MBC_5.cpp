//
//  MBC_5.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 04/11/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "MBC_5.hpp"

MBC_5::MBC_5(){
    
}

MBC_5::MBC_5(uint16_t nROMBanks, uint8_t nRAMBanks)
:MBC(nROMBanks,nRAMBanks)
{
}

MBC_5::~MBC_5(){
    
}

void MBC_5::BankSwitching(uint16_t a, uint8_t data){
    
    // Enable RAM
    if (a < 0x2000){
        
        if((data & 0x0F) == 0x0A){
            RAMEnabled = true;
        }else{
            RAMEnabled = false;
        }
        
    }
    // Switch ROM Bank (lower 8 bits)
    else if (a < 0x3000){
        
        currentROMBank = data & 0xFF;
        
    }
    // Switch ROM Bank (upper 9th bit)
    else if (a < 0x4000){
        
        currentROMBank |= (data & 1) << 8;
        
    }
    // Switch RAM Bank
    else if (a < 0x6000){
        if(data <= 0xF){
            currentRAMBank = data;
        }
    }
}
