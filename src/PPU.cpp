//
//  PPU.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 15/09/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "PPU.hpp"
#include "Bus.hpp"
#include "Utils.hpp"
#include "Debugger.hpp"
#include <algorithm>

PPU::PPU()
:current_scanline(0),lcdc(0),status(0)
{
    sprite_list.reserve(10);
}

PPU::~PPU(){}

void PPU::AdvanceClocks(uint8_t clocks){
    
    lcdc = read(LCDC);
    status = read(STAT);
    
    if(!LCDEnabled()){
        SetStatMode(0);
        clock_counter = 0;
        window.line_count = 0;
        current_scanline = 0;
        write(LY,0);
        
        return;
    }
    
    
    current_scanline = read(LY);
    current_mode = status & 0b11;
    
    clock_counter += clocks;
    
    switch (current_mode) {
        case 2:
            if(clock_counter >= oam_length){
                
                SetStatMode(3);
                PerformOAMScan();
                
                clock_counter -= oam_length;
            }
            break;
            
        case 3:
            if(clock_counter >= draw_length){
                RenderScanline();
                SetStatMode(0);
                
                clock_counter -= draw_length;
            }
            break;
            
        case 0:
            if(clock_counter >= hblank_length){
                current_scanline++;
                write(LY,current_scanline);
                CheckForCoincidenceIntReq();
                
                if(current_scanline == screen_height){
                    RequestInterrupt(0);
                    SetStatMode(1);
                }else{
                    SetStatMode(2);
                }
                
                clock_counter -= hblank_length;
            }
            break;
            
        case 1:
            if(clock_counter >= scanline_length){
                if(current_scanline < 153){
                    current_scanline++;
                    
                }else{
                    current_scanline = 0;
                    window.line_count = 0;
                    window.visible = false;
                    SetStatMode(2);
                }
                
                CheckForCoincidenceIntReq();
                write(LY,current_scanline);
                clock_counter -= scanline_length;
            }
            break;
            
    }
    write(STAT,status);
}

void PPU::RenderScanline(){
    
    if(TestBit(lcdc, 0)){
        RenderBackground();
    }
    if(TestBit(lcdc, 1)){
        RenderSprites();
    }
    
    MixAndPush();
}

