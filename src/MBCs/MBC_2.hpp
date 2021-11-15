//
//  MBC_2.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 03/11/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#ifndef MBC_2_hpp
#define MBC_2_hpp

#include "MBC.hpp"

class MBC_2 : public MBC {
public:
    MBC_2();
    MBC_2(uint16_t nROMBanks, uint8_t  nRAMBanks);
    ~MBC_2();
    
public:
    void BankSwitching(uint16_t a, uint8_t data) override;
};

#endif /* MBC_2_hpp */
