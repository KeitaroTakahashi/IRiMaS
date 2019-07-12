//
//  IRViewUI.cpp
//  IRiMaS
//
//  Created by Keitaro on 12/07/2019.
//

#include "IRViewUI.hpp"

IRViewUI::IRViewUI(Component* main,
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
}

IRViewUI::~IRViewUI()
{
    
}


