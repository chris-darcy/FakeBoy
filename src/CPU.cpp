//
//  CPU.cpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 18/07/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#include "CPU.hpp"
#include "Bus.hpp"
#include "Utils.hpp"
#include "array"
#include "Debugger.hpp"

CPU::CPU(){
    
    arg_buf.reserve(2);
    
    using a = CPU;
    lookup =
    
    {
        {&a::NOP   ,&a::XX   ,&a::XX ,"NOP"        , 4},{&a::LD16   ,&a::BC ,&a::IM16,"LD BC, d16",12},{&a::LD_ar ,&a::BC   ,&a::A ,"LD (BC), A" , 8},{&a::INC16 ,&a::BC ,&a::XX,"INC BC"    , 8},{&a::INC_r ,&a::B    ,&a::XX ,"INC B"       , 4},{&a::DEC_r ,&a::B  ,&a::XX ,"DEC B"     , 4},{&a::LD    ,&a::B  ,&a::IM ,"LD B, d8"   , 8},{&a::RLCA  ,&a::XX ,&a::XX ,"RLCA"      , 4},{&a::LDIMSP  ,&a::IM16 ,&a::XX ,"LD (a16), SP"  ,20},{&a::ADD16 ,&a::HL ,&a::BC ,"ADD HL, BC", 8},{&a::LD    ,&a::A   ,&a::aBC ,"LD A, (BC)" , 8},{&a::DEC16,&a::BC ,&a::XX ,"DEC BC"  , 8},{&a::INC_r ,&a::C   ,&a::XX ,"INC C"      , 4},{&a::DEC_r ,&a::C  ,&a::XX ,"DEC C"   , 4},{&a::LD    ,&a::C  ,&a::IM ,"LD C, d8"   , 8},{&a::RRCA  ,&a::XX ,&a::XX ,"RRCA"    , 4},
        {&a::STOP  ,&a::IM   ,&a::XX ,"STOP"       , 4},{&a::LD16   ,&a::DE ,&a::IM16,"LD DE, d16",12},{&a::LD_ar ,&a::DE   ,&a::A ,"LD (DE), A" , 8},{&a::INC16 ,&a::DE ,&a::XX,"INC DE"    , 8},{&a::INC_r ,&a::D    ,&a::XX ,"INC D"       , 4},{&a::DEC_r ,&a::D  ,&a::XX ,"DEC D"     , 4},{&a::LD    ,&a::D  ,&a::IM ,"LD D, d8"   , 8},{&a::RLA   ,&a::XX ,&a::XX ,"RLA"       , 4},{&a::JR      ,&a::XX   ,&a::XX ,"JR r8"         ,12},{&a::ADD16 ,&a::HL ,&a::DE ,"ADD HL, DE", 8},{&a::LD    ,&a::A   ,&a::aDE ,"LD A, (DE)" , 8},{&a::DEC16,&a::DE ,&a::XX ,"DEC DE"  , 8},{&a::INC_r ,&a::E   ,&a::XX ,"INC E"      , 4},{&a::DEC_r ,&a::E  ,&a::XX ,"DEC E"   , 4},{&a::LD    ,&a::E  ,&a::IM ,"LD E, d8"   , 8},{&a::RRA   ,&a::XX ,&a::XX ,"RRA"     , 4},
        {&a::JRF   ,&a::NZ_cc,&a::XX ,"JR NZ,r8"   ,12},{&a::LD16   ,&a::HL ,&a::IM16,"LD HL, d16",12},{&a::LDI_ar,&a::HL   ,&a::A ,"LD (HL+), A", 8},{&a::INC16 ,&a::HL ,&a::XX,"INC HL"    , 8},{&a::INC_r ,&a::H    ,&a::XX ,"INC H"       , 4},{&a::DEC_r ,&a::H  ,&a::XX ,"DEC H"     , 4},{&a::LD    ,&a::H  ,&a::IM ,"LD H, d8"   , 8},{&a::DAA   ,&a::XX ,&a::XX ,"DAA"       , 4},{&a::JRF     ,&a::Z_cc ,&a::XX ,"JR Z, r8"      ,12},{&a::ADD16 ,&a::HL ,&a::HL ,"ADD HL, HL", 8},{&a::LDI_ra,&a::A   ,&a::aHL ,"LD A, (HL+)", 8},{&a::DEC16,&a::HL ,&a::XX ,"DEC HL"  , 8},{&a::INC_r ,&a::L   ,&a::XX ,"INC L"      , 4},{&a::DEC_r ,&a::L  ,&a::XX ,"DEC L"   , 4},{&a::LD    ,&a::L  ,&a::IM ,"LD L, d8"   , 8},{&a::CPL   ,&a::XX ,&a::XX ,"CPL"     , 4},
        {&a::JRF   ,&a::NC_cc,&a::XX ,"JR NC,r8"   ,12},{&a::LDSP_nn,&a::XX ,&a::IM16,"LD SP, d16",12},{&a::LDD_ar,&a::HL   ,&a::A ,"LD (HL-), A", 8},{&a::INCSP ,&a::XX ,&a::XX,"INC SP"    , 8},{&a::INC_a ,&a::aHL  ,&a::XX ,"INC (HL)"    ,12},{&a::DEC_a ,&a::aHL,&a::XX ,"DEC (HL)"  ,12},{&a::LD_ar ,&a::HL ,&a::IM ,"LD (HL), d8",12},{&a::SCF   ,&a::XX ,&a::XX ,"SCF"       , 4},{&a::JRF     ,&a::C_cc ,&a::XX ,"JR C, r8"      ,12},{&a::ADDSP ,&a::XX ,&a::XX ,"ADD HL, SP", 8},{&a::LDD_ra,&a::A   ,&a::aHL ,"LD A, (HL-)", 8},{&a::DECSP,&a::XX ,&a::XX ,"DEC SP"  , 8},{&a::INC_r ,&a::A   ,&a::XX ,"INC A"      , 4},{&a::DEC_r ,&a::A  ,&a::XX ,"DEC A"   , 4},{&a::LD    ,&a::A  ,&a::IM ,"LD A, d8"   , 8},{&a::CCF   ,&a::XX ,&a::XX ,"CCF"     , 4},
        {&a::LD    ,&a::B    ,&a::B  ,"LD B,B"     , 4},{&a::LD     ,&a::B  ,&a::C   ,"LD B, C"   , 4},{&a::LD    ,&a::B    ,&a::D ,"LD B, D"    , 4},{&a::LD    ,&a::B  ,&a::E ,"LD B, E"   , 4},{&a::LD    ,&a::B    ,&a::H  ,"LD B, H"     , 4},{&a::LD    ,&a::B  ,&a::L  ,"LD B, L"   , 4},{&a::LD    ,&a::B  ,&a::aHL,"LD B, (HL)" , 8},{&a::LD    ,&a::B  ,&a::A  ,"LD B, A"   , 4},{&a::LD      ,&a::C    ,&a::B  ,"LD C, B"       , 4},{&a::LD    ,&a::C  ,&a::C  ,"LD C, C"   , 4},{&a::LD    ,&a::C   ,&a::D   ,"LD C, D"    , 4},{&a::LD   ,&a::C  ,&a::E  ,"LD C, E" , 4},{&a::LD    ,&a::C   ,&a::H  ,"LD C, H"    , 4},{&a::LD    ,&a::C  ,&a::L  ,"LD C, L" , 4},{&a::LD    ,&a::C  ,&a::aHL,"LD C, (HL)" , 8},{&a::LD    ,&a::C  ,&a::A  ,"LD C, A" , 4},
        {&a::LD    ,&a::D    ,&a::B  ,"LD D,B"     , 4},{&a::LD     ,&a::D  ,&a::C   ,"LD D, C"   , 4},{&a::LD    ,&a::D    ,&a::D ,"LD D, D"    , 4},{&a::LD    ,&a::D  ,&a::E ,"LD D, E"   , 4},{&a::LD    ,&a::D    ,&a::H  ,"LD D, H"     , 4},{&a::LD    ,&a::D  ,&a::L  ,"LD D, L"   , 4},{&a::LD    ,&a::D  ,&a::aHL,"LD D, (HL)" , 8},{&a::LD    ,&a::D  ,&a::A  ,"LD D, A"   , 4},{&a::LD      ,&a::E    ,&a::B  ,"LD E, B"       , 4},{&a::LD    ,&a::E  ,&a::C  ,"LD E, C"   , 4},{&a::LD    ,&a::E   ,&a::D   ,"LD E, D"    , 4},{&a::LD   ,&a::E  ,&a::E  ,"LD E, E" , 4},{&a::LD    ,&a::E   ,&a::H  ,"LD E, H"    , 4},{&a::LD    ,&a::E  ,&a::L  ,"LD E, L" , 4},{&a::LD    ,&a::E  ,&a::aHL,"LD E, (HL)" , 8},{&a::LD    ,&a::E  ,&a::A  ,"LD E, A" , 4},
        {&a::LD    ,&a::H    ,&a::B  ,"LD H,B"     , 4},{&a::LD     ,&a::H  ,&a::C   ,"LD H, C"   , 4},{&a::LD    ,&a::H    ,&a::D ,"LD H, D"    , 4},{&a::LD    ,&a::H  ,&a::E ,"LD H, E"   , 4},{&a::LD    ,&a::H    ,&a::H  ,"LD H, H"     , 4},{&a::LD    ,&a::H  ,&a::L  ,"LD H, L"   , 4},{&a::LD    ,&a::H  ,&a::aHL,"LD H, (HL)" , 8},{&a::LD    ,&a::H  ,&a::A  ,"LD H, A"   , 4},{&a::LD      ,&a::L    ,&a::B  ,"LD L, B"       , 4},{&a::LD    ,&a::L  ,&a::C  ,"LD L, C"   , 4},{&a::LD    ,&a::L   ,&a::D   ,"LD L, D"    , 4},{&a::LD   ,&a::L  ,&a::E  ,"LD L, E" , 4},{&a::LD    ,&a::L   ,&a::H  ,"LD L, H"    , 4},{&a::LD    ,&a::L  ,&a::L  ,"LD L, L" , 4},{&a::LD    ,&a::L  ,&a::aHL,"LD L, (HL)" , 8},{&a::LD    ,&a::L  ,&a::A  ,"LD L, A" , 4},
        {&a::LD_ar ,&a::HL   ,&a::B  ,"LD (HL),B"  , 8},{&a::LD_ar  ,&a::HL ,&a::C   ,"LD (HL), C", 8},{&a::LD_ar ,&a::HL   ,&a::D ,"LD (HL), D" , 8},{&a::LD_ar ,&a::HL ,&a::E ,"LD (HL), E", 8},{&a::LD_ar ,&a::HL   ,&a::H  ,"LD (HL), H"  , 8},{&a::LD_ar ,&a::HL ,&a::L  ,"LD (HL), L", 8},{&a::HALT  ,&a::XX ,&a::XX ,"HALT"       , 4},{&a::LD_ar ,&a::HL ,&a::A  ,"LD (HL), A", 8},{&a::LD      ,&a::A    ,&a::B  ,"LD A, B"       , 4},{&a::LD    ,&a::A  ,&a::C  ,"LD A, C"   , 4},{&a::LD    ,&a::A   ,&a::D   ,"LD A, D"    , 4},{&a::LD   ,&a::A  ,&a::E  ,"LD A, E" , 4},{&a::LD    ,&a::A   ,&a::H  ,"LD A, H"    , 4},{&a::LD    ,&a::A  ,&a::L  ,"LD A, L" , 4},{&a::LD    ,&a::A  ,&a::aHL,"LD A, (HL)" , 8},{&a::LD    ,&a::A  ,&a::A  ,"LD A, A" , 4},
        {&a::ADD   ,&a::A    ,&a::B  ,"ADD A, B"   , 4},{&a::ADD    ,&a::A  ,&a::C   ,"ADD A, C"  , 4},{&a::ADD   ,&a::A    ,&a::D ,"ADD A, D"   , 4},{&a::ADD   ,&a::A  ,&a::E ,"ADD A, E"  , 4},{&a::ADD   ,&a::A    ,&a::H  ,"ADD A, H"    , 4},{&a::ADD   ,&a::A  ,&a::L  ,"ADD A, L"  , 4},{&a::ADD   ,&a::A  ,&a::aHL,"ADD A, (HL)", 8},{&a::ADD   ,&a::A  ,&a::A  ,"ADD A, A"  , 4},{&a::ADC     ,&a::A    ,&a::B  ,"ADC A, B"      , 4},{&a::ADC   ,&a::A  ,&a::C  ,"ADC A, C"  , 4},{&a::ADC   ,&a::A   ,&a::D   ,"ADC A, D"   , 4},{&a::ADC  ,&a::A  ,&a::E  ,"ADC A, E", 4},{&a::ADC   ,&a::A   ,&a::H  ,"ADC A, H"   , 4},{&a::ADC   ,&a::A  ,&a::L  ,"ADC A, L", 4},{&a::ADC   ,&a::A  ,&a::aHL,"ADC A, (HL)", 8},{&a::ADC   ,&a::A  ,&a::A  ,"ADC A, A", 4},
        {&a::SUB   ,&a::A    ,&a::B  ,"SUB B"      , 4},{&a::SUB    ,&a::A  ,&a::C   ,"SUB C"     , 4},{&a::SUB   ,&a::A    ,&a::D ,"SUB D"      , 4},{&a::SUB   ,&a::A  ,&a::E ,"SUB E"     , 4},{&a::SUB   ,&a::A    ,&a::H  ,"SUB H"       , 4},{&a::SUB   ,&a::A  ,&a::L  ,"SUB L"     , 4},{&a::SUB   ,&a::A  ,&a::aHL,"SUB (HL)"   , 8},{&a::SUB   ,&a::A  ,&a::A  ,"SUB A"     , 4},{&a::SBC     ,&a::A    ,&a::B  ,"SBC A, B"      , 4},{&a::SBC   ,&a::A  ,&a::C  ,"SBC A, C"  , 4},{&a::SBC   ,&a::A   ,&a::D   ,"SBC A, D"   , 4},{&a::SBC  ,&a::A  ,&a::E  ,"SBC A, E", 4},{&a::SBC   ,&a::A   ,&a::H  ,"SBC A, H"   , 4},{&a::SBC   ,&a::A  ,&a::L  ,"SBC A, L", 4},{&a::SBC   ,&a::A  ,&a::aHL,"SBC A, (HL)", 8},{&a::SBC   ,&a::A  ,&a::A  ,"SBC A, A", 4},
        {&a::AND   ,&a::A    ,&a::B  ,"AND B"      , 4},{&a::AND    ,&a::A  ,&a::C   ,"AND C"     , 4},{&a::AND   ,&a::A    ,&a::D ,"AND D"      , 4},{&a::AND   ,&a::A  ,&a::E ,"AND E"     , 4},{&a::AND   ,&a::A    ,&a::H  ,"AND H"       , 4},{&a::AND   ,&a::A  ,&a::L  ,"AND L"     , 4},{&a::AND   ,&a::A  ,&a::aHL,"AND (HL)"   , 8},{&a::AND   ,&a::A  ,&a::A  ,"AND A"     , 4},{&a::XOR     ,&a::A    ,&a::B  ,"XOR B"         , 4},{&a::XOR   ,&a::A  ,&a::C  ,"XOR C"     , 4},{&a::XOR   ,&a::A   ,&a::D   ,"XOR D"      , 4},{&a::XOR  ,&a::A  ,&a::E  ,"XOR E"   , 4},{&a::XOR   ,&a::A   ,&a::H  ,"XOR H"      , 4},{&a::XOR   ,&a::A  ,&a::L  ,"XOR L"   , 4},{&a::XOR   ,&a::A  ,&a::aHL,"XOR (HL)"   , 8},{&a::XOR   ,&a::A  ,&a::A  ,"XOR A"   , 4},
        {&a::OR    ,&a::A    ,&a::B  ,"OR B"       , 4},{&a::OR     ,&a::A  ,&a::C   ,"OR C"      , 4},{&a::OR    ,&a::A    ,&a::D ,"OR D"       , 4},{&a::OR    ,&a::A  ,&a::E ,"OR E"      , 4},{&a::OR    ,&a::A    ,&a::H  ,"OR H"        , 4},{&a::OR    ,&a::A  ,&a::L  ,"OR L"      , 4},{&a::OR    ,&a::A  ,&a::aHL,"OR (HL)"    , 8},{&a::OR    ,&a::A  ,&a::A  ,"OR A"      , 4},{&a::CP      ,&a::A    ,&a::B  ,"CP B"          , 4},{&a::CP    ,&a::A  ,&a::C  ,"CP C"      , 4},{&a::CP    ,&a::A   ,&a::D   ,"CP D"       , 4},{&a::CP   ,&a::A  ,&a::E  ,"CP E"    , 4},{&a::CP    ,&a::A   ,&a::H  ,"CP H"       , 4},{&a::CP    ,&a::A  ,&a::L  ,"CP L"    , 4},{&a::CP    ,&a::A  ,&a::aHL,"CP (HL)"    , 8},{&a::CP    ,&a::A  ,&a::A  ,"CP A"    , 4},
        {&a::RETF  ,&a::NZ_cc,&a::XX ,"RET NZ"     ,20},{&a::POP    ,&a::BC ,&a::XX  ,"POP BC"    ,12},{&a::JPF   ,&a::NZ_cc,&a::XX,"JP NZ, a16" ,16},{&a::JP    ,&a::XX ,&a::XX,"JP a16"    ,16},{&a::CALLF ,&a::NZ_cc,&a::XX ,"CALL NZ, a16",24},{&a::PUSH  ,&a::BC ,&a::XX ,"PUSH BC"   ,16},{&a::ADD   ,&a::A  ,&a::IM ,"ADD A, d8"  , 8},{&a::RST   ,&a::XX ,&a::XX ,"RST 00H"   ,16},{&a::RETF    ,&a::Z_cc ,&a::XX ,"RET Z"         ,20},{&a::RET   ,&a::XX ,&a::XX ,"RET"       ,16},{&a::JPF   ,&a::Z_cc,&a::XX  ,"JP Z, a16"  ,16},{&a::XXX  ,&a::XX ,&a::XX ,"PREFIX"  , 4},{&a::CALLF ,&a::Z_cc,&a::XX ,"CALL Z, a16",24},{&a::CALL  ,&a::XX ,&a::XX ,"CALL a16",24},{&a::ADC   ,&a::A  ,&a::IM ,"ADC A, d8"  , 8},{&a::RST   ,&a::XX ,&a::XX ,"RST 08H" ,16},
        {&a::RETF  ,&a::NC_cc,&a::XX ,"RET NC"     ,20},{&a::POP    ,&a::DE ,&a::XX  ,"POP DE"    ,12},{&a::JPF   ,&a::NC_cc,&a::XX,"JP NC, a16" ,16},{&a::XXX   ,&a::XX ,&a::XX,"---"       , 0},{&a::CALLF ,&a::NC_cc,&a::XX ,"CALL NC, a16",24},{&a::PUSH  ,&a::DE ,&a::XX ,"PUSH DE"   ,16},{&a::SUB   ,&a::A  ,&a::IM ,"SUB d8"     , 8},{&a::RST   ,&a::XX ,&a::XX ,"RST 10H"   ,16},{&a::RETF    ,&a::C_cc ,&a::XX ,"RET C"         ,20},{&a::RETI  ,&a::XX ,&a::XX ,"RETI"      ,16},{&a::JPF   ,&a::C_cc,&a::XX  ,"JP C, a16"  ,16},{&a::XXX  ,&a::XX ,&a::XX ,"---"     , 0},{&a::CALLF ,&a::C_cc,&a::XX ,"CALL C, a16",24},{&a::XXX   ,&a::XX ,&a::XX ,"---"     , 0},{&a::SBC   ,&a::A  ,&a::IM ,"SBC A, d8"  , 8},{&a::RST   ,&a::XX ,&a::XX ,"RST 18H" ,16},
        {&a::LD_nnr,&a::aIM  ,&a::A  ,"LDH (a8), A",12},{&a::POP    ,&a::HL ,&a::XX  ,"POP HL"    ,12},{&a::LD_nnr,&a::aC   ,&a::A ,"LD (C), A"  , 8},{&a::XXX   ,&a::XX ,&a::XX,"---"       , 0},{&a::XXX   ,&a::XX   ,&a::XX ,"---"         , 0},{&a::PUSH  ,&a::HL ,&a::XX ,"PUSH HL"   ,16},{&a::AND   ,&a::A  ,&a::IM ,"AND d8"     , 8},{&a::RST   ,&a::XX ,&a::XX ,"RST 20H"   ,16},{&a::ADDSP_dd,&a::IM   ,&a::XX ,"ADD SP, r8"    ,16},{&a::JP    ,&a::XX ,&a::XX ,"JP HL"     , 4},{&a::LD_nnr,&a::IM16,&a::A   ,"LD (a16), A",16},{&a::XXX  ,&a::XX ,&a::XX ,"---"     , 0},{&a::XXX   ,&a::XX  ,&a::XX ,"---"        , 0},{&a::XXX   ,&a::XX ,&a::XX ,"---"     , 0},{&a::XOR   ,&a::A  ,&a::IM ,"XOR d8"     , 8},{&a::RST   ,&a::XX ,&a::XX ,"RST 28H" ,16},
        {&a::LD_rnn,&a::A    ,&a::aIM,"LDH A, (a8)",12},{&a::POPAF  ,&a::XX ,&a::XX  ,"POP AF"    ,12},{&a::LD_rnn,&a::A    ,&a::aC,"LD A, (C)"  , 8},{&a::DI    ,&a::XX ,&a::XX,"DI"        , 4},{&a::XXX   ,&a::XX   ,&a::XX ,"---"         , 0},{&a::PUSH  ,&a::AF ,&a::XX ,"PUSH AF"   ,16},{&a::OR    ,&a::A  ,&a::IM ,"OR d8"      , 8},{&a::RST   ,&a::XX ,&a::XX ,"RST 30H"   ,16},{&a::LDHL_dd ,&a::IM   ,&a::XX ,"LD HL, SP + r8",12},{&a::LDSP_a,&a::XX ,&a::XX ,"LD SP, HL" , 8},{&a::LD_rnn,&a::A   ,&a::IM16,"LD A, (a16)",16},{&a::EI   ,&a::XX ,&a::XX ,"EI"      , 4},{&a::XXX   ,&a::XX  ,&a::XX ,"---"        , 0},{&a::XXX   ,&a::XX ,&a::XX ,"---"     , 0},{&a::CP    ,&a::A  ,&a::IM ,"CP d8"      , 8},{&a::RST   ,&a::XX ,&a::XX ,"RST 38H" ,16},
        
        {&a::RLC_r ,&a::B    ,&a::XX ,"RLC B"      , 8},{&a::RLC_r  ,&a::C  ,&a::XX  ,"RLC C"     , 8},{&a::RLC_r ,&a::D    ,&a::XX,"RLC D"      , 8},{&a::RLC_r ,&a::E  ,&a::XX,"RLC E"     , 8},{&a::RLC_r ,&a::H    ,&a::XX ,"RLC H"       , 8},{&a::RLC_r ,&a::L  ,&a::XX ,"RLC L"     , 8},{&a::RLC_a ,&a::aHL,&a::XX ,"RLC (HL)"   ,16},{&a::RLC_r ,&a::A  ,&a::XX ,"RLC A"     , 8},{&a::RRC_r   ,&a::B    ,&a::XX ,"RRC B"         , 8},{&a::RRC_r ,&a::C  ,&a::XX ,"RRC C"     , 8},{&a::RRC_r ,&a::D   ,&a::XX  ,"RRC D"      , 8},{&a::RRC_r,&a::E  ,&a::XX ,"RRC E"   , 8},{&a::RRC_r ,&a::H   ,&a::XX ,"RRC H"      , 8},{&a::RRC_r ,&a::L  ,&a::XX ,"RRC L"   , 8},{&a::RRC_a ,&a::aHL,&a::XX ,"RRC (HL)"   ,16},{&a::RRC_r ,&a::A  ,&a::XX ,"RRC A"   , 8},
        {&a::RL_r  ,&a::B    ,&a::XX ,"RL B"       , 8},{&a::RL_r   ,&a::C  ,&a::XX  ,"RL C"      , 8},{&a::RL_r  ,&a::D    ,&a::XX,"RL D"       , 8},{&a::RL_r  ,&a::E  ,&a::XX,"RL E"      , 8},{&a::RL_r  ,&a::H    ,&a::XX ,"RL H"        , 8},{&a::RL_r  ,&a::L  ,&a::XX ,"RL L"      , 8},{&a::RL_a  ,&a::aHL,&a::XX ,"RL (HL)"    ,16},{&a::RL_r  ,&a::A  ,&a::XX ,"RL A"      , 8},{&a::RR_r    ,&a::B    ,&a::XX ,"RR B"          , 8},{&a::RR_r  ,&a::C  ,&a::XX ,"RR C"      , 8},{&a::RR_r  ,&a::D   ,&a::XX  ,"RR D"       , 8},{&a::RR_r ,&a::E  ,&a::XX ,"RR E"    , 8},{&a::RR_r  ,&a::H   ,&a::XX ,"RR H"       , 8},{&a::RR_r  ,&a::L  ,&a::XX ,"RR L"    , 8},{&a::RR_a  ,&a::aHL,&a::XX ,"RR (HL)"    ,16},{&a::RR_r  ,&a::A  ,&a::XX ,"RR A"    , 8},
        {&a::SLA_r ,&a::B    ,&a::XX ,"SLA B"      , 8},{&a::SLA_r  ,&a::C  ,&a::XX  ,"SLA C"     , 8},{&a::SLA_r ,&a::D    ,&a::XX,"SLA D"      , 8},{&a::SLA_r ,&a::E  ,&a::XX,"SLA E"     , 8},{&a::SLA_r ,&a::H    ,&a::XX ,"SLA H"       , 8},{&a::SLA_r ,&a::L  ,&a::XX ,"SLA L"     , 8},{&a::SLA_a ,&a::aHL,&a::XX ,"SLA (HL)"   ,16},{&a::SLA_r ,&a::A  ,&a::XX ,"SLA A"     , 8},{&a::SRA_r   ,&a::B    ,&a::XX ,"SRA B"         , 8},{&a::SRA_r ,&a::C  ,&a::XX ,"SRA C"     , 8},{&a::SRA_r ,&a::D   ,&a::XX  ,"SRA D"      , 8},{&a::SRA_r,&a::E  ,&a::XX ,"SRA E"   , 8},{&a::SRA_r ,&a::H   ,&a::XX ,"SRA H"      , 8},{&a::SRA_r ,&a::L  ,&a::XX ,"SRA L"   , 8},{&a::SRA_a ,&a::aHL,&a::XX ,"SRA (HL)"   ,16},{&a::SRA_r ,&a::A  ,&a::XX ,"SRA A"   , 8},
        {&a::SWAP_r,&a::B    ,&a::XX ,"SWAP B"     , 8},{&a::SWAP_r ,&a::C  ,&a::XX  ,"SWAP C"    , 8},{&a::SWAP_r,&a::D    ,&a::XX,"SWAP D"     , 8},{&a::SWAP_r,&a::E  ,&a::XX,"SWAP E"    , 8},{&a::SWAP_r,&a::H    ,&a::XX ,"SWAP H"      , 8},{&a::SWAP_r,&a::L  ,&a::XX ,"SWAP L"    , 8},{&a::SWAP_a,&a::aHL,&a::XX ,"SWAP (HL)"  ,16},{&a::SWAP_r,&a::A  ,&a::XX ,"SWAP A"    , 8},{&a::SRL_r   ,&a::B    ,&a::XX ,"SRL B"         , 8},{&a::SRL_r ,&a::C  ,&a::XX ,"SRL C"     , 8},{&a::SRL_r ,&a::D   ,&a::XX  ,"SRL D"      , 8},{&a::SRL_r,&a::E  ,&a::XX ,"SRL E"   , 8},{&a::SRL_r ,&a::H   ,&a::XX ,"SRL H"      , 8},{&a::SRL_r ,&a::L  ,&a::XX ,"SRL L"   , 8},{&a::SRL_a ,&a::aHL,&a::XX ,"SRL (HL)"   ,16},{&a::SRL_r ,&a::A  ,&a::XX ,"SRL A"   , 8},
        {&a::BIT_r ,&a::B0   ,&a::B  ,"BIT 0, B"   , 8},{&a::BIT_r  ,&a::B0 ,&a::C   ,"BIT 0, C"  , 8},{&a::BIT_r ,&a::B0   ,&a::D ,"BIT 0, D"   , 8},{&a::BIT_r ,&a::B0 ,&a::E ,"BIT 0, E"  , 8},{&a::BIT_r ,&a::B0   ,&a::H  ,"BIT 0, H"    , 8},{&a::BIT_r ,&a::B0 ,&a::L  ,"BIT 0, L"  , 8},{&a::BIT_a ,&a::B0 ,&a::aHL,"BIT 0, (HL)",12},{&a::BIT_r ,&a::B0 ,&a::A  ,"BIT 0, A"  , 8},{&a::BIT_r   ,&a::B1   ,&a::B  ,"BIT 1, B"      , 8},{&a::BIT_r ,&a::B1 ,&a::C  ,"BIT 1, C"  , 8},{&a::BIT_r ,&a::B1  ,&a::D   ,"BIT 1, D"   , 8},{&a::BIT_r,&a::B1 ,&a::E  ,"BIT 1, E", 8},{&a::BIT_r ,&a::B1  ,&a::H  ,"BIT 1, H"   , 8},{&a::BIT_r ,&a::B1 ,&a::L  ,"BIT 1, L", 8},{&a::BIT_a ,&a::B1 ,&a::aHL,"BIT 1, (HL)",12},{&a::BIT_r ,&a::B1 ,&a::A  ,"BIT 1, A", 8},
        {&a::BIT_r ,&a::B2   ,&a::B  ,"BIT 2, B"   , 8},{&a::BIT_r  ,&a::B2 ,&a::C   ,"BIT 2, C"  , 8},{&a::BIT_r ,&a::B2   ,&a::D ,"BIT 2, D"   , 8},{&a::BIT_r ,&a::B2 ,&a::E ,"BIT 2, E"  , 8},{&a::BIT_r ,&a::B2   ,&a::H  ,"BIT 2, H"    , 8},{&a::BIT_r ,&a::B2 ,&a::L  ,"BIT 2, L"  , 8},{&a::BIT_a ,&a::B2 ,&a::aHL,"BIT 2, (HL)",12},{&a::BIT_r ,&a::B2 ,&a::A  ,"BIT 2, A"  , 8},{&a::BIT_r   ,&a::B3   ,&a::B  ,"BIT 3, B"      , 8},{&a::BIT_r ,&a::B3 ,&a::C  ,"BIT 3, C"  , 8},{&a::BIT_r ,&a::B3  ,&a::D   ,"BIT 3, D"   , 8},{&a::BIT_r,&a::B3 ,&a::E  ,"BIT 3, E", 8},{&a::BIT_r ,&a::B3  ,&a::H  ,"BIT 3, H"   , 8},{&a::BIT_r ,&a::B3 ,&a::L  ,"BIT 3, L", 8},{&a::BIT_a ,&a::B3 ,&a::aHL,"BIT 3, (HL)",12},{&a::BIT_r ,&a::B3 ,&a::A  ,"BIT 3, A", 8},
        {&a::BIT_r ,&a::B4   ,&a::B  ,"BIT 4, B"   , 8},{&a::BIT_r  ,&a::B4 ,&a::C   ,"BIT 4, C"  , 8},{&a::BIT_r ,&a::B4   ,&a::D ,"BIT 4, D"   , 8},{&a::BIT_r ,&a::B4 ,&a::E ,"BIT 4, E"  , 8},{&a::BIT_r ,&a::B4   ,&a::H  ,"BIT 4, H"    , 8},{&a::BIT_r ,&a::B4 ,&a::L  ,"BIT 4, L"  , 8},{&a::BIT_a ,&a::B4 ,&a::aHL,"BIT 4, (HL)",12},{&a::BIT_r ,&a::B4 ,&a::A  ,"BIT 4, A"  , 8},{&a::BIT_r   ,&a::B5   ,&a::B  ,"BIT 5, B"      , 8},{&a::BIT_r ,&a::B5 ,&a::C  ,"BIT 5, C"  , 8},{&a::BIT_r ,&a::B5  ,&a::D   ,"BIT 5, D"   , 8},{&a::BIT_r,&a::B5 ,&a::E  ,"BIT 5, E", 8},{&a::BIT_r ,&a::B5  ,&a::H  ,"BIT 5, H"   , 8},{&a::BIT_r ,&a::B5 ,&a::L  ,"BIT 5, L", 8},{&a::BIT_a ,&a::B5 ,&a::aHL,"BIT 5, (HL)",12},{&a::BIT_r ,&a::B5 ,&a::A  ,"BIT 5, A", 8},
        {&a::BIT_r ,&a::B6   ,&a::B  ,"BIT 6, B"   , 8},{&a::BIT_r  ,&a::B6 ,&a::C   ,"BIT 6, C"  , 8},{&a::BIT_r ,&a::B6   ,&a::D ,"BIT 6, D"   , 8},{&a::BIT_r ,&a::B6 ,&a::E ,"BIT 6, E"  , 8},{&a::BIT_r ,&a::B6   ,&a::H  ,"BIT 6, H"    , 8},{&a::BIT_r ,&a::B6 ,&a::L  ,"BIT 6, L"  , 8},{&a::BIT_a ,&a::B6 ,&a::aHL,"BIT 6, (HL)",12},{&a::BIT_r ,&a::B6 ,&a::A  ,"BIT 6, A"  , 8},{&a::BIT_r   ,&a::B7   ,&a::B  ,"BIT 7, B"      , 8},{&a::BIT_r ,&a::B7 ,&a::C  ,"BIT 7, C"  , 8},{&a::BIT_r ,&a::B7  ,&a::D   ,"BIT 7, D"   , 8},{&a::BIT_r,&a::B7 ,&a::E  ,"BIT 7, E", 8},{&a::BIT_r ,&a::B7  ,&a::H  ,"BIT 7, H"   , 8},{&a::BIT_r ,&a::B7 ,&a::L  ,"BIT 7, L", 8},{&a::BIT_a ,&a::B7 ,&a::aHL,"BIT 7, (HL)",12},{&a::BIT_r ,&a::B7 ,&a::A  ,"BIT 7, A", 8},
        {&a::RES_r ,&a::B0   ,&a::B  ,"RES 0, B"   , 8},{&a::RES_r  ,&a::B0 ,&a::C   ,"RES 0, C"  , 8},{&a::RES_r ,&a::B0   ,&a::D ,"RES 0, D"   , 8},{&a::RES_r ,&a::B0 ,&a::E ,"RES 0, E"  , 8},{&a::RES_r ,&a::B0   ,&a::H  ,"RES 0, H"    , 8},{&a::RES_r ,&a::B0 ,&a::L  ,"RES 0, L"  , 8},{&a::RES_a ,&a::B0 ,&a::aHL,"RES 0, (HL)",16},{&a::RES_r ,&a::B0 ,&a::A  ,"RES 0, A"  , 8},{&a::RES_r   ,&a::B1   ,&a::B  ,"RES 1, B"      , 8},{&a::RES_r ,&a::B1 ,&a::C  ,"RES 1, C"  , 8},{&a::RES_r ,&a::B1  ,&a::D   ,"RES 1, D"   , 8},{&a::RES_r,&a::B1 ,&a::E  ,"RES 1, E", 8},{&a::RES_r ,&a::B1  ,&a::H  ,"RES 1, H"   , 8},{&a::RES_r ,&a::B1 ,&a::L  ,"RES 1, L", 8},{&a::RES_a ,&a::B1 ,&a::aHL,"RES 1, (HL)",16},{&a::RES_r ,&a::B1 ,&a::A  ,"RES 1, A", 8},
        {&a::RES_r ,&a::B2   ,&a::B  ,"RES 2, B"   , 8},{&a::RES_r  ,&a::B2 ,&a::C   ,"RES 2, C"  , 8},{&a::RES_r ,&a::B2   ,&a::D ,"RES 2, D"   , 8},{&a::RES_r ,&a::B2 ,&a::E ,"RES 2, E"  , 8},{&a::RES_r ,&a::B2   ,&a::H  ,"RES 2, H"    , 8},{&a::RES_r ,&a::B2 ,&a::L  ,"RES 2, L"  , 8},{&a::RES_a ,&a::B2 ,&a::aHL,"RES 2, (HL)",16},{&a::RES_r ,&a::B2 ,&a::A  ,"RES 2, A"  , 8},{&a::RES_r   ,&a::B3   ,&a::B  ,"RES 3, B"      , 8},{&a::RES_r ,&a::B3 ,&a::C  ,"RES 3, C"  , 8},{&a::RES_r ,&a::B3  ,&a::D   ,"RES 3, D"   , 8},{&a::RES_r,&a::B3 ,&a::E  ,"RES 3, E", 8},{&a::RES_r ,&a::B3  ,&a::H  ,"RES 3, H"   , 8},{&a::RES_r ,&a::B3 ,&a::L  ,"RES 3, L", 8},{&a::RES_a ,&a::B3 ,&a::aHL,"RES 3, (HL)",16},{&a::RES_r ,&a::B3 ,&a::A  ,"RES 3, A", 8},
        {&a::RES_r ,&a::B4   ,&a::B  ,"RES 4, B"   , 8},{&a::RES_r  ,&a::B4 ,&a::C   ,"RES 4, C"  , 8},{&a::RES_r ,&a::B4   ,&a::D ,"RES 4, D"   , 8},{&a::RES_r ,&a::B4 ,&a::E ,"RES 4, E"  , 8},{&a::RES_r ,&a::B4   ,&a::H  ,"RES 4, H"    , 8},{&a::RES_r ,&a::B4 ,&a::L  ,"RES 4, L"  , 8},{&a::RES_a ,&a::B4 ,&a::aHL,"RES 4, (HL)",16},{&a::RES_r ,&a::B4 ,&a::A  ,"RES 4, A"  , 8},{&a::RES_r   ,&a::B5   ,&a::B  ,"RES 5, B"      , 8},{&a::RES_r ,&a::B5 ,&a::C  ,"RES 5, C"  , 8},{&a::RES_r ,&a::B5  ,&a::D   ,"RES 5, D"   , 8},{&a::RES_r,&a::B5 ,&a::E  ,"RES 5, E", 8},{&a::RES_r ,&a::B5  ,&a::H  ,"RES 5, H"   , 8},{&a::RES_r ,&a::B5 ,&a::L  ,"RES 5, L", 8},{&a::RES_a ,&a::B5 ,&a::aHL,"RES 5, (HL)",16},{&a::RES_r ,&a::B5 ,&a::A  ,"RES 5, A", 8},
        {&a::RES_r ,&a::B6   ,&a::B  ,"RES 6, B"   , 8},{&a::RES_r  ,&a::B6 ,&a::C   ,"RES 6, C"  , 8},{&a::RES_r ,&a::B6   ,&a::D ,"RES 6, D"   , 8},{&a::RES_r ,&a::B6 ,&a::E ,"RES 6, E"  , 8},{&a::RES_r ,&a::B6   ,&a::H  ,"RES 6, H"    , 8},{&a::RES_r ,&a::B6 ,&a::L  ,"RES 6, L"  , 8},{&a::RES_a ,&a::B6 ,&a::aHL,"RES 6, (HL)",16},{&a::RES_r ,&a::B6 ,&a::A  ,"RES 6, A"  , 8},{&a::RES_r   ,&a::B7   ,&a::B  ,"RES 7, B"      , 8},{&a::RES_r ,&a::B7 ,&a::C  ,"RES 7, C"  , 8},{&a::RES_r ,&a::B7  ,&a::D   ,"RES 7, D"   , 8},{&a::RES_r,&a::B7 ,&a::E  ,"RES 7, E", 8},{&a::RES_r ,&a::B7  ,&a::H  ,"RES 7, H"   , 8},{&a::RES_r ,&a::B7 ,&a::L  ,"RES 7, L", 8},{&a::RES_a ,&a::B7 ,&a::aHL,"RES 7, (HL)",16},{&a::RES_r ,&a::B7 ,&a::A  ,"RES 7, A", 8},
        {&a::SET_r ,&a::B0   ,&a::B  ,"SET 0, B"   , 8},{&a::SET_r  ,&a::B0 ,&a::C   ,"SET 0, C"  , 8},{&a::SET_r ,&a::B0   ,&a::D ,"SET 0, D"   , 8},{&a::SET_r ,&a::B0 ,&a::E ,"SET 0, E"  , 8},{&a::SET_r ,&a::B0   ,&a::H  ,"SET 0, H"    , 8},{&a::SET_r ,&a::B0 ,&a::L  ,"SET 0, L"  , 8},{&a::SET_a ,&a::B0 ,&a::aHL,"SET 0, (HL)",16},{&a::SET_r ,&a::B0 ,&a::A  ,"SET 0, A"  , 8},{&a::SET_r   ,&a::B1   ,&a::B  ,"SET 1, B"      , 8},{&a::SET_r ,&a::B1 ,&a::C  ,"SET 1, C"  , 8},{&a::SET_r ,&a::B1  ,&a::D   ,"SET 1, D"   , 8},{&a::SET_r,&a::B1 ,&a::E  ,"SET 1, E", 8},{&a::SET_r ,&a::B1  ,&a::H  ,"SET 1, H"   , 8},{&a::SET_r ,&a::B1 ,&a::L  ,"SET 1, L", 8},{&a::SET_a ,&a::B1 ,&a::aHL,"SET 1, (HL)",16},{&a::SET_r ,&a::B1 ,&a::A  ,"SET 1, A", 8},
        {&a::SET_r ,&a::B2   ,&a::B  ,"SET 2, B"   , 8},{&a::SET_r  ,&a::B2 ,&a::C   ,"SET 2, C"  , 8},{&a::SET_r ,&a::B2   ,&a::D ,"SET 2, D"   , 8},{&a::SET_r ,&a::B2 ,&a::E ,"SET 2, E"  , 8},{&a::SET_r ,&a::B2   ,&a::H  ,"SET 2, H"    , 8},{&a::SET_r ,&a::B2 ,&a::L  ,"SET 2, L"  , 8},{&a::SET_a ,&a::B2 ,&a::aHL,"SET 2, (HL)",16},{&a::SET_r ,&a::B2 ,&a::A  ,"SET 2, A"  , 8},{&a::SET_r   ,&a::B3   ,&a::B  ,"SET 3, B"      , 8},{&a::SET_r ,&a::B3 ,&a::C  ,"SET 3, C"  , 8},{&a::SET_r ,&a::B3  ,&a::D   ,"SET 3, D"   , 8},{&a::SET_r,&a::B3 ,&a::E  ,"SET 3, E", 8},{&a::SET_r ,&a::B3  ,&a::H  ,"SET 3, H"   , 8},{&a::SET_r ,&a::B3 ,&a::L  ,"SET 3, L", 8},{&a::SET_a ,&a::B3 ,&a::aHL,"SET 3, (HL)",16},{&a::SET_r ,&a::B3 ,&a::A  ,"SET 3, A", 8},
        {&a::SET_r ,&a::B4   ,&a::B  ,"SET 4, B"   , 8},{&a::SET_r  ,&a::B4 ,&a::C   ,"SET 4, C"  , 8},{&a::SET_r ,&a::B4   ,&a::D ,"SET 4, D"   , 8},{&a::SET_r ,&a::B4 ,&a::E ,"SET 4, E"  , 8},{&a::SET_r ,&a::B4   ,&a::H  ,"SET 4, H"    , 8},{&a::SET_r ,&a::B4 ,&a::L  ,"SET 4, L"  , 8},{&a::SET_a ,&a::B4 ,&a::aHL,"SET 4, (HL)",16},{&a::SET_r ,&a::B4 ,&a::A  ,"SET 4, A"  , 8},{&a::SET_r   ,&a::B5   ,&a::B  ,"SET 5, B"      , 8},{&a::SET_r ,&a::B5 ,&a::C  ,"SET 5, C"  , 8},{&a::SET_r ,&a::B5  ,&a::D   ,"SET 5, D"   , 8},{&a::SET_r,&a::B5 ,&a::E  ,"SET 5, E", 8},{&a::SET_r ,&a::B5  ,&a::H  ,"SET 5, H"   , 8},{&a::SET_r ,&a::B5 ,&a::L  ,"SET 5, L", 8},{&a::SET_a ,&a::B5 ,&a::aHL,"SET 5, (HL)",16},{&a::SET_r ,&a::B5 ,&a::A  ,"SET 5, A", 8},
        {&a::SET_r ,&a::B6   ,&a::B  ,"SET 6, B"   , 8},{&a::SET_r  ,&a::B6 ,&a::C   ,"SET 6, C"  , 8},{&a::SET_r ,&a::B6   ,&a::D ,"SET 6, D"   , 8},{&a::SET_r ,&a::B6 ,&a::E ,"SET 6, E"  , 8},{&a::SET_r ,&a::B6   ,&a::H  ,"SET 6, H"    , 8},{&a::SET_r ,&a::B6 ,&a::L  ,"SET 6, L"  , 8},{&a::SET_a ,&a::B6 ,&a::aHL,"SET 6, (HL)",16},{&a::SET_r ,&a::B6 ,&a::A  ,"SET 6, A"  , 8},{&a::SET_r   ,&a::B7   ,&a::B  ,"SET 7, B"      , 8},{&a::SET_r ,&a::B7 ,&a::C  ,"SET 7, C"  , 8},{&a::SET_r ,&a::B7  ,&a::D   ,"SET 7, D"   , 8},{&a::SET_r,&a::B7 ,&a::E  ,"SET 7, E", 8},{&a::SET_r ,&a::B7  ,&a::H  ,"SET 7, H"   , 8},{&a::SET_r ,&a::B7 ,&a::L  ,"SET 7, L", 8},{&a::SET_a ,&a::B7 ,&a::aHL,"SET 7, (HL)",16},{&a::SET_r ,&a::B7 ,&a::A  ,"SET 7, A", 8}
    };
    
    pc = 0x0000;
}


