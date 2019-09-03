//
//  IRStr.h
//  IRiMaS
//
//  Created by Keitaro on 29/08/2019.
//

#ifndef IRStr_h
#define IRStr_h

#include "JuceHeader.h"
#include "ColourLib.h"
#include "IRIconBank.hpp"
#include "IRObjectFactory.hpp"
#include "UserSettingStr.h"


struct IRStr
{
    
    // KeyListener from IRWIndowComponent
    KeyListener* key;
    void setKeyListener(KeyListener* key) { this->key = key; }
    
    // ==================================================

    String projectName;
    String projectURL;
    
    // ==================================================
    UserSettingStr USERSETTING;
    IR::IRColours SYSTEMCOLOUR;
    IRIconBank    ICONBANK;
    IRObjectFactory OBJECTFACTORY;
    // ==================================================

};

#endif /* IRStr_h */