void PPU::RenderBackground(){
    uint16_t lcd_idx = 0;
    uint16_t tile_data_base = 0;
    uint16_t tile_map_address = 0;
    uint16_t tile_data_address = 0;
    
    uint8_t  colour, colour_id, colour_bit, colour_bit_mask;
    uint8_t  lo = 0, hi = 0;
    uint8_t  bgp = read(BGP);
    uint8_t  scx = read(SCX);
    uint8_t  scy = read(SCY);
    uint8_t  ly  = read(LY);
    uint8_t  lx  = 0;
    uint8_t  wx  = 0;
    
    window.x  = read(WX) - 7;
    window.y  = read(WY);
    
    bool unsigned_addressing = true;
    
    if(TestBit(lcdc, 4))
    {
        tile_data_base = 0x8000;
    }
    else
    {
        tile_data_base      = 0x8800;
        unsigned_addressing = false;
    }
    
    window.visible = (TestBit(lcdc, 5)) && (window.x < screen_width) && (window.y < screen_height)? true:false;
    window.on_this_line = window.visible && (ly >= window.y);
    
    if(window.on_this_line)
    {
        window.map_base     = (TestBit(lcdc, 6) == 0)? 0x9800:0x9C00;
        window.map_y_offset = window.map_base + ((window.line_count >> 3) * 32); // (wlc/8)*32
        window.pixel_row    = window.line_count;
        window.pixel_col    = 0;
        window.sub_row      = (window.pixel_row % 8) * 2;
    }
    
    background.pixel_row    = scy + ly;
    background.pixel_col    = scx;
    background.map_base     = (TestBit(lcdc, 3) == 0)? 0x9800:0x9C00;
    background.map_y_offset = background.map_base + ((background.pixel_row >> 3) * 32); // (tile_row/8)*32
    background.sub_row      = (background.pixel_row % 8) * 2;


    while(lx < screen_width)
    {
        
        lcd_idx = lx + (ly * screen_width);
        
        // === WINDOW RENDERING ===
        if(window.on_this_line && (lx >= window.x))
        {
            tile_map_address = window.map_y_offset + (wx >> 3);
            window.tile_id = read(tile_map_address);
            
            if(unsigned_addressing)
            {
                tile_data_address = tile_data_base + (window.tile_id << 4); //(id * 16)
            }
            else
            {
                int16_t loc = ((int8_t)window.tile_id) + 128;
                tile_data_address = tile_data_base + (loc << 4);
            }
            
            tile_data_address += window.sub_row;
            
            lo = read(tile_data_address++);
            hi = read(tile_data_address);
            
            colour_bit = (7 - (wx % 8));
            colour_bit_mask = 1 << colour_bit;
            colour_id = (((hi & colour_bit_mask) >> (colour_bit)) << 1) | ((lo & colour_bit_mask) >> colour_bit);
            colour = (bgp & (0b11 << (colour_id * 2))) >> (colour_id * 2);
            
            bg_line_buf[lx] = colour;
            
            wx++;

        }
        
        // === BACKGROUND RENDERING ===
        else if (TestBit(lcdc, 0)){
            tile_map_address = background.map_y_offset + (background.pixel_col >> 3);
            background.tile_id = read(tile_map_address);
        
            if(unsigned_addressing)
            {
                tile_data_address = tile_data_base + (background.tile_id << 4); //(id * 16)
            }
            else
            {
                int16_t loc = ((int8_t)background.tile_id) + 128;
                tile_data_address = tile_data_base + (loc << 4);
            }
        
            tile_data_address += background.sub_row;
        
            lo = read(tile_data_address++);
            hi = read(tile_data_address);
        
            colour_bit = (7 - ((lx+scx) % 8));
            colour_bit_mask = 1 << colour_bit;
            colour_id = (((hi & colour_bit_mask) >> (colour_bit)) << 1) | ((lo & colour_bit_mask) >> colour_bit);
            colour = (bgp & (0b11 << (colour_id * 2))) >> (colour_id * 2);
        
            
            bg_line_buf[lx] = colour;
            
            background.pixel_col++;
        
        }
        else{
            bg_line_buf[lx] = 0;
        }
        
        lx++;
        
    }
    
    window.line_count += window.on_this_line;
    
}

void PPU::PerformOAMScan(){
    uint8_t oam_id      = 0;
    uint8_t sprite_ypos = 0;
    uint8_t sprite_xpos = 0;
    uint8_t mempos      = 0;
    uint8_t tile_id     = 0;
    uint8_t attributes  = 0;
    uint8_t sprite_height = TestBit(lcdc, 2)? 16:8; //  sprites height 8x8 or 8x16
    uint8_t mask = (TestBit(lcdc, 2)?0xFE:0xFF);
    
    bool pal_num, xflip, yflip, bg_priority;
    
    sprite_list.clear();
    for(uint8_t i = 0; i < 40; i++){
        
        // first of each 4 byte oam entry
        oam_id = (i<<2); // i*2;
        sprite_ypos = read(oam_base + oam_id) - 16;
        
        if((current_scanline >= sprite_ypos) && (current_scanline < (sprite_ypos + sprite_height))){
            sprite_xpos = read(oam_base + oam_id + 1) - 8;
            tile_id     = read(oam_base + oam_id + 2) & mask;
            attributes  = read(oam_base + oam_id + 3);
            
            mempos      = i;
            pal_num     = attributes & 0b00010000;
            xflip       = attributes & 0b00100000;
            yflip       = attributes & 0b01000000;
            bg_priority = attributes & 0b10000000;
            
            
            // add to visible sprites this line
            SPRITE s = {sprite_xpos,sprite_ypos,mempos,tile_id,pal_num,xflip,yflip,bg_priority};
            sprite_list.emplace_back(s);
        }
        
        if(sprite_list.size() >= 10){
            break;
        }
    }
    std::sort(sprite_list.begin(), sprite_list.end(), ComparePriority);
    
}

