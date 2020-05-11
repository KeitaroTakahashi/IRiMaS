//
//  VideoAnotaterWorkspace.cpp
//  IRiMaS
//
//  Created by Keitaro on 01/03/2020.
//

#include "VideoAnnotaterWorkspace.hpp"

VideoAnnotaterWorkspace::VideoAnnotaterWorkspace(String title, Rectangle<int> draggableMargin, IRStr* str) :
IRWorkspaceComponent(title, draggableMargin, str)
{
    enableDrawGrids(true);
    setGridsBackgroundAlpha(0.2);
    
}

VideoAnnotaterWorkspace::~VideoAnnotaterWorkspace()
{
}

void VideoAnnotaterWorkspace::onPaint(Graphics& g)
{
}

void VideoAnnotaterWorkspace::setFixObjectSizeRatioWithOriginalSize(bool flag, Rectangle<float> originalSize)
{
    setFixObjectSizeRatio(flag, originalSize);

}
