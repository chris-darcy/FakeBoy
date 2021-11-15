//
//  MBC_5.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 04/11/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#ifndef MBC_5_hpp
#define MBC_5_hpp

#include "MBC.hpp"

class MBC_5 : public MBC {
public:
    MBC_5();
    MBC_5(uint16_t nROMBanks, uint8_t  nRAMBanks);
    ~MBC_5();
    
public:
    void BankSwitching(uint16_t a, uint8_t data) override;
};
#endif /* MBC_5_hpp */
