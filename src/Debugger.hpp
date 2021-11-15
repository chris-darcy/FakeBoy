//
//  Debugger.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 25/09/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#ifndef Debugger_hpp
#define Debugger_hpp

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <fstream>
#include "BasicIO.hpp"
#include "Bus.hpp" // REMOVE LATER!!!


class Debugger{
public:
    
    ~Debugger();
    
    static Debugger* GetInstance();
    void LogMessage(const char* message);
    void LogCPUExecution(uint16_t &pc,Bus* bus, uint16_t &opcode,const char* op_name, uint8_t* regs, uint16_t &sp, uint32_t &clocks) const;
    void LogPPUExecution(uint8_t LY, int16_t scanline_counter, uint8_t current_mode, uint8_t status, uint8_t lcdc, uint32_t &clocks) const;
    
    void CreateEpicLog(uint16_t &pc,uint8_t* mem, uint8_t* regs, uint16_t &sp, uint32_t &clocks) const;
    void CreateDiffFile();
    
    void DrawBGMap(uint8_t* memory);
    
    void CreateMemoryDump(uint8_t* memory);
    
    std::string STATUS;
    
private:
    Debugger();
    static Debugger* instance;
 
private:
    BasicIO* debug_screen;
    uint8_t screen_buffer[256*256*3];
    
private:
    const char* test_file_name;
    
    FILE* general_log_file;
    FILE* cpu_log_file;
    FILE* ppu_log_file;
    
    FILE* epic_log_file;
    FILE* diff_file;
    FILE* memory_dump_file;

};

#endif /* Debugger_hpp */
