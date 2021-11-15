//
//  GameBoy.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 16/09/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//
#pragma once
#include "Bus.hpp"

class GameBoy{
public:
    GameBoy();
    ~GameBoy();

    void TurnOn();
    
private:
    void Run();

    void StartFPSTimer();
    void LimitFPS();
    void CheckFPS();
    
private:
    CPU cpu;
    PPU ppu;
    APU apu;
    Bus bus;
    
    BasicIO io;
    
    Cartridge* cart;
    
private:
    std::string ROM;
    
    const float FPS = 59.73;
    const int   CLOCKS_PER_FRAME = cpu.CLOCKSPEED/FPS;
    const int   TARGET_FRAME_DURATION = 1000/FPS;
    
    uint32_t clock_count = 0;
    uint8_t  clocks;
    
    uint64_t then;
    uint64_t now;
    int64_t  diff;
    
    uint64_t fps_duration = 0;
    uint64_t fps_count = 0;
    uint64_t fps_then;
    uint64_t fps_now;
};
