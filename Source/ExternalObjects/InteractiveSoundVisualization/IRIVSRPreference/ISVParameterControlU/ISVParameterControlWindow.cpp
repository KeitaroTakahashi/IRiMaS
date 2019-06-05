//
//  ISVParameterControlWindow.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 03/06/2019.
//

#include "ISVParameterControlWindow.hpp"

ISVParameterControlWindow::ISVParameterControlWindow(ISVParameterControlUI* ui, String name, Rectangle<int> frameRect) :
DocumentWindow(name, Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId), DocumentWindow::allButtons)
{
    this->ui = ui;
    this->name = name;
    setUsingNativeTitleBar(true);
    this->controlUI = std::make_shared<ISVParameterControlUI>();
    this->controlUI->setBounds(frameRect.getX(),
                               frameRect.getY(),
                               frameRect.getWidth(),
                               frameRect.getHeight());
    this->controlUI->addChangeListener(this);
    setContentOwned(this->controlUI.get(), true);
    
    setResizable(true, true);
    setName(name);
    
    Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    
    int y = r.getY();
    int w = r.getWidth();
    setBounds(w - getWidth(), y, getWidth(), getHeight());
    
    setVisible(true);
}

ISVParameterControlWindow::~ISVParameterControlWindow()
{
    
}


void ISVParameterControlWindow::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == this->controlUI.get())
    {
        if(this->controlUI->getStatus() == ISVParameterControlUI::ISVParameterControlUIStatus::SliderValueChanged)
        {
            
        }
    }
}

void ISVParameterControlWindow::closeButtonPressed()
{
    removeFromDesktop();
}

void ISVParameterControlWindow::show()
{
    addToDesktop();
}
