//
//  IRIconBank
//  InteractiveAutomation - App
//
//  Created by Keitaro on 02/05/2019.
//

#ifndef IRIconBank_hpp
#define IRIconBank_hpp

#include "JuceHeader.h"
#include "singletonClass.hpp"

class IRIconBank
{
    
public:
    
    struct IRIconImage
    {
        Image white;
        Image black;
    };
    
    IRIconBank();
    ~IRIconBank();
    
    void loadImages();
    
    IRIconImage getZoomIn() const { return this->icon_zoomIn; }
    IRIconImage getZoomOut() const { return this->icon_zoomOut; }
    IRIconImage getHorizontalMovable() const { return this->icon_horizontalMovable; }
    IRIconImage getVerticalMovable() const { return this->icon_verticalMovable; }
    IRIconImage getCrossMovable() const { return this->icon_crossMovable; }
    IRIconImage getNotMovable() const { return this->icon_notMovable; }
    IRIconImage getComment() const { return this->icon_comment; }
    IRIconImage getBezier() const { return this->icon_bezier; }
    IRIconImage getLinear() const { return this->icon_linear; }

    IRIconImage loadImageAndReturn(String url);
    
    
    IRIconImage icon_addPage;
    IRIconImage icon_addPage2;
    IRIconImage icon_inspector;
    IRIconImage icon_load;
    IRIconImage icon_loop;
    IRIconImage icon_newProject;
    IRIconImage icon_object;
    IRIconImage icon_preference;
    IRIconImage icon_save;
    IRIconImage icon_toNavigator;
    IRIconImage icon_toObjectMenu;
    
    IRIconImage icon_text;
    IRIconImage icon_chart;
    IRIconImage icon_image;
    IRIconImage icon_play;
    IRIconImage icon_wav;
    

    
private:
    
    IRIconImage loadImage(String url);
    IRIconImage icon_zoomIn;
    IRIconImage icon_zoomOut;
    
    IRIconImage icon_horizontalMovable;
    IRIconImage icon_verticalMovable;
    IRIconImage icon_crossMovable;
    IRIconImage icon_notMovable;
    
    IRIconImage icon_comment;
    IRIconImage icon_bezier;
    IRIconImage icon_linear;
    
    
    

    
};
#endif /* IRIconBank_hpp */
