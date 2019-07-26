//
//  IRSpectrogramViewPort.cpp
//  IRiMaS
//
//  Created by Keitaro on 23/07/2019.
//

#include "IRSpectrogramViewPort.hpp"


IRSpectrogramViewUI::IRSpectrogramViewUI(IRSpectrogramComponent* main,
                                         float vmin, float vmax,
                                         float hmin, float hmax) :
verticalGrid(IRGridStr::IRMeasureGridType::VERTICAL),
horizontalGrid(IRGridStr::IRMeasureGridType::HORIZONTAL)
{
    
    this->verticalGrid.setRange(vmin, vmax);
    this->horizontalGrid.setRange(hmin, hmax);
    this->viewPort = std::make_shared<Component4ViewPort>(main,
                                                          &this->verticalGrid,
                                                          &this->horizontalGrid,
                                                          this->gridSize);
    setViewedComponent(this->viewPort.get());
    
    
}

IRSpectrogramViewUI::~IRSpectrogramViewUI()
{
    
}

// ==================================================

void IRSpectrogramViewUI::resized()
{
    
    //setMainComponentBounds(0, 0, getWidth(), getHeight());
}


// ==================================================


void IRSpectrogramViewUI::setComponentBounds(int x, int y, int w, int h)
{
    std::cout << "xywh = " << x << ", " << y << ", " << w << ", "<< h << std::endl;
    this->viewPort->setBounds(x, y, w, h);
}



void IRSpectrogramViewUI::setVisibleArea(Rectangle<int> area)
{
    this->viewPort->setVisibleArea(area);
}


void IRSpectrogramViewUI::visibleAreaChangedAction(const Rectangle< int > &newVisibleArea)
{
    
    this->viewPort->setHorizontalBounds(newVisibleArea.getX(),
                                        newVisibleArea.getWidth());
    
    
    if(this->visibleAreaChangedCallback != nullptr)
        this->visibleAreaChangedCallback(newVisibleArea);
}

void IRSpectrogramViewUI::setMainComponentBounds(int x, int y, int w, int h)
{
    this->viewPort->setMainComponentBounds(x, y, getWidth(), getHeight());
}
