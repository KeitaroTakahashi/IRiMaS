//
//  IRWorkspaceSlide.cpp
//  IRiMaS
//
//  Created by Keitaro on 17/09/2019.
//

#include "IRWorkspaceSlide.hpp"

IRWorkspaceSlide::IRWorkspaceSlide(IRStr* str, int index) :
IRStrComponent(str),
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
    }else{
        g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
    }
    
    g.setColour(getStr()->SYSTEMCOLOUR.text);
    g.drawText(String(this->index), 0, 0, getWidth(), getHeight(), Justification::centred);
    
}

void IRWorkspaceSlide::mouseDown(const MouseEvent& e)
{
    
    if(this->onClick != nullptr) this->onClick();
    setSelectedFlag(true);
    repaint();
    std::cout <<"mouseDown\n";

}

