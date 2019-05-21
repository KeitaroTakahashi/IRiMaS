//
//  IRIconBank.cpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 02/05/2019.
//

#include "IRIconBank.hpp"

IRIconBank::IRIconBank()
{
    loadImages();
}

IRIconBank::~IRIconBank()
{
    
}

Image IRIconBank::loadImageAndReturn(String url)
{
#if JUCE_MAC
    return loadImage("/Contents/Resources" + url);
#elif JUCE_IOS
    return loadImage(url);
#endif
    return Image();
}

void IRIconBank::loadImages()
{
    String url = "/materials/Images/icon/menu/icon_zoomIn_white.png";
    this->icon_zoomIn = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_zoomOut_white.png";
    this->icon_zoomOut = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_horizontalArrow_white.png";
    this->icon_horizontalMovable = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_verticalArrow_white.png";
    this->icon_verticalMovable = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_crossArrow_white.png";
    this->icon_crossMovable = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_stop_white.png";
    this->icon_notMovable = loadImageAndReturn(url);
}


Image IRIconBank::loadImage(String url)
{
    File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + url);
    return ImageFileFormat::loadFrom(file);
}
