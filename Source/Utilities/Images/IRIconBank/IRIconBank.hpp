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
