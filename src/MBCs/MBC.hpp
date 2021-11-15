//
//  MBC.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 03/11/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#ifndef MBC_hpp
#define MBC_hpp

#include <stdint.h>

class MBC{
    
public:
    MBC();
    MBC(uint16_t nROMBanks, uint8_t  nRAMBanks);
    ~MBC();
    
public:
    
    void MapAddrToReadROM(uint32_t &a);
    void MapAddrToReadRAM(uint32_t &a);
    bool MapAddrToWriteRAM(uint32_t &a);
    
    virtual void BankSwitching(uint16_t a, uint8_t data) = 0;
    
protected:
    bool RAMEnabled     = false;
    bool ROMBankingMode = true;
    
    uint16_t nROMBanks;
    uint8_t  nRAMBanks;
    
    uint16_t currentROMBank = 0; // 1st after 0th non switchable bank
    uint8_t currentRAMBank = 0;
    
};

#endif /* MBC_hpp */
