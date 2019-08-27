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
    
    
    // icons
    url = "/materials/Images/icon/menu/icon_addPage";
    this->icon_addPage = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_addPage2";
    this->icon_addPage2 = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_inspector";
    this->icon_inspector = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_load";
    this->icon_load = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_loop";
    this->icon_loop = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_NewProject";
    this->icon_newProject = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_object";
    this->icon_object = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_preference";
    this->icon_preference = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_save";
    this->icon_save = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_toNavigator";
    this->icon_toNavigator = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_toObjectMenu";
    this->icon_toObjectMenu = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_text";
    this->icon_text = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_data";
    this->icon_chart = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_img";
    this->icon_image = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_play";
    this->icon_play = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_wav";
    this->icon_wav = loadImageAndReturn(url);
}


IRIconBank::IRIconImage IRIconBank::loadImage(String url)
{
    IRIconImage img;
    
    String url_white = url + "_white.png";
    String url_black = url + ".png";
    
    File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + url_white);
    if(!file.exists()) std::cout << "Error : IRIconBank : file does not exist!! " << file.getFullPathName() << std::endl;
    
    img.white = ImageFileFormat::loadFrom(file);
    
    file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + url_black);
    if(!file.exists()) std::cout << "Error : IRIconBank : file does not exist!! " << file.getFullPathName() << std::endl;

    img.black = ImageFileFormat::loadFrom(file);
    
    return img;
}
