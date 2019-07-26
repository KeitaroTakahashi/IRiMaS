//
//  IRSpectrogramComponent.hpp
//  IRiMaS
//
//  Created by Keitaro on 25/07/2019.
//

#ifndef IRSpectrogramComponent_hpp
#define IRSpectrogramComponent_hpp

#include "IRSpectrogram.hpp"

class IRSpectrogramComponent : public Component
{
public:
    IRSpectrogramComponent(IRNodeObject* nodeObject);
    ~IRSpectrogramComponent();
    
    // ==================================================
    void resized() override;
    void paint(Graphics& g) override;
    
    IRSpectrogram* getComponent() { return this->spectrogram.get(); }
    
    // ==================================================

    void setMainComponentBounds(Rectangle<int> area);
    
    // ==================================================

    void setVisibleArea(Rectangle<int> area);
    
    
private:
    // ==================================================

    std::shared_ptr<IRSpectrogram> spectrogram;
    
};
#endif /* IRSpectrogramComponent_hpp */
