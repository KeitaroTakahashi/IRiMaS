//
//  IRImage.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 01/03/2019.
//

#include "IRImage.hpp"
// --------------------------------------------------
IRImage::IRImage(File file)
{
    loadImage(file);
}
// --------------------------------------------------
IRImage::~IRImage()
{
    
}
// --------------------------------------------------
void IRImage::loadImage(File file)
{
    this->img = ImageFileFormat::loadFrom(file);
}
// --------------------------------------------------
Image IRImage::getImageData() const
{
    return this->img;
}
// --------------------------------------------------
int IRImage::getHeight()
{
    return this->img.getHeight();
}
// --------------------------------------------------

int IRImage::getWidth()
{
    return this->img.getWidth();
}
// --------------------------------------------------
