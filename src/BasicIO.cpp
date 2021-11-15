//
//  BasicIO.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 20/09/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "BasicIO.hpp"
//#include <iostream>
#include "PaletteManager.hpp"

BasicIO::BasicIO()
{
}

BasicIO::BasicIO(int width, int height, const char* title, int scale)
:WIDTH(width),HEIGHT(height),title(title),scl(scale)
{
}

BasicIO::~BasicIO(){
    Terminate();
}

bool BasicIO::Create(){
    
    // GLFW WINDOW SETUP
    if(!glfwInit()){
        printf( "Couldn't init GLFW\n" );
        return 1;
    }
    
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    window = glfwCreateWindow( WIDTH*scl, HEIGHT*scl, title, NULL, NULL );
    if(!window){
        printf( "Couldn't create window\n" );
        return 1;
    }
    glfwMakeContextCurrent( this->window );
    ResetScreenBuffer();
    
    // GLFW KEYBOARD INPUT SETUP
    glfwSetInputMode( window, GLFW_STICKY_KEYS, GLFW_TRUE );
    
    return 0;
}

bool BasicIO::ShouldTerminate(){
    return glfwWindowShouldClose( window ) || quit;
}

void BasicIO::Terminate(){
    glfwTerminate();
}

void BasicIO::DrawScreen(){
    glfwMakeContextCurrent( this->window );
    
    glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
    
    glLoadIdentity();
    glRasterPos2i( -1, 1 );
    glPixelZoom( scl*2, -scl*2 );
    
    glDrawPixels( WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, screen_buffer );
    
    glfwSwapBuffers(window);
    
    glfwPollEvents();
}

uint8_t BasicIO::GetJoypadInput(uint8_t &current_controller_key_states){
    
    bool select_action    = (~current_controller_key_states) & 0b00100000;
    bool select_direction = (~current_controller_key_states) & 0b00010000;
    
    bool quit_Held          = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;
    bool theme_change_Held  = glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS;
    
    if(quit_prevHeld && !quit_Held){ quit = true; }
    if(theme_change_prevHeld && !theme_change_Held){ PaletteManager::CyclePalettes();}
    
    quit_prevHeld = quit_Held;
    theme_change_prevHeld = theme_change_Held;
    
    controller_key_states = 0;
    uint8_t key_start  = (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) << 3; // Start
    uint8_t key_select = (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) << 2; // Select
    uint8_t key_B      = (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) << 1; // B
    uint8_t key_A      = (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS);      // A
    uint8_t key_down   = (glfwGetKey(window, GLFW_KEY_DOWN)  == GLFW_PRESS) << 3; // Down
    uint8_t key_up     = (glfwGetKey(window, GLFW_KEY_UP)    == GLFW_PRESS) << 2; // Up
    uint8_t key_left   = (glfwGetKey(window, GLFW_KEY_LEFT)  == GLFW_PRESS) << 1; // Left
    uint8_t key_right  = (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);      // Right
    
    if(select_action){
        controller_key_states = key_start | key_select | key_B    | key_A;
    }else if(select_direction){
        controller_key_states = key_down  | key_up     | key_left | key_right;
    }
    
    controller_key_states  = (~controller_key_states) & 0x0F;
    controller_key_states |= (current_controller_key_states & 0xF0);
    
    return controller_key_states;
}

void BasicIO::ResetScreenBuffer(){
    memset(screen_buffer,0xFF,WIDTH*HEIGHT*3*sizeof(uint8_t));
}

void BasicIO::SetScreenTitle(const char* title){
    glfwSetWindowTitle(window, title);
}

int BasicIO::current_audio_bank = 0;