CPU::~CPU(){
    // Destructor - has nothing to do
}

//========================= Boot ROM ====================================

void CPU::BootStrapRegisters(){
    pc = 0x0100;
    sp = 0xFFFE;
    *A_reg = 0x01;
    *F_reg = 0xB0;
    *B_reg = 0x00;
    *C_reg = 0x13;
    *D_reg = 0x00;
    *E_reg = 0xD8;
    *H_reg = 0x01;
    *L_reg = 0x4D;
}

//========================= Helper Functions ============================

uint8_t CPU::Clock(){
        
        if (!halted){
            
            FetchNextOpcode();
                
            if (halt_bug){
                pc--;
                halt_bug = false;
            }
        }
    
        ExecuteCurrentOpcode();
    
        ServiceIME();
        ServiceInterrupts();
    
        UpdateTimer(clocks);
    
        arg_buf.clear();
    
    clock_count += clocks;
    
    return clocks;
}

void CPU::FetchNextOpcode(){
    opcode = read(pc);
    
    if(opcode == 0xCB){
        pc++;
        opcode = read(pc) + 256;
    }
    
    pc++;
}

void CPU::ExecuteCurrentOpcode(){
    
    clocks = lookup[opcode].clocks;
    ( this->*lookup[opcode].arg1)();
    ( this->*lookup[opcode].arg2)();
    ( this->*lookup[opcode].operate)();
    
}

