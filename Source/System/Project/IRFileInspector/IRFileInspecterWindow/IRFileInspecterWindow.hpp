//
//  IRFileInspecterWindow.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 11/03/2019.
//

#ifndef IRFileInspecterWindow_hpp
#define IRFileInspecterWindow_hpp

#include "JuceHeader.h"

#include "IRFileInspecter.hpp"

class IRFileInspecterWindow : DocumentWindow
{
public:
    IRFileInspecterWindow(String name);
    ~IRFileInspecterWindow();
    
    // ==================================================
    void closeButtonPressed();
    
    // ==================================================

private:
    
    IRFileInspecter* fileInspecter;
    
};

#endif /* IRFileInspecterWindow_hpp */
