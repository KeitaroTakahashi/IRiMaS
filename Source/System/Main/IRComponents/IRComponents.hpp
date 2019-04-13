//
//  IRComponents.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 14/04/2019.
//
//
// Pack of essential components of IRiMaS interface
//

#ifndef IRComponents_hpp
#define IRComponents_hpp

#include "JuceHeader.h"
#include "IRFileManager.hpp"

class IRComponents
{
public:
    IRComponents() {}
    ~IRComponents() {}
    
    // set IRFileManager from IRProject
    void setIRFileManager(IRFileManager* manager) { this->fileManager = manager; }
    IRFileManager* getFileManager() { return this->fileManager; }
    
private:
    // IRFileManager is exclusive for each Project
    IRFileManager* fileManager = nullptr;
    
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRComponents)
};


#endif /* IRComponents_hpp */