inline uint8_t CPU::GetFlag(FLAGS f){
    return ((*F_reg & f) > 0) ? 1 : 0;
}

inline void CPU::SetFlag(FLAGS f, bool v){
    if (v)
        *F_reg |= f;
    else
        *F_reg &= ~f;
}

inline uint8_t CPU::fastRead(uint16_t a){
    return bus->memoryBus[a];
}

inline void CPU::fastWrite(uint16_t a, uint8_t data){
    bus->memoryBus[a] = data;
}

inline uint8_t CPU::read(uint16_t a){
    return bus->cpuRead(a,false);
}

inline void CPU::write(uint16_t a, uint8_t d){
    bus->cpuWrite(a,d);
}

void CPU::PushAddressToStack(uint16_t &a){
    uint8_t hi = ((a & 0xFF00) >> 8);
    uint8_t lo =  (a & 0x00FF);
    
    sp--;
    write(sp,hi);
    sp--;
    write(sp,lo);
}

void CPU::RunDMA(uint8_t data){
    uint16_t src_address = data << 8;
    
    for(uint16_t new_address = 0xFE00; new_address < 0xFEA0; new_address++){
        fastWrite(new_address, read(src_address++));
        // ADDITIONAL clocks????
    }
}

//===================== Interrupts & Timers ======================

