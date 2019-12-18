//
//  LoadingBar.hpp
//  IRiMaS
//
//  Created by Keitaro on 17/12/2019.
//

#ifndef LoadingBar_hpp
#define LoadingBar_hpp

#include "JuceHeader.h"
#include "IRStrComponent.hpp"
#include "KeAnimationComponent.h"

class LoadingBar : public Component,
public IRStrComponent,
public KeAnimationComponent
{
public:
    LoadingBar(IRStr* str);
    ~LoadingBar();
    
    // ==================================================
    void resized() override;
    void paint(Graphics& g) override;
    // ==================================================
    void setCompleteValue(float val);
    void setCurrentVal(float currentVal);

    // initialise all values and stop animation
    void initialise();
    
    // ==================================================

    // start animation
    void start();
    void pause();
    // ==================================================

    float getProgressRate() const;

    // ==================================================

private:
    // ==================================================

    float rate = 0.0;
    
    float completeVal = 0.0;
    float currentVal = 0.0;
    // ==================================================
    
    void updateAnimationFrame() override;

    // ==================================================
    // ==================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LoadingBar)

};

#endif /* LoadingBar_hpp */
