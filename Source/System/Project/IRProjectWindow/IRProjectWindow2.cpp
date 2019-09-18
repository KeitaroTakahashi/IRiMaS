//
//  IRProjectWindow2.cpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#include "IRProjectWindow2.hpp"

IRProjectWindow2::IRProjectWindow2(String name) : IRMainWindow(name)
{
    
    Rectangle<int> frameRect (10, 10, 900, 700);
    
    this->comp.reset(new IRWindowComponent(name, frameRect));
    this->comp->setSize(900, 700);
    this->comp->windowMoveAction = [this](Point<int>pos) { windowMoveToPos(pos); };

    setContentOwned (this->comp.get(), true);
    
#if JUCE_IOS || JUCE_ANDROID
    setFullScreen (true);
#else
    setResizable (true, true);
    centreWithSize (getWidth(), getHeight());
#endif
    
    setVisible (true);
}

IRProjectWindow2::~IRProjectWindow2()
{
    
}

// ==================================================

void IRProjectWindow2::changeListenerCallback (ChangeBroadcaster* source)
{
    /*
    if(source == this->bar.get())
    {
        if(this->bar->getStatus() == IRWindowBarActionStatus::MoveWindow)
            setTopLeftPosition(this->bar->pos);
    }
    else if(source == this->leftBar.get())
    {
        if(this->leftBar->getStatus() == IRWindowBarActionStatus::MoveWindow)
            setTopLeftPosition(this->leftBar->pos);
    }
     */
}
// ==================================================

void IRProjectWindow2::closeButtonPressed()
{
    
}
// ==================================================


void IRProjectWindow2::windowMoveToPos(Point<int>pos)
{
    setTopLeftPosition(pos);
}
// ==================================================
