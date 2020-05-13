//
//  IRVideoAnnotaterObject2.hpp
//  IRiMaS
//
//  Created by Keitaro on 10/05/2020.
//

#ifndef IRVideoAnnotaterObject2_hpp
#define IRVideoAnnotaterObject2_hpp

#include "IRVideoAnnotaterWorkspace.hpp"
#include "IRVATextEditorObject.h"
#include "IRObjectCreater.hpp"

#include "AnnotationTextEventComponent.h"
#include "VideoAnnotationEventComponent.h"

class IRVideoAnnotaterObject2 : public IRNodeObject
{
public:
    IRVideoAnnotaterObject2(Component* parent, IRStr* str, bool withOpenButton = true);

    ~IRVideoAnnotaterObject2();
    
    void resized() override;
    
    void paint(Graphics& g) override;
    
    virtual void videoLoadCompletedCallback();
    
    // --------------------------------------------------
    // --------------------------------------------------
    // define if call videoLoadCompletedCallbackFunc();
    void openFile(File file, bool isCallback = true);
    void openFile(bool isCallbback = true);

    std::function<void()> videoLoadCompletedCallbackFunc;
   // virtual void videoLoadCompletedCallback() {}
    //virtual void videoPlayingUpdateCallback(double pos) {}
    // --------------------------------------------------
    void play();
    void stop();
    void setPlayPosition(double newPositionInSec);
    
    juce::Point<int> getVideoSize();
    // --------------------------------------------------

    IRVideoPlayerObject2* getVideoPlayerObject();


    // --------------------------------------------------
    void enableController(bool flag);
    // --------------------------------------------------
    // Annotation
    void createAnnotationComponent(VideoAnnotationEventComponent* event);
    void createTEXTAnnotationComponent(VideoAnnotationEventComponent* event);
    // --------------------------------------------------
    void createTextObject();

private:
    
    
    void createNodeObjectOnWorkspace(IRNodeObject* obj);
    
    
    std::shared_ptr<IRVideoAnnotaterWorkspace> workspace;
    
};
#endif /* IRVideoAnnotaterObject2_hpp */
