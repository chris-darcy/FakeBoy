//
//  Debugger.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 25/09/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "Debugger.hpp"

#include <iostream>

Debugger* Debugger::instance = 0;

Debugger::Debugger()
:general_log_file(NULL),test_file_name("Blargg11.txt"),cpu_log_file(NULL),ppu_log_file(NULL),diff_file(NULL),epic_log_file(NULL),memory_dump_file(NULL),debug_screen(0)
{
    if(std::remove("../DebugFiles/Message.txt")){}
    if(std::remove("../DebugFiles/cpu.log")){}
    if(std::remove("../DebugFiles/ppu.log")){}
    if(std::remove("../DebugFiles/diff.txt")){}
    if(std::remove("../DebugFiles/MyEpicLog.txt")){}
    if(std::remove("../DebugFiles/MemoryDump.txt")){}
    
    general_log_file = fopen("../DebugFiles/Message.txt", "w");
    
    cpu_log_file = fopen("../DebugFiles/cpu.log", "w");
    
    ppu_log_file = fopen("../DebugFiles/ppu.log", "w");
    
    diff_file = fopen("../DebugFiles/diff.txt", "w");
    
    epic_log_file = fopen("../DebugFiles/MyEpicLog.txt", "w");
    
    memory_dump_file = fopen("../DebugFiles/MemoryDump.txt", "w");
}

Debugger::~Debugger(){
    
    if(general_log_file != nullptr){
        fclose(general_log_file);
    }
    if(cpu_log_file != nullptr){
        fclose(cpu_log_file);
    }
    if(ppu_log_file != nullptr){
        fclose(ppu_log_file);
    }
    if(diff_file != nullptr){
        fclose(diff_file);
    }
    if(epic_log_file != nullptr){
        fclose(epic_log_file);
    }
    if(memory_dump_file != nullptr){
        fclose(memory_dump_file);
    }
}

Debugger* Debugger::GetInstance(){
    if(instance == nullptr){
        instance = new Debugger();
    }

    return instance;
}

void   Debugger::LogCPUExecution(uint16_t &pc,Bus* bus, uint16_t &opcode,const char* op_name, uint8_t* regs, uint16_t &sp, uint32_t &clocks) const{
    
    if (cpu_log_file != nullptr){
        char message[200] ;
        sprintf(message, "PC : %04X (%02X %02X %02X %02X) [OP : %04X ] -> | %14s |  <A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X>  [SP : %04X (%02X %02X %02X)]  | %u \n",pc,bus->cpuRead(pc),bus->cpuRead(pc+1),bus->cpuRead(pc+2),bus->cpuRead(pc+3), opcode, op_name, *regs,*(regs+1),*(regs+2),*(regs+3),*(regs+4),*(regs+5),*(regs+6),*(regs+7), sp,bus->cpuRead(sp),bus->cpuRead(sp+1),bus->cpuRead(sp+2), clocks);
        
        fputs(message, cpu_log_file) ;
    }
    
    //CreateEpicLog(pc, mem, regs, sp, clocks);
    
}

void   Debugger::LogPPUExecution(uint8_t LY, int16_t scanline_counter, uint8_t current_mode, uint8_t status, uint8_t lcdc, uint32_t &clocks) const{
    if (ppu_log_file != nullptr){
        char message[200];
        sprintf(message,"STATUS: %02X | LCDC: %02X | LY: %3u | scanline_counter: %5i | Mode: %u | %u \n",status,lcdc, LY, scanline_counter, current_mode, clocks );
        fputs(message, ppu_log_file) ;
    }
}


void   Debugger::CreateEpicLog(uint16_t &pc,uint8_t* mem, uint8_t* regs, uint16_t &sp, uint32_t &clocks) const{
    
    if (epic_log_file != nullptr){
        char message[108] ;
        sprintf(message, "A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X SP: %04X PC: 00:%04X (%02X %02X %02X %02X)\n", *(regs), *(regs+1),*(regs+2),*(regs+3),*(regs+4),*(regs+5),*(regs+6),*(regs+7), sp, pc, *(mem+pc),*(mem+pc+1),*(mem+pc+2),*(mem+pc+3));
        
        fputs(message, epic_log_file) ;
    }
}

void Debugger::LogMessage(const char* message){
    if(general_log_file != nullptr){
        fputs(message, general_log_file);
    }
}

