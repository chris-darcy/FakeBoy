//
//  MBC.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 03/11/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "MBC.hpp"

MBC::MBC(){
    
}

MBC::MBC(uint16_t nROMBanks, uint8_t nRAMBanks)
:nROMBanks(nROMBanks),nRAMBanks(nRAMBanks)
{}

MBC::~MBC(){
    
}

void MBC::MapAddrToReadROM(uint32_t &a){
    a += 0x4000*currentROMBank;
}

void MBC::MapAddrToReadRAM(uint32_t &a){
    a += 0x2000*currentRAMBank;
}

bool MBC::MapAddrToWriteRAM(uint32_t &a){
    if(!RAMEnabled){
        return false;
    }
    a += 0x2000*currentRAMBank;
    
    return true;
}

void MBC::BankSwitching(uint16_t a, uint8_t data){
    
}
