//
//  APU.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 27/10/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#ifndef APU_hpp
#define APU_hpp
#include <stdint.h>
#include <vector>

class Bus;

class APU{
public:
    APU();
    ~APU();
    
    void ConnectBus(Bus* b){
        bus = b;
    }
    
    float* GetBuffer(){
        return audio_buffer;
    }
    
    void AdvanceClocks(uint8_t clocks);
    void UpdateEnvelopeChannel(uint8_t channel_num, uint16_t a);
    void RestartChannel(uint8_t channel_num, uint16_t a);
    
private:
    
    uint8_t read(uint16_t a);
    void    write(uint16_t a, uint8_t data);
    
    bool APUEnabled();
    
private:
    
    struct FRAME_SEQUENCER {
        const uint16_t timer_length = 8192;
        uint16_t timer_counter;
        uint8_t  step;
    };
    FRAME_SEQUENCER fseq;
    
    struct SWEEP {
        
    };
    
    struct ENVELOPE {
        
        void step();
        void set_params(uint8_t reg);
        
        uint8_t vol;
        
        private:
            uint8_t initial_vol;
            uint8_t target_vol;
            uint8_t timer_length;
            uint8_t timer_counter;
            bool    enabled;
            bool    rise;
        
    };
    
    struct CHANNEL {
        void step_length_func();
        
        uint8_t  sound_length_counter;
        uint8_t  sound_length;
        uint16_t freq;
        uint16_t freq_timer_length;
        uint16_t freq_timer_counter;
        uint16_t a_reg_sound_len;
        uint16_t a_reg_freq_lo;
        uint16_t a_reg_freq_hi;
        
        bool     length_func_enabled;
        bool     initial_flag;
        bool     on;
        bool     amp;
        float    dac_out;
        
    };
    
    struct QUAD_CHANNEL : CHANNEL{
        uint8_t  duty;
        uint8_t  duty_pos;
        
        ENVELOPE env;
        SWEEP    sweep;
    };
    QUAD_CHANNEL ch1, ch2;
    
    struct WAV_CHANNEL : CHANNEL{
        uint8_t output_level_select;
    };
    WAV_CHANNEL ch3;
    
    struct NOISE_CHANNEL : CHANNEL{
        
    };
    NOISE_CHANNEL ch4;
    
    CHANNEL* channels[4];
    
private:
    uint16_t clock_counter;
    
    const uint8_t sample_gen_length = 95;
    uint8_t sample_gen_counter;
    
    float left_out;
    float right_out;
    
    float pleb;
    
    int    buffer_idx;
    static const int buffer_length = 2 * 64 * 3;
    float  audio_buffer[buffer_length];
    
    
    std::vector<uint8_t> DUTY_TABLE;
    
    Bus* bus = nullptr;
    
};

#endif /* APU_hpp */
