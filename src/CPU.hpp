//
//  CPU.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 18/07/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#pragma once

#include <map>
#include <vector>
#include <string>

class Bus;

class CPU{
    
public:
    CPU();
    ~CPU();
    
    void  ConnectBus(Bus* b){
        bus = b;
    }
    
    uint8_t Clock();
    
    void BootStrapRegisters();
    void UpdateTimerFreq();
    void RunDMA(uint8_t data);
    
    uint32_t const CLOCKSPEED = 4194304;
    
private:
    
    // flag masks
    enum FLAGS{
        c = (1 << 4),    // Carry Flag
        h = (1 << 5),    // Half Carry Flag
        n = (1 << 6),    // Subtraction Flag
        z = (1 << 7),    // Zero Flag
    };
    
    // bit masks
    enum BIT{
        b0 = (1 << 0),
        b1 = (1 << 1),
        b2 = (1 << 2),
        b3 = (1 << 3),
        b4 = (1 << 4),
        b5 = (1 << 5),
        b6 = (1 << 6),
        b7 = (1 << 7),
    };
    
    void FetchNextOpcode();
    void ExecuteCurrentOpcode();
    void PushAddressToStack(uint16_t &a);
    
    uint8_t GetFlag(FLAGS f);
    void    SetFlag(FLAGS f, bool v);
    
    uint8_t fastRead(uint16_t a);
    void    fastWrite(uint16_t a, uint8_t data);
    // Direct access to memory, reserved for explicit IO registers
    
    uint8_t read(uint16_t a);
    void    write(uint16_t a, uint8_t d);
    // Issued through Bus class and strictly controlled
    
private:
    
    // ========Interupt Routines & Timers==========
    void ServiceIME();
    void ServiceInterrupts();
    void RequestInterrupt(uint8_t b);
    void EnableInterrupt(uint8_t b);
    
    void UpdateTimer(uint8_t cycles);
    bool TimerEnabled();
    
private:
    
    Bus* bus = nullptr;
    
    uint32_t clock_count = 0;
    
    uint8_t regs[8];
    
    uint8_t* const A_reg = &regs[0];
    uint8_t* const F_reg = &regs[1];
    uint8_t* const B_reg = &regs[2];
    uint8_t* const C_reg = &regs[3];
    uint8_t* const D_reg = &regs[4];
    uint8_t* const E_reg = &regs[5];
    uint8_t* const H_reg = &regs[6];
    uint8_t* const L_reg = &regs[7];
    
    uint16_t sp = 0x0000;
    uint16_t pc = 0x0000;
    
private:
    
    std::vector<uint8_t*>  arg_buf;
    
    uint8_t* reg_ptr;
    
    uint16_t addr_abs = 0x0000;
    uint16_t im_16    = 0x0000;
    uint16_t temp_16  = 0x0000;
    uint16_t opcode   = 0x00;
    uint8_t  im_8     = 0x00;
    uint8_t  temp_8   = 0x00;
    uint8_t  clocks   = 0;
    
    const uint16_t div_rate = 256;
    uint16_t div_counter = 0;
    uint16_t timer_rate = 1024;
    uint16_t timer_counter = 0;
    
    BIT      b;
    bool     cc;
    
    uint8_t schedule_IME;
    bool IME;
    bool halt_bug;
    bool halted = false;
    
    struct INSTRUCTION{
        void (CPU::*operate )(void) = nullptr;
        void (CPU::*arg1)(void)     = nullptr;
        void (CPU::*arg2)(void)     = nullptr;
        std::string op_name;
        uint8_t clocks = 0;
    };

    std::vector<INSTRUCTION> lookup;
    
private:
    
    // =====Arguments======
    void A();      void B();      void C();      void D();        void E();     void H();      void L();
    void aIM();    void aC();     void aBC();    void aDE();      void aHL();
    void AF();     void BC();     void DE();     void HL();
    void IM();     void IM16();   void XX();
    void B0();     void B1();     void B2();     void B3();       void B4();    void B5();     void B6();     void B7();
    void Z_cc();   void C_cc();   void NZ_cc();  void NC_cc();
    
private:
    
    // =====Operations======
    
    // 8bit loads
    void LD();     void LD_ar();  void LD_rnn(); void LD_nnr();
    void LDI_ra(); void LDI_ar(); void LDD_ra(); void LDD_ar();
    
    //16bit loads
    void LD16();   void PUSH();   void POP(); void POPAF(); void LDSP_nn(); void LDSP_a(); void LDIMSP();
    
    //8bit arithmetic/logic
    void ADD();    void ADC();    void SUB();
    void SBC();    void AND();    void XOR();
    void OR();     void CP();     void DAA();   void CPL();
    void INC_r();  void INC_a();  void DEC_r(); void DEC_a();
    
    //16bit arithemtic/logic
    void ADD16();  void ADDSP();  void ADDSP_dd(); void INC16(); void INCSP();
    void DEC16();  void DECSP();  void LDHL_dd();
    
    
    //Rotate and shift
    void RLCA();   void RLA();    void RRCA();  void RRA();
    void RLC_r();  void RLC_a();  void RL_r();  void RL_a();
    void RRC_r();  void RRC_a();  void RR_r();  void RR_a();
    void SLA_r();  void SLA_a();  void SWAP_r();void SWAP_a();
    void SRA_r();  void SRA_a();  void SRL_r(); void SRL_a();
    
    //Single bit operations
    void BIT_r();  void BIT_a();  void SET_r(); void SET_a();
    void RES_r();  void RES_a();
    
    //CPU control
    void CCF();    void SCF();    void NOP();
    void HALT();   void STOP();   void DI();
    void EI();
    
    //Jump instructions
    void JP();     void JPF();    void JR();
    void JRF();    void CALL();   void CALLF();
    void RET();    void RETF();   void RETI();
    void RST();
    
    void XXX();

};



