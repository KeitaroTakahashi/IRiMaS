//
//  IRFileInspecterWindow.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 11/03/2019.
//

#include "IRFileInspecterWindow.hpp"

IRFileInspecterWindow::IRFileInspecterWindow(String name) :
DocumentWindow(name,
               Desktop::getInstance().getDefaultLookAndFeel()
                .findColour (ResizableWindow::backgroundColourId),
                DocumentWindow::allButtons)
{
    
}

IRFileInspecterWindow::~IRFileInspecterWindow()
{
    
}
