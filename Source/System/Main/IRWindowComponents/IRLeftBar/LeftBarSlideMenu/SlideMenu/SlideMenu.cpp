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
    addSlide();
    addSlide();
    addSlide();
    addSlide();
    addSlide();


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
    g.drawLine(0, 0, getWidth(), 0, 2);
    

}

void SlideMenu::addSlide()
{
    int i = (int)this->slides.size() + 1;
    this->slides.push_back(new IRWorkspaceSlide(getStr(), i));
    addAndMakeVisible(this->slides[i-1]);
    this->slides[i-1]->onClick = [this]{ slideSelectedAction(); };
}

void SlideMenu::slideSelectedAction()
{
    for(auto item : this->slides)
    {
        item->setSelectedFlag(false);
    }
    
    std::cout <<"slideSelectedAction\n";
}
