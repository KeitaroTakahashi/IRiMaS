//
//  IRViewPort.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 21/05/2019.
//

#include "IRViewPort.hpp"

IRViewPort::IRViewPort()
{
    ScrollBar& bar = getHorizontalScrollBar();
    bar.setColour(ScrollBar::ColourIds::thumbColourId, SYSTEMCOLOUR.fundamental);

}

IRViewPort::~IRViewPort()
{
    
}


void IRViewPort::visibleAreaChanged (const Rectangle< int > &newVisibleArea)
{
    visibleAreaChangedAction(newVisibleArea);

    if(this->visibleAreaChangedCallback != nullptr)
        this->visibleAreaChangedCallback(newVisibleArea);
 
}

void IRViewPort::viewedComponentChanged (Component *newComponent)
{
    if(this->viewedComponentChangedCallback != nullptr)
        this->viewedComponentChangedCallback();
}

