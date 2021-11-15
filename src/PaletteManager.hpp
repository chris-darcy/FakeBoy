//
//  PaletteManager.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 06/11/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#ifndef PaletteManager_hpp
#define PaletteManager_hpp

#include <stdio.h>
#include <vector>
#include <array>
#include <fstream>

class PaletteManager {
    
public:
    PaletteManager();
    ~PaletteManager();
    
    static void LoadPalettes();
    static void CyclePalettes();
    
public:
    struct COLOUR{
        COLOUR(uint8_t red, uint8_t green, uint8_t blue);
        COLOUR(uint32_t hex_code);
        
        uint8_t r; uint8_t g; uint8_t b;
    };
    
    struct PALETTE{
        PALETTE(const char* name, COLOUR a, COLOUR b, COLOUR c, COLOUR d);
        
        const char* name;
        std::array<COLOUR,4> colours;
    };
    
    static PALETTE current_palette;
    
private:
    
    static std::vector<PALETTE> palettes;
    static int palette_idx;
};

#endif /* PaletteManager_hpp */
