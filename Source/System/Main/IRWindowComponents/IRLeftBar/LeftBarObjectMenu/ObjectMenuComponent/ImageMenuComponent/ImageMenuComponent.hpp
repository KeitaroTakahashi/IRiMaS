//
//  ImageMenuComponent.hpp
//  IRiMaS
//
//  Created by Keitaro on 12/08/2019.
//

#ifndef ImageMenuComponent_hpp
#define ImageMenuComponent_hpp


#include "ObjectMenuComponent.hpp"

class ImageMenuComponent : public ObjectMenuComponent
{
public:
    ImageMenuComponent(Rectangle<int> frameRect);
    ~ImageMenuComponent();
    // ==================================================
    
    void resized() override;
    void paint(Graphics& g) override;
    // ==================================================
    // ==================================================
    
private:
    // ==================================================
    
    
    
    // ==================================================
    
    // ==================================================
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImageMenuComponent)
    
};

#endif /* ImageMenuComponent_hpp */