void CPU::ServiceIME(){
    if(schedule_IME > 0){
        
        if(TestBit(schedule_IME, 0)){
            IME = 1;
            schedule_IME = 0;
        }else{
            schedule_IME |= b0;
        }
    }
}

void CPU::ServiceInterrupts(){
    
    if(IME){
        
        uint8_t enabled   = fastRead(IE);
        uint8_t requested = fastRead(IF);
        
        for(uint8_t interrupt=0; interrupt < 5; interrupt++){
            
            if(TestBit(enabled,interrupt) && TestBit(requested,interrupt)){
                IME = 0;
                fastWrite(IF,SetBit(fastRead(IF), interrupt, 0));
                
                PushAddressToStack(pc);
                
                switch (interrupt) {
                    case 0: pc = 0x0040;break;
                    case 1: pc = 0x0048;break;
                    case 2: pc = 0x0050;break;
                    case 3: pc = 0x0058;break;
                    case 4: pc = 0x0060;break;
                    default: break;
                }
                clocks += 20;
                
                return; // ATTENTION ???
            }
        }
    }
}

void CPU::RequestInterrupt(uint8_t b){
    uint8_t reg = fastRead(IF);
    reg |= (1<<b);
    fastWrite(IF,reg);
}

void CPU::EnableInterrupt(uint8_t b){
    uint8_t reg = fastRead(IE);
    reg |= (1<<b);
    fastWrite(IE,reg);
}

