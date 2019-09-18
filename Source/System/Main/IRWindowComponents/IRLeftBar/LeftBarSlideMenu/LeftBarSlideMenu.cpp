//
//  LeftBarSlideMenu.cpp
//  IRiMaS
//
//  Created by Keitaro on 17/09/2019.
//

#include "LeftBarSlideMenu.hpp"

LeftBarSlideMenu::LeftBarSlideMenu(IRStr* str) :
IRStrComponent(str)
{
    this->slideMenu = std::make_shared<SlideMenu>(str);
    this->viewPort = std::make_shared<Component4ViewPort>(this->slideMenu.get());
    
    //addAndMakeVisible(this->viewPort.get());
    
    setViewedComponent(this->viewPort.get());
}

LeftBarSlideMenu::~LeftBarSlideMenu()
{
    
}

// ==================================================

void LeftBarSlideMenu::resized()
{
    this->viewPort->setBounds(0,0,getWidth()-10, 2000);
    this->slideMenu->setBounds(0, 0, getWidth()-10, 2000);
    
}


// ==================================================

void LeftBarSlideMenu::visibleAreaChangedAction(const Rectangle<int> &newVisibleArea)
{

}
// ==================================================
