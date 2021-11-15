//
//  Bus.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 18/07/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "Bus.hpp"
#include "Utils.hpp"
#include "stdio.h"
#include "Debugger.hpp"

Bus::Bus(){
    memset(&memoryBus,0,sizeof(memoryBus));
    EnableBootROM();
}

Bus::~Bus(){
    
}

uint8_t Bus::cpuRead(uint16_t a, bool readOnly){
    if ((a < 0x0100) && bootROM_enabled){
        return bootROM[a];
    }
    // Cartridge ROM Switchable Banks
    if ((a >= 0x4000) && (a < 0x8000)){
        return cartridge->readROM(a - 0x4000);
    }
    // Cartridge RAM Switchable Banks
    else if ((a >= 0xA000) && (a < 0xC000)){
        return cartridge->readRAM(a - 0xA000);
    }
    // Unusable RAM
    else if ((a >= 0xFEA0) && (a < 0xFEFF)){
        return 0;
    }
    else if (a == P1){
        return (memoryBus[a] = io->GetJoypadInput(memoryBus[a]));
    }
    else{
        return memoryBus[a];
    }
}

void Bus::cpuWrite(uint16_t a, uint8_t data){
    // dont allow any writing to the read only memory
    if (a < 0x8000){
        cartridge->BankSwitching(a, data);
    }
    else if ((a >= 0xA000) && (a < 0xC000)){
        cartridge->writeRAM(a - 0xA000, data);
    }
    // writing to ECHO ram also writes in RAM
    else if ((a >= 0xE000) && (a < 0xFE00)){
        memoryBus[a] = data ;
        cpuWrite(a-0x2000, data) ;
    }
    // this area is restricted
    else if ((a >= 0xFEA0 ) && (a < 0xFEFF)){
    }
    // IO registers
    else if ((a >= 0xFF00 ) && (a <= 0xFFFF)){
        switch(a){
            case P1:
            {
                // lower nibble is read only, top 2 bits unused
                memoryBus[a] &= 0xCF;
                memoryBus[a] |= (data & 0x30);
            } break;
                
            case DIV:
            {
                memoryBus[a] = 0;
            }break;
                
            case TAC:
            {
                memoryBus[a] = data;
                cpu->UpdateTimerFreq();
            } break;
                
            case NR22:
            {
                memoryBus[a] = data;
                apu->UpdateEnvelopeChannel(2, a);
            } break;
                
            case NR24:
            {
                memoryBus[a] = data;
                apu->RestartChannel(2, a);
            } break;
                
            case NR52:
            {
                data &= 0x80;
                memoryBus[a] &= 0x0F;
                memoryBus[a] |= data;
                
            } break;
                
            case LCDC:
            {
                if((data & 0b10000000) != (memoryBus[a] & 0b10000000)){
                    ppu->ToggleLCD(data & 0b10000000);
                }
                memoryBus[a] = data;
            } break;
                
            case STAT:
            {
                memoryBus[a] &= 0b00000111;
                memoryBus[a] |= (data & 0b01111000);
                memoryBus[a] |= 0b10000000;
            } break;
                
            case LY:
            {
                memoryBus[a] = 0;
            } break;
                
            case DMA:
            {
                cpu->RunDMA(data);
            } break;
                
            case BANK:
            {
                DisableBootROM();
            } break;
                
            default:
            {
                memoryBus[a] = data;
            }
        }
        return;
    }
    // no control needed over this area so write to memory
    else{
        memoryBus[a] = data ;
    }
}

void Bus::EnableBootROM(){
    
    FILE *in;
    in = fopen("../res/dmg_boot.bin", "rb" );
    
    if(in != nullptr){
        fread(bootROM, 1, 0x100, in);
    }
    fclose(in);
    
}

void Bus::DisableBootROM(){
    bootROM_enabled = false;
    
    memoryBus[P1  ] = 0xCF;
    memoryBus[NR10] = 0x80;
    memoryBus[NR11] = 0xBF;
    memoryBus[NR12] = 0xF3;
    memoryBus[NR14] = 0xBF;
    memoryBus[NR21] = 0x3F;
    memoryBus[NR24] = 0xBF;
    memoryBus[NR30] = 0x7F;
    memoryBus[NR31] = 0xFF;
    memoryBus[NR32] = 0x9F;
    memoryBus[NR34] = 0xBF;
    memoryBus[NR41] = 0xFF;
    memoryBus[NR44] = 0xBF;
    memoryBus[NR50] = 0x77;
    memoryBus[NR51] = 0xF3;
    memoryBus[NR52] = 0xF1;
    memoryBus[LCDC] = 0x91;
    memoryBus[BGP ] = 0xFC;
    memoryBus[OBP0] = 0xFF;
    memoryBus[OBP1] = 0xFF;
    
    cpu->BootStrapRegisters();
}
