//
//  IRStrComponent.hpp
//  IRiMaS
//
//  Created by Keitaro on 29/08/2019.
//

#ifndef IRStrComponent_hpp
#define IRStrComponent_hpp

#include "IRStr.h"

class IRStrComponent
{
 
public:
    IRStrComponent(IRStr* str);
    ~IRStrComponent();
    
    IRStr* getStr() const { return this->ir_str; }
    void setStr(IRStr* str) { this->ir_str = str; }
    KeyListener* getMasterKey() const { return this->masterKey; }
    
protected:
    // accessible from child classes
    UserSettingStr USERSETTING;
    IR::IRColours SYSTEMCOLOUR;
    IRIconBank    ICONBANK;
    IRObjectFactory OBJECTFACTORY;
    
private:
    IRStr* ir_str;
    KeyListener* masterKey;
    
};

#endif /* IRStrComponent_hpp */
