//
//  PPU.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 15/09/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//
#pragma once

#include <vector>
#include <array>
#include "PaletteManager.hpp"

class Bus;

class PPU{
public:
    PPU();
    ~PPU();
    
    void AdvanceClocks(uint8_t clocks);
    
    bool LCDEnabled();
    
    void ToggleLCD(bool turn_on);
    
    uint8_t  screen_buffer[160*144*3];
    uint16_t clock_counter = 0;
    
    void ConnectBus(Bus* b){
        bus = b;
    }
    
    uint8_t* GetBuffer(){
        return screen_buffer;
    }
    
private:
    
    static const uint16_t screen_height = 144;
    static const uint16_t screen_width  = 160;
    
    const uint16_t oam_length      =  80;
    const uint16_t draw_length     = 172;
    const uint16_t hblank_length   = 204;
    const uint16_t scanline_length = 456;
    const uint16_t oam_base        = 0xFE00;

    uint8_t  current_scanline;
    uint8_t  coincidence_flag;
    uint8_t  current_mode;
    uint8_t  status;
    uint8_t  lcdc;
    
    bool     mode_change;
    bool     req_int;
    
    Bus* bus = nullptr;
    
private:
    uint8_t read(uint16_t a);
    void    write(uint16_t a, uint8_t d);
    // Direct access to memory, address validity up to game dev
    
    void SetLCDStatus();
    void RenderScanline();
    
    void RequestInterrupt(uint8_t b);
    void CheckForStatModeIntReq();
    void CheckForCoincidenceIntReq();
    
    void SetStatMode(uint8_t mode);
    
    void PerformOAMScan();
    void RenderBackground();
    void RenderSprites();
    void MixAndPush();
    
    void AddToBuffer(uint16_t lcd_idx, uint8_t colour);
 
private:
    uint8_t bg_line_buf[screen_width];
    uint8_t sprite_line_buf[screen_width];
    
    struct WINDOW{
        uint16_t map_base;
        uint16_t map_y_offset;
        uint16_t tile_address;
        uint16_t line_count = 0; // internal line counter
        uint8_t  pixel_col; // map relative pixel position
        uint8_t  pixel_row;
        uint8_t  tile_id;
        uint8_t  x; // map relative pixel position
        uint8_t  y;
        uint8_t  sub_col; // tile relative pixel position
        uint8_t  sub_row;
        
        bool visible;
        bool on_this_line;
    } window;
    
    struct BACKGROUND{
        uint16_t map_base;
        uint16_t map_y_offset;
        uint8_t  tile_id;
        uint8_t  pixel_col;
        uint8_t  pixel_row;
        uint8_t  sub_col;
        uint8_t  sub_row;
        
    } background;
    
    struct SPRITE{
        int16_t  xpos;
        int16_t  ypos;
        uint16_t mempos;
        uint8_t  tile_id;
        
        bool palette_num;
        bool xflip;
        bool yflip;
        bool bg_priority;
    };
    
    std::vector<SPRITE> sprite_list;
    
    static bool ComparePriority(const SPRITE &a, const SPRITE &b){
        if(a.xpos == b.xpos){
            return (a.mempos > b.mempos); // memory priority
        }else{
            return (a.xpos > b.xpos); // last in line first
        }
    }
};