void CPU::UpdateTimer(uint8_t opcode_clocks){
    
    div_counter += opcode_clocks;
    
    if(div_counter >= div_rate){
        div_counter -= div_rate;
        
        if(fastRead(DIV) == 255){
            fastWrite(DIV,0);
        }else{
            //direct access. writing resets DIV
            bus->memoryBus[DIV]++;
        }
    }
    
    if(TimerEnabled()){        
        
        timer_counter += opcode_clocks;
        
        if(timer_counter >= timer_rate){
            
            while (timer_counter >= timer_rate) {
                if(fastRead(TIMA) == 255){
                    fastWrite(TIMA,fastRead(TMA));
                    RequestInterrupt(2);
                }else{
                    write(TIMA,read(TIMA)+1);
                }
                timer_counter -= timer_rate;
            }
        }
    }
}

inline bool CPU::TimerEnabled(){
    return (fastRead(TAC) & b2) > 0;
}

void CPU::UpdateTimerFreq(){
    uint16_t test_timer_rate = 0;
    switch (fastRead(TAC) & 0b11) {
        case 0: test_timer_rate = 1024; break;
        case 1: test_timer_rate =   16; break;
        case 2: test_timer_rate =   64; break;
        case 3: test_timer_rate =  256; break;
    }

    if(timer_rate != test_timer_rate){
        timer_rate = test_timer_rate;
    }
}


