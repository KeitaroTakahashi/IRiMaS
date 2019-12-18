//
//  VideoAnnotationEventComponent.hpp
//  IRiMaS
//
//  Created by Keitaro on 11/12/2019.
//

#ifndef VideoAnnotationEventComponent_hpp
#define VideoAnnotationEventComponent_hpp

#include "IRStrComponent.hpp"

class VideoAnnotationEventComponent : public Component,
public IRStrComponent
{
public:
    VideoAnnotationEventComponent(IRStr* str);
    ~VideoAnnotationEventComponent();
    
    // ==================================================
    // ==================================================
    virtual void paint(Graphics& g ) override;
    virtual void resized() override;
    // ==================================================
    
    void mouseDown(const MouseEvent& e) override;
    // ==================================================
    void setSelected(bool flag) { this->isSelectedFlag = flag; }
    bool isSelected() const { return this->isSelectedFlag; }
    // ==================================================

    
private:
    bool isSelectedFlag = false;
    // ==================================================
    // ==================================================
    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoAnnotationEventComponent)

    
};
#endif /* VideoAnnotationEventComponent_hpp */
