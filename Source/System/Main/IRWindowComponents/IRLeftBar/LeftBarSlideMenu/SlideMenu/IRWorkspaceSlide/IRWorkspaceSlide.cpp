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

void IRWorkspaceSlide::mouseDown(const MouseEvent& e)
{
    
    if(this->onClick != nullptr) this->onClick(this);
    
    std::cout <<"mouseDown\n";
    
}