// ==================== Arguments =======================
inline void CPU::A(){
    arg_buf.push_back(A_reg);
}

inline void CPU::B(){
    arg_buf.push_back(B_reg);
}

inline void CPU::C(){
    arg_buf.push_back(C_reg);
}

inline void CPU::D(){
    arg_buf.push_back(D_reg);
}

inline void CPU::E(){
    arg_buf.push_back(E_reg);
}

inline void CPU::H(){
    arg_buf.push_back(H_reg);
}

inline void CPU::L(){
    arg_buf.push_back(L_reg);
}

inline void CPU::aIM(){
    im_16 = 0xFF00 + read(pc);
    pc++;
}

inline void CPU::aC(){
    im_16 = 0xFF00 + *C_reg;
}

inline void CPU::AF(){
    arg_buf.push_back(A_reg);
}

inline void CPU::BC(){
    arg_buf.push_back(B_reg);
}

inline void CPU::DE(){
    arg_buf.push_back(D_reg);
}

inline void CPU::HL(){
    arg_buf.push_back(H_reg);
}

inline void CPU::aBC(){
    addr_abs = (*B_reg << 8) | *C_reg;
    temp_8 = read(addr_abs);
    arg_buf.push_back(&temp_8);
}

inline void CPU::aDE(){
    addr_abs = (*D_reg << 8) | *E_reg;
    temp_8 = read(addr_abs);
    arg_buf.push_back(&temp_8);
}

inline void CPU::aHL(){
    addr_abs = (*H_reg << 8) | *L_reg;
    temp_8 = read(addr_abs);
    arg_buf.push_back(&temp_8);
}

inline void CPU::IM(){
    im_8 = read(pc);
    pc++;
    
    arg_buf.push_back(&im_8);
}

inline void CPU::IM16(){
    uint16_t lo = read(pc);
    pc++;
    uint16_t hi = read(pc);
    pc++;
    
    im_16 = (hi << 8) | lo;
}

inline void CPU::XX(){
    
}

inline void CPU::B0(){
    b = b0;
}
inline void CPU::B1(){
    b = b1;
}

inline void CPU::B2(){
    b = b2;
}

inline void CPU::B3(){
    b = b3;
}

inline void CPU::B4(){
    b = b4;
}

inline void CPU::B5(){
    b = b5;
}

inline void CPU::B6(){
    b = b6;
}

inline void CPU::B7(){
    b = b7;
}

inline void CPU::Z_cc(){
    cc = (GetFlag(z) == 1);
}

inline void CPU::C_cc(){
    cc = (GetFlag(c) == 1);
}

inline void CPU::NZ_cc(){
    cc = (GetFlag(z) == 0);
}

inline void CPU::NC_cc(){
    cc = (GetFlag(c) == 0);
}

// ======================= operations ============================

//=== 8bit loads===

inline void CPU::LD(){
    *arg_buf.front() = *arg_buf.back();
}

inline void CPU::LD_ar(){
    reg_ptr = arg_buf.front();
    
    uint16_t hi = *reg_ptr;
    reg_ptr++;
    uint16_t lo = *reg_ptr;
    
    addr_abs = (hi << 8) | lo;
    write(addr_abs,*arg_buf.back());
}

inline void CPU::LD_rnn(){
    *arg_buf.front() = read(im_16);
}

