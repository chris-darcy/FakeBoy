//
//  BasicIO.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 20/09/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#ifndef BasicIO_hpp
#define BasicIO_hpp

#define GL_SILENCE_DEPRECATION

#include <GLFW/glfw3.h>
#include <portaudio.h>
#include <stdio.h>

class BasicIO{
public:
    BasicIO();
    BasicIO(int width, int height, const char* title, int scale);
    ~BasicIO(); 
    
public:
    void LinkScreenBuffer(uint8_t* screen_buffer){
        this->screen_buffer = screen_buffer;
    }
    
    void LinkAudioBuffer(float* audio_buffer){
        this->audio_buffer = audio_buffer;
    }
    
    uint8_t GetJoypadInput(uint8_t &current_key_states);
    bool ShouldTerminate();
    void Terminate();
    bool Create();
    void DrawScreen();
    void ResetScreenBuffer();
    void ResetAudioBuffer();
    void SetScreenTitle(const char* title);
    
private:
    
    
private:
    const int WIDTH = 160;
    const int HEIGHT = 144;
    const int scl = 4;
    const char* title = "BasicIO";
    
    GLFWwindow* window;
    
    uint8_t*   screen_buffer;
    float*     audio_buffer;
    static int current_audio_bank;
    uint8_t    controller_key_states;
    
    bool       quit_prevHeld = false;
    bool       theme_change_prevHeld = false;
    
    bool       quit = false;
};

#endif /* BasicIO_hpp */
