//
//  IRWindowComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#include "IRWindowComponent.hpp"

// ==================================================
// FUNDAMENTAL
// ==================================================

IRWindowComponent::IRWindowComponent(String projectName,
                                     Rectangle<int> frameRect) :
projectName(projectName), frameRect(frameRect)
{
    initialize();
    createComponents();
    //createNewWorkspace();
}
// ----------------------------------------

IRWindowComponent::~IRWindowComponent()
{
    
}

// ----------------------------------------

void IRWindowComponent::paint(Graphics& g)
{
    g.fillAll(SYSTEMCOLOUR.fundamental);
}
// ----------------------------------------

void IRWindowComponent::resized() 
{
    int w = getWidth();
    int h = getHeight();
    
    this->mainComponentHeight = h - this->barHeight;
    
    this->bar->setBounds(0, 0, w, this->barHeight);
    this->mainSpace->setBounds(this->leftBarWidth, this->barHeight,
                               w - this->leftBarWidth, this->mainComponentHeight);
    
    
    this->leftBar->setBounds(0, this->barHeight,
                             this->leftBarWidth,
                             h - this->barHeight);
    
    
    
    
}
// ----------------------------------------

void IRWindowComponent::initialize()
{
    
    //keyListener setup
    setWantsKeyboardFocus(true);
    addKeyListener(this);
    
    // create IRStr
    this->ir_str.reset(new IRStr());
    this->ir_str->setKeyListener(this);
    this->ir_str->setMouseListener(this);
    this->ir_str->projectName = this->projectName;
    this->ir_str->SYSTEMCOLOUR = IR::IRBlue();
}
// ----------------------------------------

void IRWindowComponent::createComponents()
{
    this->leftBar.reset(new IRLeftBar(this->ir_str.get()));
    this->bar.reset(new IRTitleBar(this->ir_str.get()));
    this->mainSpace.reset(new IRMainSpace(this->ir_str.get()));
    
    this->bar->addChangeListener(this);
    this->leftBar->addChangeListener(this);
    
    // main space is back
    addAndMakeVisible(this->mainSpace.get());
    addAndMakeVisible(this->bar.get());
    addAndMakeVisible(this->leftBar.get());

    
}
// ----------------------------------------

void IRWindowComponent::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == this->bar.get())
    {
        if(this->bar->getStatus() == IRWindowBarActionStatus::MoveWindow)
        {
            if(this->windowMoveAction != nullptr)
                this->windowMoveAction(this->bar->pos);
        }
    }
    else if(source == this->leftBar.get())
    {
        if(this->leftBar->getStatus() == IRWindowBarActionStatus::MoveWindow)
        {
            if(this->windowMoveAction != nullptr)
            {
                this->windowMoveAction(this->leftBar->pos);
            }
        }
    }
}
// ----------------------------------------

// ==================================================
// KeyEvent
// ==================================================

bool IRWindowComponent::keyPressed(const KeyPress& key, Component* originatingComponent)
{
    std::cout << "IRWorkSpace keyPressed() : " << key.getKeyCode() << " : " << key.getTextDescription() << ", " << key.getTextCharacter() <<   std::endl;
    
    if(key.getKeyCode() == key.deleteKey || key.getKeyCode() == key.backspaceKey)
    {
        
        std::cout << "delete key\n";
        //if(isEditMode())
         //   deleteSelectedObjects();
        return true;
    }
    
    if(key.getTextDescription() == "command + C")
    {
        
        std::cout << "command C\n";

        /*
        if(this->isEditMode())
        {
            copySelectedObjects();
        }
         */
        return true;
    }
    
    if(key.getTextDescription() == "command + V")
    {
        /*
        if(this->isEditMode())
        {
            pasteSelectedObjects();
        }
         */
        return true;
    }
    
    if(key.getTextDescription() == "command + D")
    {
        /*
        if(this->isEditMode())
        {
            duplicateSelectedObjects();
        }*/
        return true;
    }
    
    if(key.getTextDescription() == "command + E")
    {
        /*
        setEditMode(!this->editModeFlag);
        
        // notify it to IRProject
        if(this->notifyEditModeChanged != nullptr)
        {
            this->notifyEditModeChanged();
        }
        */
        return true;
    }
    
    // in case no key
    /*
    if(! this->isEditMode())
    {
        std::cout << "IRWworkspace : The workspace can not receive any key event because it is in Control Mode, if you want to control Object interface, please click an object you want to control and give a KeyEventFocus on it." << std::endl;
    }
    else {
        std::cout << "IRWorkspace : Unknown KeyEvent received." << std::endl;
    }
    */
    return false;
}
// ----------------------------------------

