//
//  MBC_1.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 03/11/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#ifndef MBC_1_hpp
#define MBC_1_hpp

#include "MBC.hpp"

class MBC_1 : public MBC {
public:
    MBC_1();
    MBC_1(uint16_t nROMBanks, uint8_t  nRAMBanks);
    ~MBC_1();
    
public:
    void BankSwitching(uint16_t a, uint8_t data) override;
};


#endif /* MBC_1_hpp */
