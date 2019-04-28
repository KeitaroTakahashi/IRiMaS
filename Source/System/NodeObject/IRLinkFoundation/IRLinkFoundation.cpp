//
//  IRLinkFoundation.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 25/04/2019.
//

#include "IRLinkFoundation.hpp"

IRLinkFoundation::IRLinkFoundation(Component* parent, int size) :
parent(parent),
linkMenuSize(size)
{
    this->linkFlags.clear();
}

IRLinkFoundation::~IRLinkFoundation()
{
    this->linkFlags.clear();
}
// ==================================================
bool IRLinkFoundation::isLinkMode() const
{
    return this->linkModeFlag;
}
// --------------------------------------------------
void IRLinkFoundation::setLinkMode(bool flag)
{
    this->linkModeFlag = flag;
    linkModeChangedEvent();
}
// --------------------------------------------------
bool IRLinkFoundation::isLinkActivated() const
{
    return this->linkActivationFlag;
}
// --------------------------------------------------
void IRLinkFoundation::setLinkActivation(bool flag)
{
    this->linkActivationFlag = flag;
    setLinkActivationEvent();
}
// --------------------------------------------------

bool IRLinkFoundation::isLinked() const
{
    return this->linkedFlag;
}
// --------------------------------------------------

void IRLinkFoundation::setLinked(bool flag)
{
    this->linkedFlag = flag;
    linkChangedEvent();
}

// ==================================================
// link system
void IRLinkFoundation::addLinkParam(IRLinkSystemFlag flag)
{
    this->linkFlags.push_back(flag);
    LinkParamAddedEvent();
}
// --------------------------------------------------
void IRLinkFoundation::revemoLinkParam(IRLinkSystemFlag flag)
{
    int index = 0;
    for(auto f : this->linkFlags)
    {
        if(f == flag)
            this->linkFlags.erase(this->linkFlags.begin() + index);
        index++;
    }
    LinkParamRemovedEvent();
}
// --------------------------------------------------
void IRLinkFoundation::clearLinkParam()
{
    this->linkFlags.clear();
    LinkParamClearedEvent();
}
// --------------------------------------------------
void IRLinkFoundation::createLinkMenu()
{
    this->linkMenu = std::make_shared<IRLinkMenuObject>(this->parent, this->linkFlags);
    this->linkMenu.get()->setSize(this->linkMenuSize * (int) this->linkFlags.size(),
                                  this->linkMenuSize);
    this->linkMenu.get()->notifySelectedItem = [this] (IRLinkSystemFlag flag) { receiveSelectedLinkMenuItem(flag); };
    
    LinkMenuCreatedEvent();
}
// --------------------------------------------------
void IRLinkFoundation::receiveSelectedLinkMenuItem(IRLinkSystemFlag flag)
{
    this->selectedLinkSystemFlag = flag;
    receiveSelectedLinkMenuItemEvent();
}
// --------------------------------------------------
IRLinkMenuObject* IRLinkFoundation::getLinkMenu()
{
    if(this->linkMenu)
    {
        return this->linkMenu.get();
    }else{
        createLinkMenu();
        return this->linkMenu.get();
    }
}
// --------------------------------------------------
void IRLinkFoundation::openLinkMenu()
{
    if(this->linkMenu)
    {
        if(!this->linkMenu.get()->isShowing()){
            
            this->parent->addAndMakeVisible(this->linkMenu.get());
            this->linkMenu.get()->toFront(true);
        }
    }else{
        
        createLinkMenu();
        this->parent->addAndMakeVisible(this->linkMenu.get());
        this->linkMenu.get()->toFront(true);
        
    }
    this->linkMenuOpenedFlag = true;
}
// --------------------------------------------------
void IRLinkFoundation::closeLinkMenu()
{
    if(this->linkMenu)
    {
        if(this->linkMenu.get()->isShowing())
        {
            this->linkMenu.get()->deSelectAll();
            this->parent->removeChildComponent(this->linkMenu.get());
            this->linkMenuOpenedFlag = false;
            setLinkActivation(false);
        }
    }
}
// --------------------------------------------------
bool IRLinkFoundation::isLinkMenuOpened() const
{
    return this->linkMenuOpenedFlag;
}
// --------------------------------------------------

void IRLinkFoundation::setLinkMenuCentrePosition(int x, int y)
{
    if(isLinkMenuOpened())
        this->linkMenu.get()->setCentrePosition(x, y);
}
// --------------------------------------------------

// --------------------------------------------------

// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------
