//
//  Cartridge.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 06/09/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string>
#include <vector>
#include "MBCs/MBC_NONE.hpp"
#include "MBCs/MBC_1.hpp"
#include "MBCs/MBC_2.hpp"
#include "MBCs/MBC_5.hpp"

class Bus;

class Cartridge {
public:
    Cartridge();
    Cartridge(const std::string &rom_filename);
    ~Cartridge();
    
    std::string rom_name;
    bool valid;
    
public:
    
    void BankSwitching(uint16_t a, uint8_t data);
    
    void writeRAM(uint16_t a, uint8_t data);
    uint8_t readRAM(uint16_t a) const;
    uint8_t readROM(uint16_t a) const;
    
    void LoadFixedROM(uint8_t* memoryBus);
    
    void ConnectBus(Bus* b){
        bus = b;
    }
    
private:
    
    std::vector<uint8_t> romBanks;
    std::vector<uint8_t> ramBanks;
    
    uint16_t nROMBanks;
    uint8_t  nRAMBanks;
    
    uint8_t currentROMBank = 1; // 1st after 0th non switchable bank
    uint8_t currentRAMBank = 0;
    
    Bus* bus = nullptr;
    
    std::string save_filename;
    
    std::shared_ptr<MBC> pMBC;
};
