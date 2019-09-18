//
//  ImageMenuComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 12/08/2019.
//

#include "ImageMenuComponent.hpp"


ImageMenuComponent::ImageMenuComponent(IRStr* str, Rectangle<int> frameRect) :
ObjectMenuComponent(str, frameRect)
{
    setTitleText("Image");
}

ImageMenuComponent::~ImageMenuComponent()
{
    
}


// ==================================================
void ImageMenuComponent::paint(Graphics& g)
{
    g.fillAll(SYSTEMCOLOUR.contents);
}

void ImageMenuComponent::resized()
{
    ObjectMenuComponent::resized();
}
// ==================================================



// ==================================================
// ==================================================
// ==================================================
