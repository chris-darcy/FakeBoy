//
//  PaletteManager.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 06/11/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "PaletteManager.hpp"
#include <stdlib.h>

PaletteManager::PaletteManager(){
    
}

PaletteManager::~PaletteManager(){
    
}

std::vector<PaletteManager::PALETTE> PaletteManager::palettes = {
    PaletteManager::PALETTE("NORMAL",
                            COLOUR(0xFFFFFF),
                            COLOUR(0xBFBFBF),
                            COLOUR(0x808080),
                            COLOUR(0x000000))
    
};
PaletteManager::PALETTE PaletteManager::current_palette = PaletteManager::palettes[0];

int PaletteManager::palette_idx = 0;

void PaletteManager::LoadPalettes(){
    
    
    uint32_t hex_codes[4];
    
    std::ifstream ifs("res/dmg_palettes.txt",std::fstream::in);
    
    if(ifs.is_open()){
        while(!ifs.eof()){
            
            char* name = new char[16];
            ifs.getline(name,12,'\r');
        
            for(int i=0; i < 4; i++){
                ifs >> std::hex >> hex_codes[i];
            }
            PaletteManager::palettes.emplace_back(PALETTE(name, COLOUR(hex_codes[3]), COLOUR(hex_codes[2]), COLOUR(hex_codes[1]), COLOUR(hex_codes[0])));
            
            // flush stray characters
            while(ifs.peek() == '\n'|| ifs.peek() == '\r'){
                ifs.get();
            }
            
        }
    }
    ifs.close();
}

void PaletteManager::CyclePalettes(){
    
    palette_idx++;
    if(palette_idx == palettes.size()){
        palette_idx = 0;
    }
    
    current_palette = palettes[palette_idx];
}

PaletteManager::COLOUR::COLOUR(uint8_t red, uint8_t green, uint8_t blue)
: r(red), g(green), b(blue)
{}

PaletteManager::COLOUR::COLOUR(uint32_t hex_code){
    b = (hex_code         ) & 0xFF;
    g = (hex_code >> 1 * 8) & 0xFF;
    r = (hex_code >> 2 * 8) & 0xFF;
}

PaletteManager::PALETTE::PALETTE(const char* name,COLOUR a, COLOUR b, COLOUR c, COLOUR d)
:colours({a,b,c,d})
{
    this->name = name;
}
