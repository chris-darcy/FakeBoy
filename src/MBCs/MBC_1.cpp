//
//  MBC_1.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 03/11/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "MBC_1.hpp"

MBC_1::MBC_1(){
    currentROMBank = 1; // 1st after 0th non switchable bank
}

MBC_1::MBC_1(uint16_t nROMBanks, uint8_t nRAMBanks)
:MBC(nROMBanks,nRAMBanks)
{
    currentROMBank = 1;
}

MBC_1::~MBC_1(){
    
}

void MBC_1::BankSwitching(uint16_t a, uint8_t data){
    
    // Enable RAM
    if (a < 0x2000){
        
        if((data & 0x0F) == 0x0A){
            RAMEnabled = true;
        }else{
            RAMEnabled = false;
        }
        
    }
    // Switch ROM Bank
    else if (a < 0x4000){

        currentROMBank = data & 0b11111;
        if(currentROMBank == 0) currentROMBank++;
        
    }
    // Switch ROM or RAM Bank (Mode Dependent)
    else if (a < 0x6000){

        if(ROMBankingMode){
            if(nROMBanks == 64){
                currentROMBank &= 0b1111;
                data &= 0b110000;
                currentROMBank |= data;
                
            }else{
                currentROMBank &= 0b11111;
                data &= 0b1100000;
                currentROMBank |= data;
            
            }
            if(currentROMBank == 0) currentROMBank++;
            
        }else{
            currentRAMBank = data & 0b11;
        }
    }
    // Switch Banking Mode
    else if (a < 0x8000){
        if(data & 1){
            ROMBankingMode = true;
            currentRAMBank = 0;
        }else{
            ROMBankingMode = false;
        }
    }
}