void Debugger::CreateDiffFile(){
    if (diff_file != nullptr){
        
        std::string my_line = "";
        std::string their_prev_line = "--begin--";
        std::string cpu_state = "--begin--";
        std::string their_line = "";
        std::ifstream their_log(test_file_name);
        std::ifstream my_log("MyEpicLog.txt");
        std::ifstream my_states("cpu.log");

        
        while(std::getline(their_log,their_line) && std::getline(my_log,my_line)){
            
            if(their_line != my_line){
                their_prev_line.append(" | ");
                their_prev_line.append(cpu_state);
                their_prev_line.append("\n");
                
                fputs(their_prev_line.c_str(), diff_file) ;
                
                std::getline(my_states,cpu_state);
                
                their_line.append(" | ");
                their_line.append(cpu_state);
                their_line.append("\n");
                
                fputs(their_line.c_str(), diff_file) ;
                
                return;
            }
            their_prev_line = their_line;
            std::getline(my_states,cpu_state);
        }
    }
}

void Debugger::CreateMemoryDump(uint8_t* memory){

    if (memory_dump_file != nullptr){
        printf("creating memory dump \n");
        
        std::string line;
        char byte[4];
        char address[9];
        int byte_count = 0;
        
        sprintf(address,"%04X | ",0);
        line.append(address);
        for(uint32_t a = 0x0; a <= 0xFFFF; a++){
            
            sprintf(byte,"%02X ",(uint8_t) *(memory + a));
            line.append(byte);
            
            byte_count++;
            
            if(byte_count == 16){
                
                line.append("\n");
                fputs(line.c_str(), memory_dump_file);
                line.clear();
                
                sprintf(address,"%04X | ",a+1);
                line.append(address);
                byte_count = 0;
            }
        }
        fputs("\n", memory_dump_file);
    }
}

void Debugger::DrawBGMap(uint8_t *memory){
    if(debug_screen == nullptr){
        debug_screen = new BasicIO(256,256,"Background Map",2);
        debug_screen->LinkScreenBuffer(screen_buffer);
        debug_screen->ResetScreenBuffer();
        debug_screen->Create();
        printf("created \n");
    }
    
    //debug_screen->Reset();
    
    uint8_t lo, hi;
    bool unsig = true;
    
    uint8_t scx = *(memory + 0xFF43);
    uint8_t scy = *(memory + 0xFF42);
    uint8_t abs_x, abs_y;
    
    uint16_t data_area;
    uint16_t map_area  = ((*(memory + 0xFF40) & 0b1000) == 0)?0x9800:0x9C00;

    
    if((*(memory + 0xFF40) & 0b10000) == 0){
        data_area = 0x8800;
        unsig = false;
    }else{
        data_area = 0x8000;
    }
    
    uint16_t tile_id;
    uint16_t tile_start_address;
    
    uint8_t colour_id;
    
    uint8_t r,g,b;
    
    int idx;
    
    for(int row = 0; row < 32; row++){
        for(int col = 0; col < 32; col++){
            tile_id = *(memory + map_area + col + (row*32));
            
            if(unsig){
                tile_start_address = data_area + (tile_id*16);
            }else{
                int16_t loc = ((int16_t)tile_id) + 128;
                tile_start_address = data_area + loc*16;
            }

            for(int line = 0; line < 8; line++){
                
                abs_y =  line + row*8;
                
                lo = *(memory + tile_start_address + line*2);
                hi = *(memory + tile_start_address + line*2 + 1);
                
                for (int pixel = 0; pixel < 8; pixel++){
                    idx = (pixel + col*8 + (line*32*8) + (row*32*8*8)) * 3;
                    
                    abs_x =  pixel + col*8;
                    
                    uint8_t colour_bit = (7 - (pixel % 8));
                    uint8_t colour_bit_mask = 1 << colour_bit;
                    uint8_t colour_id = (((hi & colour_bit_mask) >> (colour_bit)) << 1) | ((lo & colour_bit_mask) >> colour_bit);
                    
                    if(!(pixel%8) || !(line%8)){
                        colour_id = 1;
                    }
                    
                    if( abs_x == scx || abs_x == ((scx + 160)&255) ){
                        if((abs_y > scy) && (abs_y < (scy + 144))){
                            colour_id = 3;
                        }
                    }
                    if( abs_y == scy || abs_y == ((scy + 144)&255) ){
                        if((abs_x > scx) && (abs_x < (scx + 160))){
                            colour_id = 3;
                        }
                    }
                    
                    switch (colour_id) {
                        case  0:r = 255;g = 255;b = 255; break;
                        case  1:r = 128;g = 128;b = 128; break;
                        case  2:r =  64;g =  64;b =  64; break;
                        case  3:r =   0;g =   0;b =   0; break;
                        default:r = 255;g = 255;b = 255; break;
                    }
                    
                    screen_buffer[idx++] = r;
                    screen_buffer[idx++] = g;
                    screen_buffer[idx++] = b;
                }
            }
        }
    }
    debug_screen->DrawScreen();
    //printf("attempting to draw screen now \n");
    
}

