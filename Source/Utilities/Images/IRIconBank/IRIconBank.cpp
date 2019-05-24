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

IRIconBank::IRIconImage IRIconBank::loadImageAndReturn(String url)
{
#if JUCE_MAC
    return loadImage("/Contents/Resources" + url);
#elif JUCE_IOS
    return loadImage(url);
#endif
    return IRIconImage();
}

void IRIconBank::loadImages()
{
    String url = "/materials/Images/icon/menu/icon_zoomIn";
    this->icon_zoomIn = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_zoomOut";
    this->icon_zoomOut = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_horizontalArrow";
    this->icon_horizontalMovable = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_verticalArrow";
    this->icon_verticalMovable = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_crossArrow";
    this->icon_crossMovable = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_stop";
    this->icon_notMovable = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_comment";
    this->icon_comment = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_bezier";
    this->icon_bezier = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_linear";
    this->icon_linear = loadImageAndReturn(url);
}


IRIconBank::IRIconImage IRIconBank::loadImage(String url)
{
    IRIconImage img;
    
    String url_white = url + "_white.png";
    String url_black = url + "_black.png";
    
    File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + url_white);
    img.white = ImageFileFormat::loadFrom(file);
    
    file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + url_black);
    img.black = ImageFileFormat::loadFrom(file);
    
    return img;
}
