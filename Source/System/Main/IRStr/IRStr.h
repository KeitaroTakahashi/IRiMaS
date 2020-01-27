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
#include "IRFileManager.hpp"
#include "UserSettingStr.h"
#include "IRProjectOwnerBase.h"


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
    
    // this flag is accessible by all objects inheriting IRStrComponent.
    // it is ideal to give FALSE to isProjectSaved when user made any changes
    bool isProjectSaved = true;
    
    // store currently active workspace
    Component* TopWorkspace = nullptr;
    
    // store the project owner
    IRProjectOwnerBase* projectOwner = nullptr;
    // ==================================================
    // callBack
    std::vector<std::function<void(IRFileManager&)>> fileManagerUpdatedCallbackList;
        
    void addFileManagerUpdatedCallbackFunc(std::function<void(IRFileManager&)> callBack)
    {
        this->fileManagerUpdatedCallbackList.push_back(callBack);
    }
    
    void notifyFileManagerUpdate()
    {
        for(int i = 0; i < this->fileManagerUpdatedCallbackList.size(); i ++ )
        {
            this->fileManagerUpdatedCallbackList[i](FILEMANAGER);
        }
    }
    
    // ==================================================
    UserSettingStr USERSETTING;
    IR::IRColours SYSTEMCOLOUR;
    IRIconBank    ICONBANK;
    IRFileManager FILEMANAGER;

    // ==================================================
    // load and store here as it costs time
    StringArray fontFamilyList;

};

#endif /* IRStr_h */