inline void CPU::LD_nnr(){
    write(im_16,*arg_buf.front());
}

inline void CPU::LDI_ra(){
    LD();
    addr_abs++;
    *H_reg = ((addr_abs & 0xFF00) >> 8);
    *L_reg = (addr_abs & 0x00FF);
}

inline void CPU::LDI_ar(){
    LD_ar();
    addr_abs++;
    *H_reg = ((addr_abs & 0xFF00) >> 8);
    *L_reg = (addr_abs & 0x00FF);
}

inline void CPU::LDD_ra(){
    LD();
    addr_abs--;
    *H_reg = ((addr_abs & 0xFF00) >> 8);
    *L_reg = (addr_abs & 0x00FF);
}

inline void CPU::LDD_ar(){
    LD_ar();
    addr_abs--;
    *H_reg = ((addr_abs & 0xFF00) >> 8);
    *L_reg = (addr_abs & 0x00FF);
}

// ===16bit loads===

inline void CPU::LD16(){
    reg_ptr = arg_buf.front();
    
    *reg_ptr = ((im_16 & 0xFF00) >> 8);
    reg_ptr++;
    *reg_ptr = (im_16 & 0x00FF);
}

inline void CPU::PUSH(){
    reg_ptr = arg_buf.front();
    
    sp--;
    write(sp,*reg_ptr);
    reg_ptr++;
    sp--;
    write(sp,*reg_ptr);
}

inline void CPU::POP(){
    reg_ptr = arg_buf.front();
    
    uint8_t lo = read(sp);
    sp++;
    uint8_t hi = read(sp);
    sp++;
    
    *reg_ptr = hi;
    reg_ptr++;
    *reg_ptr = lo;
}

inline void CPU::POPAF(){
    uint8_t lo = (read(sp) & 0xF0);
    sp++;
    uint8_t hi = read(sp);
    sp++;
    
    *A_reg = hi;
    *F_reg = lo;
}

inline void CPU::LDSP_nn(){
    sp = im_16;
}

inline void CPU::LDSP_a(){
    sp = (*H_reg << 8) | *L_reg;
}

inline void CPU::LDIMSP(){
    uint8_t hi = ((sp & 0xFF00) >> 8);
    uint8_t lo = (sp & 0x00FF);
    
    write(im_16,lo);
    im_16++;
    write(im_16,hi);
}

// ===8bit arithmetic/logic===

inline void CPU::ADD(){
    temp_16 = (uint16_t)*arg_buf.front()+ (uint16_t)*arg_buf.back();
    uint8_t hc = (*arg_buf.front() & 0x0F) + (*arg_buf.back() & 0x0F);
    
    *arg_buf.front() = (temp_16 & 0x00FF);
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,hc > 15);
    SetFlag(c, temp_16 > 255);
}

inline void CPU::ADC(){
    temp_16 = (uint16_t)*arg_buf.front() + (uint16_t)*arg_buf.back() + (uint16_t)GetFlag(c);
    uint8_t hc = (*arg_buf.front() & 0x0F) + (*arg_buf.back() & 0x0F) + GetFlag(c);
    
    *arg_buf.front() = temp_16 & 0x00FF;
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,hc > 15);
    SetFlag(c, temp_16 > 255);
}

void CPU::SUB(){
    temp_16 = (uint16_t)*arg_buf.front() - (uint16_t)*arg_buf.back();
    bool hc  = (*arg_buf.back() & 0x0F) > (*arg_buf.front() & 0x0F);
    bool carry = *arg_buf.back() > *arg_buf.front();
    
    *arg_buf.front() = (temp_16 & 0x00FF);
    
    if(opcode == 0x97){
        SetFlag(z,1);
        SetFlag(n,1);
        SetFlag(h,0);
        SetFlag(c,0);
    }else{
        SetFlag(z,*arg_buf.front() == 0);
        SetFlag(n,1);
        SetFlag(h,hc);
        SetFlag(c,carry);
    }
}

inline void CPU::SBC(){
    temp_16 = (uint16_t)*arg_buf.front() - (uint16_t)*arg_buf.back() - GetFlag(c);
    bool hc    = ((*arg_buf.back() & 0x0F) + GetFlag(c)) > (*arg_buf.front() & 0x0F);
    bool carry = ( *arg_buf.back() + GetFlag(c)) > *arg_buf.front();
    
    *arg_buf.front() = (temp_16 & 0x00FF);
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,1);
    SetFlag(h,hc);
    SetFlag(c,carry);
}

inline void CPU::AND(){
    *arg_buf.front() &= *arg_buf.back();
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,1);
    SetFlag(c,0);
}

inline void CPU::XOR(){
    *arg_buf.front() ^= *arg_buf.back();
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,0);
    SetFlag(c,0);
}

inline void CPU::OR(){
    *arg_buf.front() |= *arg_buf.back();
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,0);
    SetFlag(c,0);
}

inline void CPU::CP(){
    
    SetFlag(z,(*arg_buf.front() - *arg_buf.back()) == 0);
    SetFlag(n,1);
    SetFlag(h,(*arg_buf.back() & 0x0F) > (*arg_buf.front() & 0x0F));
    SetFlag(c, *arg_buf.back() > *arg_buf.front());
}

void CPU::DAA(){
    
    if(!GetFlag(n)){
        if(((*A_reg & 0xFF) > 0x99) || GetFlag(c)){
            *A_reg += 0x60;
            SetFlag(c,1);
        }
        
        if(((*A_reg & 0x0F) > 0x09) || GetFlag(h)){
            *A_reg += 0x06;
        }
    
    }else{
        if(GetFlag(h)){
            *A_reg -= 0x06;
        }
        
        if(GetFlag(c)){
            *A_reg -= 0x60;
        }
    }
    
    SetFlag(z, *A_reg == 0);
    SetFlag(h,0);
}

inline void CPU::CPL(){
    *A_reg = ~*A_reg;
    
    SetFlag(n,1);
    SetFlag(h,1);
}

inline void CPU::INC_r(){
    (*arg_buf.front())++;
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,(*arg_buf.front() & 0x0F) == 0);
}

inline void CPU::INC_a(){
    
    temp_8++;
    write(addr_abs,temp_8);
    
    SetFlag(z,temp_8 == 0);
    SetFlag(n,0);
    SetFlag(h,(temp_8 & 0x0F) == 0);
}

inline void CPU::DEC_r(){
    SetFlag(h,(*arg_buf.front() & 0x0F) == 0);
    (*arg_buf.front())--;
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,1);
}

inline void CPU::DEC_a(){
    SetFlag(h,(temp_8 & 0x0F) == 0);
    temp_8--;
    write(addr_abs,temp_8);
    
    SetFlag(z,temp_8 == 0);
    SetFlag(n,1);
}

// ======= 16bit arithmetic=========

void CPU::ADD16(){
    reg_ptr = arg_buf.back();
    
    uint8_t hi = *reg_ptr;
    reg_ptr++;
    uint8_t lo = *reg_ptr;
    
    uint32_t temp =  (uint16_t)(((*H_reg << 8) | *L_reg) & 0x0FFF);
    temp += (uint16_t)(((hi << 8) | lo) & 0x0FFF);
    
    bool hc = temp > 0x0FFF;
    
    temp = (uint32_t)((*H_reg << 8) | *L_reg);
    temp += (uint32_t)((hi << 8) | lo);
    
    bool carry = temp > 0xFFFF;
    
    *H_reg = (temp & 0xFF00) >> 8;
    *L_reg = (temp & 0x00FF);
    
    SetFlag(n,0);
    SetFlag(h,hc);
    SetFlag(c,carry);
}

void CPU::ADDSP(){
    uint32_t temp = (uint16_t)(((*H_reg << 8) | *L_reg) & 0x0FFF);
    temp += (sp & 0x0FFF);
    
    bool hc = temp > 0x0FFF;
    
    temp = (uint32_t)((*H_reg << 8) | *L_reg);
    temp += sp;
    
    bool carry = temp > 0xFFFF;
    
    *H_reg = (temp & 0xFF00) >> 8;
    *L_reg = (temp & 0x00FF);
    
    SetFlag(n,0);
    SetFlag(h,hc);
    SetFlag(c,carry);
}

void CPU::ADDSP_dd(){
    
    int8_t dd = (int8_t)im_8;
    
    if(dd >= 0){
        SetFlag(c,(uint16_t)((sp & 0x00FF) + dd) > 0xFF);
        SetFlag(h,(uint16_t)((sp & 0x000F) + dd) > 0x0F);
    }else{
        SetFlag(c,(uint16_t)((sp & 0x00FF) + dd) <= (sp & 0x00FF));
        SetFlag(h,(uint16_t)((sp & 0x000F) + dd) <= (sp & 0x000F));
    }
    
    sp = (uint16_t)(sp + dd) & 0xFFFF;
    
    SetFlag(z,0);
    SetFlag(n,0);
}

void CPU::INC16(){
    reg_ptr = arg_buf.front();
    
    uint8_t hi = *reg_ptr;
    reg_ptr++;
    uint8_t lo = *reg_ptr;
    
    temp_16 = ((hi << 8) | lo);
    temp_16++;
    
    reg_ptr--;
    *reg_ptr = (temp_16 & 0xFF00) >> 8;
    reg_ptr++;
    *reg_ptr = (temp_16 & 0x00FF);
}