void IRWindowComponent::modifierKeysChanged(const ModifierKeys &mod)
{
    
}

// ----------------------------------------

// ==================================================
// MOUSE EVENT
// ==================================================

void IRWindowComponent::mouseDrag(const MouseEvent& e)
{
    auto pos = e.getEventRelativeTo(this).getPosition();
    
    if(this->isResizable)
    {
        Point<int> delta = pos - this->prevPos;
        
        int newW = this->prevSize.getX() + delta.getX();
        int newH = this->prevSize.getY() + delta.getY();
        if(newW < this->minWidth) newW = this->minWidth;
        if(newH < this->minHeight) newH = this->minHeight;
        
        setSize(newW, newH);
    }
    
}
// ----------------------------------------

void IRWindowComponent::mouseUp(const MouseEvent& e)
{
    if(this->isResizable)
    {
        this->isResizable = false;
        if(this->mainSpace->getTopWorkspace() != nullptr)
            this->mainSpace->getTopWorkspace()->setResizing(true);
    }
}
// ----------------------------------------

void IRWindowComponent::mouseDown(const MouseEvent& e)
{
    auto pos = e.getEventRelativeTo(this).getPosition();
    this->prevPos = pos;
    
    std::cout << "IRWindowComponent : " << pos.getX() << ", " << pos.getY() << std::endl;
    std::cout << "resizable area x " << getWidth() - this->resizableMargin << " : " << getHeight() - this->resizableMargin << std::endl;
 
    // store current window size
    this->prevSize = Point<int> (getWidth(), getHeight());
    
    if(pos.getX() > getWidth() - this->resizableMargin &&
       pos.getY() > getHeight() - this->resizableMargin)
    {
        this->isResizable = true;
        if(this->mainSpace->getTopWorkspace() != nullptr)
            this->mainSpace->getTopWorkspace()->setResizing(true);
    }else
    {
        this->isResizable = false;
        if(this->mainSpace->getTopWorkspace() != nullptr)
            this->mainSpace->getTopWorkspace()->setResizing(true);
    }
}
// ----------------------------------------

void IRWindowComponent::mouseMove(const MouseEvent& e)
{
    
}
// ----------------------------------------

// ==================================================
// CHILD COMPONENTS
// ==================================================

void IRWindowComponent::setComponentsHeight(int barHeight, int mainHeight)
{
    this->barHeight = barHeight;
    this->mainComponentHeight = mainHeight;
}
// ----------------------------------------


// ==================================================
// WORKSPACE
// ==================================================

void IRWindowComponent::createNewWorkspace()
{
    
    std::cout << "create IRWOrkspace\n";

    String title = this->projectName + "_" + String(this->workspaces.size() + 1);
    std::cout << "create IRWOrkspace\n";

    Rectangle<int> frameRect = this->mainSpace->getBounds();
    
    std::cout << "create IRWOrkspace\n";
   //IRWorkSpace* space = new IRWorkSpace(title, this->ir_str.get(), nullptr);
    
}
// ----------------------------------------

// ----------------------------------------
// ----------------------------------------
// ----------------------------------------
// ----------------------------------------

// ==================================================
// WORKSPACE
// ==================================================
