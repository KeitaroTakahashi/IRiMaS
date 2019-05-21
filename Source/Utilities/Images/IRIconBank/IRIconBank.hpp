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
    IRIconBank();
    ~IRIconBank();
    
    void loadImages();
    
    Image getZoomIn() const { return this->icon_zoomIn; }
    Image getZoomOut() const { return this->icon_zoomOut; }
    Image getHorizontalMovable() const { return this->icon_horizontalMovable; }
    Image getVerticalMovable() const { return this->icon_verticalMovable; }
    Image getCrossMovable() const { return this->icon_crossMovable; }
    Image getNotMovable() const { return this->icon_notMovable; }
    
    
    Image loadImageAndReturn(String url);
    
private:
    
    Image loadImage(String url);
    Image icon_zoomIn;
    Image icon_zoomOut;
    
    Image icon_horizontalMovable;
    Image icon_verticalMovable;
    Image icon_crossMovable;
    Image icon_notMovable;
    
    
};
#endif /* IRIconBank_hpp */
