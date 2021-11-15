//
//  Registers.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 16/09/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

//#pragma once

#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

enum IO_REG{
    P1   = 0xFF00,SB   = 0xFF01,SC   = 0xFF02,DIV  = 0xFF04,TIMA = 0xFF05,
    TMA  = 0xFF06,TAC  = 0xFF07,IF   = 0xFF0F,NR10 = 0xFF10,NR11 = 0xFF11,
    NR12 = 0xFF12,NR13 = 0xFF13,NR14 = 0xFF14,NR21 = 0xFF16,NR22 = 0xFF17,
    NR23 = 0xFF18,NR24 = 0xFF19,NR30 = 0xFF1A,NR31 = 0xFF1B,NR32 = 0xFF1C,
    NR33 = 0xFF1D,NR34 = 0xFF1E,NR41 = 0xFF20,NR42 = 0xFF21,NR43 = 0xFF22,
    NR44 = 0xFF23,NR50 = 0xFF24,NR51 = 0xFF25,NR52 = 0xFF26,LCDC = 0xFF40,
    STAT = 0xFF41,SCY  = 0xFF42,SCX  = 0xFF43,LY   = 0xFF44,LYC  = 0xFF45,
    DMA  = 0xFF46,BGP  = 0xFF47,OBP0 = 0xFF48,OBP1 = 0xFF49,BANK = 0xFF50,
    WY   = 0xFF4A,WX   = 0xFF4B,IE   = 0xFFFF
};

bool TestBit(uint8_t data, uint8_t b);
uint8_t SetBit(uint8_t data, uint8_t b, bool v);

#endif
