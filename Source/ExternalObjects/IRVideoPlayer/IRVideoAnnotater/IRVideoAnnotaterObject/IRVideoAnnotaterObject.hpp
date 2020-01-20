//
//  IRVideoAnnotaterObject.hpp
//  IRiMaS
//
//  Created by Keitaro on 13/12/2019.
//

#ifndef IRVideoAnnotaterObject_hpp
#define IRVideoAnnotaterObject_hpp

#include "IRVideoPlayerObject.hpp"
#include "IRVideoAnnotationComponentHeader.h"
#include "VideoAnnotationEventComponent.hpp"


class IRVideoAnnotaterObject : public IRVideoPlayerObject
{
public:
    
    IRVideoAnnotaterObject(Component* parent, IRStr* str, bool withOpenButton = true);
    ~IRVideoAnnotaterObject();
    // --------------------------------------------------
    void resized() override;
    // --------------------------------------------------
    
    void setAnnotationEvents(std::vector<VideoAnnotationEventComponent*> events);
    // --------------------------------------------------

    void videoPlayingUpdateCallback(double pos) override;

    
private:
    // --------------------------------------------------
    std::vector<IRVideoAnnotationComponent* > annotationComponents;
    std::vector<IRVideoAnnotationComponent* > visibleAnnotationComponents;
    void clearVisibleAnnotationComponents();
    void addVisibleAnnotationComponents(IRVideoAnnotationComponent* visibleComponent);
    void removeVisibleAnnotationComponents(IRVideoAnnotationComponent* visibleComponent);

    // --------------------------------------------------
    void clearAnnotationComponent();
    void createAnnotationComponent(VideoAnnotationEventComponent* event);
    void addAnnotationComponent(IRVideoAnnotationComponent* component);
    void createTEXTAnnotationComponent(VideoAnnotationEventComponent* event);
    // --------------------------------------------------

    
    // --------------------------------------------------
    // --------------------------------------------------
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoAnnotaterObject)

};

#endif /* IRVideoAnnotaterObject_hpp */
