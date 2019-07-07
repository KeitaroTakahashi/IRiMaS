//
//  IRSpectrogramPreferenceUI.hpp
//  IRiMaS
//
//  Created by Keitaro on 07/07/2019.
//

#ifndef IRSpectrogramPreferenceUI_hpp
#define IRSpectrogramPreferenceUI_hpp
#include "JuceHeader.h"

class IRSpectrogramPreferenceUI : public Component
{
public:
    IRSpectrogramPreferenceUI();
    ~IRSpectrogramPreferenceUI();
    
    void resized() override;
    
    void paint(Graphics& g) override {}
    
private:
    
    
};
#endif /* IRSpectrogramPreferenceUI_hpp */
