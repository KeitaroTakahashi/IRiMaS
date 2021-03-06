//
//  IRRightBar.cpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#include "IRRightBar.hpp"

IRRightBar::IRRightBar(IRStr* str) : IRStrComponent(str)
{
    setFps(60);
    
    //OpenGL
    this->openGLContext.setContinuousRepainting(false);
    this->openGLContext.attachTo (*this);
    
    // add keyListener of IRWindowComponent
    addKeyListener(str->key);
}

IRRightBar::~IRRightBar()
{
    this->openGLContext.detach();
}

//==================================================

void IRRightBar::resized()
{
    
    if(this->comp != nullptr)
    {
        if(this->comp->isVisible())
            this->comp->setBounds(getLocalBounds());
    }
    
}

void IRRightBar::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.contents);
}
//==================================================

void IRRightBar::openSpaceAction(bool flag)
{
    if(flag) this->openSpace = true;
    else this->openSpace = false;
 
    startAnimation();
    
}

void IRRightBar::updateAnimationFrame()
{
    if(this->openSpace)
    {
        
        setBounds(this->initialPos.getX() - this->space,
                  this->initialPos.getY(),
                  this->space, getHeight());
        this->isOpened = true;
        stopAnimation();
    }else
    {
      
        setBounds(this->initialPos.getX(),
                  this->initialPos.getY(),
                  0, getHeight());
        this->isOpened = false;
        stopAnimation();
    }
    
}

//==================================================
void IRRightBar::addComponentAndMakeVisible(Component& comp)
{
    // give mouse Listener
    //comp.addMouseListener(getStr()->mouse, false);
    addAndMakeVisible(comp);
    resized();
}

//==================================================

void IRRightBar::changeListenerCallback (ChangeBroadcaster* source)
{
    
}

void IRRightBar::nodeObjectSelectionChange(IRNodeObject* obj)
{

}

void IRRightBar::nodeObjectGetFocused(IRNodeObject* obj)
{
    if(this->comp != nullptr)
    {
        removeChildComponent(this->comp);
        this->comp = nullptr;
    }
    
    if(obj != nullptr)
    {
        
        this->comp = obj->getObjController();
        if(this->comp != nullptr)
            addComponentAndMakeVisible(*this->comp);
    
    }
    
    //std::cout << "nodeObjectGetFocused "<< obj << " : " << this->comp << std::endl;

}

//==================================================
