//
//  TextMenuComponent.hpp
//  IRiMaS
//
//  Created by Keitaro on 12/08/2019.
//

#ifndef TextMenuComponent_hpp
#define TextMenuComponent_hpp

#include "ObjectMenuComponent.hpp"

class TextMenuComponent : public ObjectMenuComponent
{
public:
    TextMenuComponent(Rectangle<int> frameRect);
    ~TextMenuComponent();
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TextMenuComponent)

};

#endif /* TextMenuComponent_hpp */