inline void CPU::INCSP(){
    sp++;
}

void CPU::DEC16(){
    reg_ptr = arg_buf.front();
    
    uint8_t hi = *reg_ptr;
    reg_ptr++;
    uint8_t lo = *reg_ptr;
    
    temp_16 = ((hi << 8) | lo);
    temp_16--;
    
    reg_ptr--;
    *reg_ptr = (temp_16 & 0xFF00) >> 8;
    reg_ptr++;
    *reg_ptr = (temp_16 & 0x00FF);
}

inline void CPU::DECSP(){
    sp--;
}

void CPU::LDHL_dd(){
    int8_t dd = (int8_t)im_8;
    
    
    if(dd >= 0){
        SetFlag(c,(uint16_t)((sp & 0x00FF) + dd) > 0xFF);
        SetFlag(h,(uint16_t)((sp & 0x000F) + dd) > 0x0F);
    }else{
        SetFlag(c,(uint16_t)((sp & 0x00FF) + dd) <= (sp & 0x00FF));
        SetFlag(h,(uint16_t)((sp & 0x000F) + dd) <= (sp & 0x000F));
    }
    temp_16 = (uint16_t)(sp + dd);
    *H_reg = (temp_16 & 0xFF00) >> 8;
    *L_reg = (temp_16 & 0x00FF);
    
    SetFlag(z,0);
    SetFlag(n,0);
}

// ======== rotate and shift ==========

inline void CPU::RLCA(){
    *A_reg = (*A_reg << 1)|(*A_reg >> 7);
    SetFlag(c,*A_reg & b0);
    
    SetFlag(z,0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::RLA(){
    temp_8 = *A_reg;
    *A_reg = (*A_reg << 1)|(GetFlag(c));
    SetFlag(c,(temp_8 & b7) > 0);
    
    SetFlag(z,0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::RRCA(){
    *A_reg = (*A_reg >> 1)|(*A_reg << 7);
    SetFlag(c,(*A_reg & b7) > 0);
    
    SetFlag(z,0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::RRA(){
    temp_8 = *A_reg;
    *A_reg = (*A_reg >> 1)|(GetFlag(c) << 7);
    
    SetFlag(z,0);
    SetFlag(n,0);
    SetFlag(h,0);
    SetFlag(c,(temp_8 & b0) > 0);
}
inline void CPU::RLC_r(){
    *arg_buf.front() = (*arg_buf.front() << 1)|(*arg_buf.front() >> 7);
    SetFlag(c,(*arg_buf.front() & b0) > 0);
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::RLC_a(){
    temp_8 = (temp_8 << 1)|(temp_8 >> 7);
    SetFlag(c,(temp_8 & b0) > 0);
    write(addr_abs,temp_8);
    
    SetFlag(z,temp_8 == 0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::RL_r(){
    temp_8 = *arg_buf.front();
    *arg_buf.front() = (*arg_buf.front() << 1)|(GetFlag(c));
    SetFlag(c,(temp_8 & b7) > 0);
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::RL_a(){
    uint8_t cpy = temp_8;
    temp_8 = (temp_8 << 1)|(GetFlag(c));
    write(addr_abs,temp_8);
    SetFlag(c,(cpy & b7) > 0);
    
    SetFlag(z,temp_8 == 0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::RRC_r(){
    *arg_buf.front() = (*arg_buf.front() >> 1)|(*arg_buf.front() << 7);
    SetFlag(c,(*arg_buf.front() & b7) > 0);
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::RRC_a(){
    temp_8 = (temp_8 >> 1)|(temp_8 << 7);
    write(addr_abs,temp_8);
    SetFlag(c,(temp_8 & b7) > 0);
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::RR_r(){
    temp_8 = *arg_buf.front();
    *arg_buf.front() = (*arg_buf.front() >> 1)|(GetFlag(c) << 7);
    SetFlag(c,(temp_8 & b0) > 0);
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::RR_a(){
    uint8_t cpy = temp_8;
    temp_8 = (temp_8 >> 1)|(GetFlag(c) << 7);
    write(addr_abs,temp_8);
    SetFlag(c,(cpy & b0) > 0);
    
    SetFlag(z,temp_8 == 0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::SLA_r(){
    SetFlag(c,(*arg_buf.front() & b7) > 0);
    *arg_buf.front() = *arg_buf.front() << 1;
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::SLA_a(){
    SetFlag(c,(temp_8 & b7) > 0);
    temp_8 = (temp_8 << 1);
    write(addr_abs,temp_8);
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::SWAP_r(){
    *arg_buf.front() = (*arg_buf.front() << 4)|(*arg_buf.front() >> 4);
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,0);
    SetFlag(c,0);
}
inline void CPU::SWAP_a(){
    temp_8 = (temp_8 << 4)|(temp_8 >> 4);
    write(addr_abs,temp_8);
    
    SetFlag(z,temp_8 == 0);
    SetFlag(n,0);
    SetFlag(h,0);
    SetFlag(c,0);
}
inline void CPU::SRA_r(){
    SetFlag(c,(*arg_buf.front() & b0) > 0);
    *arg_buf.front() = (*arg_buf.front() >> 1)|(*arg_buf.front() & b7);
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::SRA_a(){
    SetFlag(c,(temp_8 & b0) > 0);
    temp_8 = (temp_8 >> 1)|(temp_8 & b7);
    write(addr_abs,temp_8);
    
    SetFlag(z,temp_8 == 0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::SRL_r(){
    SetFlag(c,(*arg_buf.front() & b0) > 0);
    *arg_buf.front() = *arg_buf.front() >> 1;
    
    SetFlag(z,*arg_buf.front() == 0);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::SRL_a(){
    SetFlag(c,(temp_8 & b0) > 0);
    temp_8 = temp_8 >> 1;
    write(addr_abs,temp_8);
    
    SetFlag(z,temp_8 == 0);
    SetFlag(n,0);
    SetFlag(h,0);
}

// ========= single bit instructions ===========
inline void CPU::BIT_r(){
    SetFlag(z,(*arg_buf.front() & b) == 0);
    
    SetFlag(n,0);
    SetFlag(h,1);
}
inline void CPU::BIT_a(){
    SetFlag(z,(temp_8 & b) == 0);
    
    SetFlag(n,0);
    SetFlag(h,1);
}
inline void CPU::SET_r(){
    *arg_buf.front() |= b;
}
inline void CPU::SET_a(){
    temp_8 |= b;
    write(addr_abs,temp_8);
}
inline void CPU::RES_r(){
    *arg_buf.front() &= ~b;
}
inline void CPU::RES_a(){
    temp_8 &= ~b;
    write(addr_abs,temp_8);
}

// =========== CPU Control ============

inline void CPU::CCF(){
    SetFlag(c,!GetFlag(c));
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::SCF(){
    SetFlag(c,1);
    SetFlag(n,0);
    SetFlag(h,0);
}
inline void CPU::NOP(){
    
}
void CPU::HALT(){
    
    halted = true;
    
    //interrupt pending
    if( (read(IE) & read(IF) & 0x1F) != 0){
        if(IME){
            halted = false;
            //pc--;
        }else{
            halted = false;
            halt_bug = true;
        }
    }
}
inline void CPU::STOP(){
    write(DIV,0); // !!! check
    pc++;
}
inline void CPU::DI(){
    IME = 0;
}
inline void CPU::EI(){
    schedule_IME = 0b10;
}

//========== Jump Instructions ===========

inline void CPU::JP(){
    if(opcode == 0xC3){
        IM16();
        pc = im_16;
    }else{
        pc = (*H_reg << 8) | *L_reg;
    }
}
inline void CPU::JPF(){
    IM16();
    
    if(cc){
        pc = im_16;
    }else{
        //overide clocks
        clocks = 12;
    }
}
inline void CPU::JR(){
    int8_t dd = (int8_t)read(pc);
    pc++;
    pc += dd;
}
inline void CPU::JRF(){
    int8_t dd = (int8_t)read(pc);
    pc++;
    
    if(cc){
        pc += dd;
    }else{
        clocks = 8;
    }
}
inline void CPU::CALL(){
    IM16();
    PushAddressToStack(pc);
    pc = im_16;
}
inline void CPU::CALLF(){
    IM16();
    
    if(cc){
        PushAddressToStack(pc);
        pc = im_16;
    }else{
        clocks = 12;
    }
}
inline void CPU::RET(){
    uint8_t lo = read(sp);
    sp++;
    uint8_t hi = read(sp);
    sp++;
    
    pc = (hi << 8) | lo;
}
inline void CPU::RETF(){
    if(cc){
        RET();
    }else{
        clocks = 8;
    }
}
inline void CPU::RETI(){
    EI();
    RET();
}

void CPU::RST(){
    PushAddressToStack(pc);
    switch (opcode){
        case 0xC7: addr_abs = 0x0000;break;
        case 0xCF: addr_abs = 0x0008;break;
        case 0xD7: addr_abs = 0x0010;break;
        case 0xDF: addr_abs = 0x0018;break;
        case 0xE7: addr_abs = 0x0020;break;
        case 0xEF: addr_abs = 0x0028;break;
        case 0xF7: addr_abs = 0x0030;break;
        case 0xFF: addr_abs = 0x0038;break;
    }
    
    pc = addr_abs;
}

inline void CPU::XXX(){}


