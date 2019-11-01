//
//  IRProjectWindow2.cpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#include "IRProjectWindow2.hpp"

IRProjectWindow2::IRProjectWindow2(String name, Rectangle<int> frameRect) : IRMainWindow(name)
{
    
    //Rectangle<int> frameRect (10, 10, 1280, 800);
    this->comp.reset(new IRWindowComponent(name, frameRect));
    this->comp->setSize(frameRect.getWidth(), frameRect.getHeight());
    this->comp->windowMoveAction = [this](Point<int>pos) { windowMoveToPos(pos); };
    this->comp->newProjectCallback = [this] { newProjectCallbackAction(); };
    this->comp->closeProjectCallback = [this]{ closeButtonPressed(); };

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
    this->comp.reset();
}

// ==================================================

void IRProjectWindow2::changeListenerCallback (ChangeBroadcaster* source)
{
  
}
// ==================================================

void IRProjectWindow2::closeButtonPressed()
{
    std::cout << "IRProjectWindow2::closeButtonPressed\n";
    callCloseThisWindow();
}
// ==================================================


void IRProjectWindow2::windowMoveToPos(Point<int>pos)
{
    setTopLeftPosition(pos);
}
// ==================================================

void IRProjectWindow2::callCloseThisWindow()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.closeThisWindow(this); });
    //check again
    if(checker.shouldBailOut()) return;
}

// ==================================================

void IRProjectWindow2::setNewProjectCallbackFunc(std::function<void()> callback)
{
    this->newProjectCallback = callback;
}

void IRProjectWindow2::newProjectCallbackAction()
{
    if(this->newProjectCallback != nullptr)
        this->newProjectCallback();
}