void PPU::RenderSprites(){
    
    uint16_t tile_data_base = 0x8000;
    uint16_t tile_data_address = 0;
    uint8_t  ly = read(LY);
    uint8_t  lx = 0;
    uint8_t  sprite_height = 8;
    uint8_t  lo, hi;
    uint8_t  colour, colour_id, colour_bit, colour_bit_mask;
    uint8_t  sub_row = 0;
    uint8_t  sub_col = 0;
    
    if(sprite_list.size() == 0){
        return;
    }
    
    if(TestBit(lcdc, 2)){ //  sprites height 8x8 or 8x16
        sprite_height = 16;
    }
    
    for(auto sprite : sprite_list){
        
        if((0 <= sprite.xpos + 8) && (sprite.xpos < screen_width)){
            
            sub_row = (ly - sprite.ypos);
        
            if(sprite.yflip){
                sub_row = ((sprite_height - 1) - sub_row);
            }
            
            tile_data_address = tile_data_base + (sprite.tile_id << 4); // (id * 16)
            tile_data_address += (sub_row << 1); //(row * 2)
            
            lo = read(tile_data_address++);
            hi = read(tile_data_address);
            
            for(uint8_t i = 0; i < 8; i++){
                
                sub_col = i;
                
                if(sprite.xflip){
                    sub_col = (7 - sub_col);
                }
                
                colour_bit = (7 - sub_col);
                colour_bit_mask = 1 << colour_bit;
                colour_id = ((hi & colour_bit_mask)? 2:0) + ((lo & colour_bit_mask)? 1:0);
                colour = (read(OBP0 + sprite.palette_num) & (0b11 << (colour_id * 2))) >> (colour_id * 2);
                
                lx = sprite.xpos + i;
                
                if(colour_id == 0 || (sprite.bg_priority && bg_line_buf[lx]) || lx >= screen_width){
                    continue;
                }
                
                sprite_line_buf[lx] = colour + 1; // to distinguish transparent from "white"
            }
        }
    }
}

void PPU::MixAndPush(){
    if(current_scanline > 50 && current_scanline < 90){
        
    }
    
    uint16_t offset = current_scanline * screen_width;
    uint8_t  final_colour;
    
    for (uint8_t i = 0; i < screen_width; i++) {
        final_colour = sprite_line_buf[i] > 0 ? (sprite_line_buf[i]-1):bg_line_buf[i]; // remove sprite colour offset
        AddToBuffer(offset + i, final_colour);
        
        sprite_line_buf[i] = 0;
        bg_line_buf[i]     = 0;
    }
}

inline uint8_t PPU::read(uint16_t a){
    return bus->memoryBus[a];
}

inline void PPU::write(uint16_t a, uint8_t data){
    bus->memoryBus[a] = data;
}

inline bool PPU::LCDEnabled(){
    return TestBit(lcdc, 7);
}

inline void PPU::RequestInterrupt(uint8_t b){
    uint8_t reg = read(IF);
    reg |= (1<<b);
    write(IF,reg);
}

void PPU::CheckForStatModeIntReq(){
    req_int =    (TestBit(status, 5) && current_mode==2 )
              || (TestBit(status, 4) && current_mode==1 )
              || (TestBit(status, 3) && current_mode==0 );
    
    if(req_int){
        RequestInterrupt(1);
    }
}

void PPU::CheckForCoincidenceIntReq(){
    coincidence_flag = (current_scanline == read(LYC))? true:false;
    status = SetBit(status, 2, coincidence_flag);
    
    if(TestBit(status, 6) && coincidence_flag){
        RequestInterrupt(1);
    }
}

void PPU::SetStatMode(uint8_t mode){
    mode_change = current_mode != mode;
    current_mode = mode;
    
    CheckForStatModeIntReq();
    
    status &= ~0b11;
    status |= mode;
    write(STAT,status);
}

void PPU::ToggleLCD(bool turn_on){
    if(turn_on){
        current_scanline = 0;
        write(LY,current_scanline);
        
        SetStatMode(2);
    }else{
        clock_counter = 0;
        SetStatMode(0);
    }
}
                      
void PPU::AddToBuffer(uint16_t lcd_idx, uint8_t colour){
    uint32_t idx = lcd_idx * 3;
    screen_buffer[idx++] = PaletteManager::current_palette.colours[colour].r;
    screen_buffer[idx++] = PaletteManager::current_palette.colours[colour].g;
    screen_buffer[idx++] = PaletteManager::current_palette.colours[colour].b;
}


