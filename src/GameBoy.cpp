//
//  GameBoy.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 16/09/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "GameBoy.hpp"
#include <chrono>
#include <thread>
#include "Debugger.hpp"
#include "PaletteManager.hpp"

GameBoy::GameBoy()
:ROM("ROMS/Super Mario Land (World) (Rev 1).gb")
{
    PaletteManager::LoadPalettes();
    
    cart = new Cartridge(ROM);

    if(!cart->valid){
        return;
    }

    bus.ConnectCPU(&cpu);
    bus.ConnectPPU(&ppu);
    bus.ConnectAPU(&apu);
    bus.ConnectIO(&io);
    bus.ConnectCart(cart);

    // remove for classic GB boot sequence 
    bus.DisableBootROM();
}

GameBoy::~GameBoy(){
    delete cart;
}

void GameBoy::TurnOn(){

    if(!cart->valid){
        printf("Unable to load ROM.\nMake sure a valid ROM is placed in the ROM folder\n");
        return;
    }

    Run();

}

void GameBoy::Run(){
    
    io.Create();
    
    StartFPSTimer();
    
    while(!io.ShouldTerminate()){
        
        while(clock_count < CLOCKS_PER_FRAME){
            
            clocks = cpu.Clock();
            ppu.AdvanceClocks(clocks);
            apu.AdvanceClocks(clocks);
            clock_count += clocks;
            
        }
        clock_count -= CLOCKS_PER_FRAME;
        
        io.DrawScreen();
        
        LimitFPS();
        CheckFPS();
    }
    
}

inline void GameBoy::StartFPSTimer(){
    then = fps_then = TARGET_FRAME_DURATION + std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

void GameBoy::LimitFPS(){
    
    now = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    diff = TARGET_FRAME_DURATION - (now - then);
    
    if(now-then < TARGET_FRAME_DURATION){
        std::this_thread::sleep_for(std::chrono::milliseconds(diff));
    }
    then = now + diff;
    
}

void GameBoy::CheckFPS(){

    fps_now = std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    fps_duration += (fps_now - fps_then);
    fps_count++;
    fps_then = then;
    
    if(fps_duration >= 250){
        std::string title = cart->rom_name;
        title += " FPS: ";
        title += std::to_string((fps_count*1000)/fps_duration);
        io.SetScreenTitle(title.c_str());
        fps_count = 0;
        fps_duration = 0;
    }
    
}


