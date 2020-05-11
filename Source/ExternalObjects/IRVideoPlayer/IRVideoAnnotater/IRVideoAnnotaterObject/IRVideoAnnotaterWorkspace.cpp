//
//  IRVideoAnnotaterWorkspace.cpp
//  IRiMaS
//
//  Created by Keitaro on 09/05/2020.
//

#include "IRVideoAnnotaterWorkspace.hpp"

IRVideoAnnotaterWorkspace::IRVideoAnnotaterWorkspace(String title, Rectangle<int> draggableMargin, IRStr* str) :
IRNodeObjectWorkspace(title, draggableMargin, str)
{
    enableDrawGrids(true);

    // initially create videoplayer and add
    this->videoPlayerObj.reset( new IRVideoPlayerObject2(this, str, true) );
    addAndMakeVisible(this->videoPlayerObj.get());
    
}

IRVideoAnnotaterWorkspace::~IRVideoAnnotaterWorkspace()
{
    this->videoPlayerObj.reset();
}

void IRVideoAnnotaterWorkspace::onResized()
{
    
    std::cout << "IRVideoAnnotaterWorkspace onResized\n";
    this->videoPlayerObj->setObjectBounds(getLocalBounds());
}

void IRVideoAnnotaterWorkspace::onPaint(Graphics& g)
{
    std::cout << "print\n";
    
    g.fillAll(Colours::blue);
}

void IRVideoAnnotaterWorkspace::addAnnotationObject(IRNodeObject* obj, Rectangle<int> bounds)
{
    
}
