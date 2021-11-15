//
//  MBC_NONE.hpp
//  Emulator_GB
//
//  Created by Chris D'Arcy on 03/11/2021.
//  Copyright © 2021 Chris D'Arcy. All rights reserved.
//

#ifndef MBC_NONE_hpp
#define MBC_NONE_hpp

#include "MBC.hpp"

class MBC_NONE : public MBC {
public:
    MBC_NONE();
    ~MBC_NONE();
    
public:
    void BankSwitching(uint16_t a, uint8_t data) override;
};
#endif /* MBC_NONE_hpp */
