//
//  SlideMenu.cpp
//  IRiMaS
//
//  Created by Keitaro on 17/09/2019.
//

#include "SlideMenu.hpp"

SlideMenu::SlideMenu(IRStr* str) :
IRStrComponent(str)
{
    
}

SlideMenu::~SlideMenu()
{
    for(auto item : this->slides)
    {
        delete item;
    }
}

void SlideMenu::resized()
{
    int y = this->yMargin;
    for(auto item : this->slides)
    {
        item->setBounds(0, y, getWidth(), this->itemHeight);
        y += this->itemHeight;
    }
}

void SlideMenu::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
    g.setColour(getStr()->SYSTEMCOLOUR.text);
    //g.drawLine(0, 0, getWidth(), 0, 2);
    

}

void SlideMenu::addSlide(IRWorkspace* space)
{
    int i = (int)this->slides.size() + 1;
    this->slides.push_back(new IRWorkspaceSlide(getStr(), i, space));
    addAndMakeVisible(this->slides[i-1]);
    this->slides[i-1]->onClick = [this](IRWorkspaceSlide* slide){ slideSelectedAction(slide); };
    
    slideSelectedAction(this->slides[i-1]);
    
    resized();

}

void SlideMenu::slideSelectedAction(IRWorkspaceSlide* slide)
{
    for(auto item : this->slides)
    {
        item->setSelectedFlag(false);
    }
    
    slide->setSelectedFlag(true);
    
    if(this->slideHasSelected != nullptr)
        this->slideHasSelected(slide);
    
}

void SlideMenu::addNewWorkspaceSlide(IRWorkspace* space)
{
    std::cout << "SlideMenu::addNewWorkspaceSlide : " << space << std::endl;
    addSlide(space);
    int v_space = this->yMargin + (this->itemHeight * (int)this->slides.size());
    
    setSize(getWidth(), v_space);
    
    if(this->slideMenuUpdated != nullptr)
        this->slideMenuUpdated();
}

