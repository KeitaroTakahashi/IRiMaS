//
//  ImageMenuComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 12/08/2019.
//

#include "ImageMenuComponent.hpp"


ImageMenuComponent::ImageMenuComponent(Rectangle<int> frameRect) :
ObjectMenuComponent(frameRect)
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
