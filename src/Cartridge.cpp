//
//  Cartridge.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 06/09/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "Cartridge.hpp"
#include "Debugger.hpp"
#include <fstream>

Cartridge::Cartridge(){
    
}

Cartridge::Cartridge(const std::string &rom_filename){
    
    // reserve for cartridge header bytes
    romBanks.resize(0x150);
    
    rom_name = save_filename = rom_filename;
    
    // remove .gb extension and ROMS/
    rom_name.erase(rom_name.end() - 3, rom_name.end());
    rom_name.erase(rom_name.begin(), rom_name.begin() + 5);

    // make .sav file in ROMS folder
    save_filename.erase(save_filename.end() - 2, save_filename.end());
    save_filename.append("sav");
    
    std::ifstream ifs;
    ifs.open(rom_filename, std::ifstream::binary);
    
    if((valid = ifs.is_open())){

        ifs.read((char*)romBanks.data(), 0x150);
        
        switch(romBanks[0x0148]){
            case 0: nROMBanks =   2; break;
            case 1: nROMBanks =   4; break;
            case 2: nROMBanks =   8; break;
            case 3: nROMBanks =  16; break;
            case 4: nROMBanks =  32; break;
            case 5: nROMBanks =  64; break;
            case 6: nROMBanks = 128; break;
            case 7: nROMBanks = 256; break;
            case 8: nROMBanks = 512; break;
            default: break;
        }
        romBanks.resize(nROMBanks * 16384);
        ifs.read((char*)romBanks.data() + 0x150, romBanks.size());
        ifs.close();
        
        switch(romBanks[0x0149]){
            case 0: nRAMBanks =  0; break;
            case 2: nRAMBanks =  1; break;
            case 3: nRAMBanks =  4; break;
            case 4: nRAMBanks = 16; break;
            case 5: nRAMBanks =  8; break;
            default: break;
        }
        if(nRAMBanks == 0){
            ramBanks.resize(8192);
        }else{
            ramBanks.resize(nRAMBanks * 8192);
        }
        
        ifs.open(save_filename, std::ifstream::binary);
        if(!ifs.fail()){
            ifs.read((char*)ramBanks.data(), ramBanks.size());
        }
        ifs.close();
        
        // Determine the cartridges Memory Bank Controller type
        switch(romBanks[0x147]){
            case 0x00: pMBC = std::make_shared<MBC_NONE>(); break;
            case 0x01: pMBC = std::make_shared<MBC_1>(nROMBanks, nRAMBanks); break;
            case 0x02: pMBC = std::make_shared<MBC_1>(nROMBanks, nRAMBanks); break;
            case 0x03: pMBC = std::make_shared<MBC_1>(nROMBanks, nRAMBanks); break;
            case 0x05: pMBC = std::make_shared<MBC_2>(nROMBanks, nRAMBanks); break;
            case 0x06: pMBC = std::make_shared<MBC_2>(nROMBanks, nRAMBanks); break;
            case 0x19: pMBC = std::make_shared<MBC_5>(nROMBanks, nRAMBanks); break;
            case 0x1A: pMBC = std::make_shared<MBC_5>(nROMBanks, nRAMBanks); break;
            case 0x1B: pMBC = std::make_shared<MBC_5>(nROMBanks, nRAMBanks); break;
            case 0x1C: pMBC = std::make_shared<MBC_5>(nROMBanks, nRAMBanks); break;
            case 0x1D: pMBC = std::make_shared<MBC_5>(nROMBanks, nRAMBanks); break;
            case 0x1E: pMBC = std::make_shared<MBC_5>(nROMBanks, nRAMBanks); break;
        };
    }
}

Cartridge::~Cartridge(){
    
    // if no ROM, don't make a save file!
    if(!valid){
        return;
    }

    FILE* in = fopen(save_filename.c_str(), "wb+" );
    if(in != nullptr){
        fwrite (ramBanks.data() , sizeof(uint8_t), ramBanks.size() * sizeof(uint8_t), in);
    }
    fclose (in);
    
}

void Cartridge::writeRAM(uint16_t a, uint8_t data){
    
    uint32_t mapped_a = a;
    
    if(pMBC->MapAddrToWriteRAM(mapped_a)){
        ramBanks[mapped_a] = data;
    }
}

uint8_t Cartridge::readRAM(uint16_t a) const{
    
    uint32_t mapped_a = a;
    
    pMBC->MapAddrToReadRAM(mapped_a);
    return ramBanks[mapped_a];
}

uint8_t Cartridge::readROM(uint16_t a) const{
    
    uint32_t mapped_a = a;
    
    pMBC->MapAddrToReadROM(mapped_a);
    return romBanks[mapped_a];
}

void Cartridge::LoadFixedROM(uint8_t* memoryBus){
    memcpy(memoryBus, romBanks.data(), 0x4000);
}

void Cartridge::BankSwitching(uint16_t a, uint8_t data){
    pMBC->BankSwitching(a, data);
}
