//
//  IRWorkspaceSlide.cpp
//  IRiMaS
//
//  Created by Keitaro on 17/09/2019.
//

#include "IRWorkspaceSlide.hpp"

IRWorkspaceSlide::IRWorkspaceSlide(IRStr* str, int index, IRWorkspace* space) :
IRStrComponent(str),
space(space),
index(index)
{
    
}

IRWorkspaceSlide::~IRWorkspaceSlide()
{
    
}

void IRWorkspaceSlide::paint(Graphics& g)
{
    if(isSelected())
    {
        g.fillAll(getStr()->SYSTEMCOLOUR.contents);
        
        //std::cout << "getStr()->SYSTEMCOLOUR.contents colour = " << getStr()->SYSTEMCOLOUR.colourName << std::endl;
    }else{
        g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
    }
    
    g.setColour(getStr()->SYSTEMCOLOUR.text);
    g.drawText(String(this->index), 0, 0, getWidth(), getHeight(), Justification::centred);
    
}
// ==================================================

void IRWorkspaceSlide::mouseDown(const MouseEvent& e)
{
   if(!isSelected())
   {
       if(this->onClick != nullptr) this->onClick(this);
   }
    
    toFront(true);
    std::cout <<"mouseDown\n";
    
}

void IRWorkspaceSlide::mouseUp(const MouseEvent& e)
{
    if(this->slideSwitchedCallback != nullptr)
        this->slideSwitchedCallback(this);
       // This is important, otherwise an object on the selected workspace will have front focus and gets keyboard Focus.
       // to get focus of keyboard on this object, then we need to call toFront method
       toFront(true);
}


// ==================================================

void IRWorkspaceSlide::deleteThisWorkspace()
{
    
}
// ==================================================

void IRWorkspaceSlide::setIndex(int newIndex)
{
    this->index = newIndex;
    repaint();
}
