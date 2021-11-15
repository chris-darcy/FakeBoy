//
//  APU.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 27/10/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "APU.hpp"
#include "Bus.hpp"
#include "Utils.hpp"
#include <math.h>

APU::APU()
:buffer_idx(0)
{
    
    channels[0] = &ch1;
    channels[1] = &ch2;
    channels[2] = &ch3;
    channels[3] = &ch4;
    
    DUTY_TABLE = {
        0b00000001, // 12.5 %
        0b00000011, // 25.0 %
        0b00001111, // 50.0 %
        0b11111100  // 75.0 %
    };
    
    ch2.a_reg_sound_len   = NR21;
    ch2.a_reg_freq_lo     = NR23;
    ch2.a_reg_freq_hi     = NR24;
    
}

APU::~APU(){}

void APU::AdvanceClocks(uint8_t clocks){
    clock_counter += clocks;
    
    if(!APUEnabled()){
        return;
    }
    
    // CHECK TRIGGER EVENTS
    if(ch2.initial_flag){
        ch2.on = true;
        ch2.env.set_params(read(NR22));
        if(ch2.sound_length_counter == 0){ ch2.sound_length_counter = 64; }
        
    }
    
    // FRAME_SEQUENCER
    fseq.timer_counter += clocks;
    if(fseq.timer_counter >= fseq.timer_length){
        fseq.timer_counter -= fseq.timer_counter;
        
        fseq.step = ( fseq.step + 1 ) & 0b111;
        
        switch (fseq.step) {
            case 0:  ch2.step_length_func(); break;
            case 1:  break;
            case 2:  ch2.step_length_func(); break;
            case 3:  break;
            case 4:  ch2.step_length_func(); break;
            case 5:  break;
            case 6:  ch2.step_length_func(); break;
            case 7:  ch2.env.step(); break;
        }
    }
    
    // CHANNEL 1
    
    // CHANNEL 2
    if(ch2.on){
        ch2.freq_timer_counter += clocks;
        if( ch2.freq_timer_counter >= ch2.freq_timer_length ){
            ch2.freq_timer_counter -= ch2.freq_timer_length;
            
            ch2.freq              = ( ( read(NR24) & 0b111 ) << 8 ) | read(NR23);
            ch2.freq_timer_length = ( 2048 - ch2.freq ) << 2;
            
            ch2.duty_pos          = ( ch2.duty_pos + 1 ) & 0b111;
            ch2.duty    = (read(NR21) & 0b11000000) >> 6;
            ch2.amp     = DUTY_TABLE.at(ch2.duty) & ( 1 << ch2.duty_pos );
            ch2.dac_out = ( ( ch2.amp * ch2.env.vol ) / 7.5f ) - 1.0f;
        }
    }
    
    // CHANNEL 3
    
    // CHANNEL 4
    
    // FINAL MIX AND PUSH TO BUFFER
    sample_gen_counter += clocks;
    if(sample_gen_counter >= sample_gen_length){
        sample_gen_counter -= sample_gen_length;
        
        left_out = right_out = 0;
        
        // Panning
        uint8_t SO1 = (read(NR50) & 0b00000011);
        uint8_t SO2 = (read(NR50) & 0b01100000) >> 5;
        
        float lc = 0, rc = 0;
        uint8_t channel_panning = read(NR51);
        for(uint8_t channel = 0; channel < 4; channel++){
            uint8_t config = (channel_panning >> 0) & 0b00010001;
            switch(config){
                case  1: left_out  += channels[channel]->dac_out; lc++; break;
                case 16: right_out += channels[channel]->dac_out; rc++; break;
                case 17:
                {
                         left_out  += channels[channel]->dac_out; lc++;
                         right_out += channels[channel]->dac_out; rc++;
                } break;
                default: break;
            };
        }
        
        // Mixing
        left_out  = ch2.dac_out;// /= lc;
        right_out = ch2.dac_out;// /= rc;
        
        // LR Volume Adjustment
        //left_out  *= (float)SO1/7;
        //right_out *= (float)SO2/7;
        
        // Put Samples In Buffer;
        //float temp = (float) sin( ((double)buffer_idx/(double)buffer_length) * 3.1459 * 2. );
        audio_buffer[buffer_idx++] = left_out;
        audio_buffer[buffer_idx++] = right_out;
        
        if(buffer_idx >= buffer_length){
            buffer_idx = 0;
        }
    }
    
    uint8_t nr52 = SetBit(read(NR52), 1, ch2.on);
    write(NR52,nr52);
}

inline uint8_t APU::read(uint16_t a){
    return bus->memoryBus[a];
}

inline void    APU::write(uint16_t a, uint8_t data){
    bus->memoryBus[a] = data;
}

void APU::ENVELOPE::step(){
    
    if(!this->enabled)
        return;
    
    this->timer_counter++;
    if(this->timer_counter == this->timer_length){
        this->timer_counter = 0;
        this->vol += ( this->vol == this->target_vol ) ? 0:( this->rise ? 1:-1 );
    }
}

void APU::ENVELOPE::set_params(uint8_t reg){
    
    this->initial_vol   = (reg & 0b11110000) >> 4;
    this->rise          =  reg & 0b00001000;
    this->timer_length  =  reg & 0b00000111;
    this->target_vol    =  this->rise ? 7:0;
    this->enabled       =  this->timer_length;
    this->timer_counter = 0;
}

void APU::CHANNEL::step_length_func(){
    if(this->length_func_enabled){
        this->sound_length_counter--;
        if(this->sound_length_counter == 0){
            this->on = false;
        }
    }
}

void APU::UpdateEnvelopeChannel(uint8_t channel_num, uint16_t a){

    if(channel_num == 1){
        ch1.env.set_params(read(NR12));
    }else{
        ch2.env.set_params(read(NR22));
    }
}

void APU::RestartChannel(uint8_t channel_num, uint16_t a){
    channels[channel_num-1]->initial_flag = read(a) & 0b10000000;
    channels[channel_num-1]->length_func_enabled = read(a) & 0b01000000;
}

inline bool APU::APUEnabled(){
    return read(NR52) & 0b10000000;
}
