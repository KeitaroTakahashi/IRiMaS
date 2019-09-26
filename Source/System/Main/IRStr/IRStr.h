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
#include "IRObjectFactory2.hpp"
#include "IRFileManager.hpp"
#include "UserSettingStr.h"


struct IRStr
{
    
    // KeyListener from IRWIndowComponent
    KeyListener* key;
    void setKeyListener(KeyListener* key) { this->key = key; }
    
    MouseListener* mouse;
    void setMouseListener(MouseListener* mouse) { this->mouse = mouse; }
    
    // ==================================================

    String projectName;
    String projectURL;
    
    Component* TopWorkspace = nullptr;
    
    // ==================================================
    // callBack
    std::function<void(IRFileManager*)> fileManagerUpdated;
    
    
    
    // ==================================================
    UserSettingStr USERSETTING;
    IR::IRColours SYSTEMCOLOUR;
    IRIconBank    ICONBANK;
    IRObjectFactory2 OBJECTFACTORY;
    IRFileManager FILEMANAGER;
    // ==================================================

};

#endif /* IRStr_h */
