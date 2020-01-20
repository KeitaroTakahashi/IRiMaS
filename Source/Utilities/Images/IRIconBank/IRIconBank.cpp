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
    
    url = "/materials/Images/icon/menu/icon_minus";
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
    
    url = "/materials/Images/icon/menu/icon_pause";
    this->icon_pause = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_stop";
    this->icon_stop = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_wav";
    this->icon_wav = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_rightBar";
    this->icon_rightBar = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_leftBar";
    this->icon_leftBar = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_newSlide_noFrame";
    this->icon_newSlide_noFrame = loadImageAndReturn(url);
        
    url = "/materials/Images/icon/menu/icon_openProject_noFrame";
    this->icon_openProject_noFrame = loadImageAndReturn(url);

    
    url = "/materials/Images/icon/menu/icon_saveProject_noFrame";
    this->icon_saveProject_noFrame = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_saveasProject_noFrame";
    this->icon_saveasProject_noFrame = loadImageAndReturn(url);
    
    
    url = "/materials/Images/icon/menu/icon_saveProject_arrow";
    this->icon_saveProject_arrow = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_saveasProject_arrow";
    this->icon_saveasProject_arrow = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_openProject_arrow";
    this->icon_openProject_arrow = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_close";
    this->icon_close = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_newSlide";
    this->icon_newSlide = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_deleteSlide";
    this->icon_deleteSlide = loadImageAndReturn(url);

    url = "/materials/Images/icon/menu/icon_audioEvent";
    this->icon_AudioEvent = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_ImageEvent";
    this->icon_ImageEvent = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_ShapeEvent";
    this->icon_ShapeEvent = loadImageAndReturn(url);
    
    url = "/materials/Images/icon/menu/icon_TextEvent";
    this->icon_TextEvent = loadImageAndReturn(url);
    
    
    // square Icon
    url = "/materials/Images/icon/menu/icon_active";
    this->icon_active = loadImageAndReturn(url);
    
    
    // logo
    this->logo_darkBlue = loadLogoImage("logo_15.png");
    this->logo_darkGreen = loadLogoImage("logo_14.png");
    this->logo_darkMagenta = loadLogoImage("logo_10.png");

 
}


IRIconBank::IRIconImage IRIconBank::loadImage(String url)
{
    IRIconImage img;
    
    String url_white = url + "_white.png";
    String url_black = url + ".png";
    String url_gray  = url + "_gray.png";
    
    // white image
    File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + url_white);
    if(!file.exists()) std::cout << "Error : IRIconBank : file does not exist!! " << file.getFullPathName() << std::endl;
    
    img.white = ImageFileFormat::loadFrom(file);
    
    // black image

    file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + url_black);
    if(!file.exists()) std::cout << "Error : IRIconBank : file does not exist!! " << file.getFullPathName() << std::endl;

    img.black = ImageFileFormat::loadFrom(file);
    
    // gray image

    file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + url_gray);
    if(!file.exists()) std::cout << "Error : IRIconBank : file does not exist!! " << file.getFullPathName() << std::endl;

    img.gray = ImageFileFormat::loadFrom(file);
    
    return img;
}

IRIconBank::IRLogoImage IRIconBank::loadLogoImage(String fileName)
{
    IRLogoImage logo;
    
    String base;

    #if JUCE_MAC
        base = "/Contents/Resources/materials/Images/Logo/";
    #elif JUCE_IOS
        base = "/materials/Images/Logo/";
    #endif
    
    // small image
    File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + base + "small/" + fileName);
    if(!file.exists()) std::cout << "Error : IRIconBank : file does not exist!! " << file.getFullPathName() << std::endl;
    logo.small = ImageFileFormat::loadFrom(file);
    
    // small image
    file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + base + "medium/" + fileName);
    if(!file.exists()) std::cout << "Error : IRIconBank : file does not exist!! " << file.getFullPathName() << std::endl;
    logo.medium = ImageFileFormat::loadFrom(file);
    
    // small image
    file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + base + "large/" + fileName);
    if(!file.exists()) std::cout << "Error : IRIconBank : file does not exist!! " << file.getFullPathName() << std::endl;
    logo.large = ImageFileFormat::loadFrom(file);
    
    return logo;
}
