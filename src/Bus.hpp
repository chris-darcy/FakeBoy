//
//  Bus.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 18/07/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#ifndef Bus_hpp
#define Bus_hpp

#include "CPU.hpp"
#include "PPU.hpp"
#include "APU.hpp"
#include "Cartridge.hpp"
#include "BasicIO.hpp"


class Bus{
    
public:
    Bus();
    ~Bus();
    
public:
    uint8_t memoryBus[0x10000];
    uint8_t bootROM[0x100];
    
    CPU* cpu;
    PPU* ppu;
    APU* apu;
    Cartridge* cartridge;
    BasicIO* io;
    
public:
    bool bootROM_enabled = true;
    
    void EnableBootROM();
    void DisableBootROM();
    
public:
    
    uint8_t cpuRead(uint16_t a, bool readOnly = false);
    void cpuWrite(uint16_t a, uint8_t data);
    
    void ConnectCPU(CPU* cpu){
        this->cpu = cpu;
        cpu->ConnectBus(this);
    }
    
    void ConnectCart(Cartridge* cartridge){
        this->cartridge = cartridge;
        cartridge->ConnectBus(this);
        cartridge->LoadFixedROM(memoryBus);
    }
    
    void ConnectPPU(PPU* ppu){
        this->ppu = ppu;
        ppu->ConnectBus(this);
        
        if(io != nullptr){
            io->LinkScreenBuffer(ppu->GetBuffer());
        }
    }
    
    void ConnectAPU(APU* apu){
        this->apu = apu;
        apu->ConnectBus(this);
        
        if(io != nullptr){
            io->LinkAudioBuffer(apu->GetBuffer());
        }
    }
    
    void ConnectIO(BasicIO* io){
        this->io = io;
        
        if(ppu != nullptr){
            io->LinkScreenBuffer(ppu->GetBuffer());
        }
        
        if(apu != nullptr){
            io->LinkAudioBuffer(apu->GetBuffer());
        }
    }
};

#endif /* Bus_hpp */