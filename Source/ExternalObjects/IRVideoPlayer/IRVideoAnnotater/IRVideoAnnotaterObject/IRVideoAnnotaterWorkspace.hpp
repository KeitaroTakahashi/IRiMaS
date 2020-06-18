//
//  IRVideoAnnotaterWorkspace.hpp
//  IRiMaS
//
//  Created by Keitaro on 09/05/2020.
//

#ifndef IRVideoAnnotaterWorkspace_hpp
#define IRVideoAnnotaterWorkspace_hpp

#include "IRNodeObjectWorkspace.hpp"
#include "IRVideoPlayerObject2.hpp"

#include "IRVATextEditorObject.h"
#include "IRVAShapeObject.h"
#include "IRVAImageViewerObject.h"
#include "IRObjectCreater.hpp"

#include "AnnotationTextEventComponent.h"
#include "VideoAnnotationEventComponent.h"

class IRVideoAnnotaterWorkspace : public IRNodeObjectWorkspace
{
public:
    IRVideoAnnotaterWorkspace(String title, Rectangle<int> draggableMargin, IRStr* str, bool withOpenButton = true);
    ~IRVideoAnnotaterWorkspace();
    
    void onResized() override;
    void onPaint(Graphics& g) override;
    
    // ==============================

    void addAnnotationObject(IRNodeObject* obj, Rectangle<int> bounds);
    
    // ==============================
    void copyAllDataToWorkspace(IRVideoAnnotaterWorkspace* newWorkspace);
    void replaceVideoPlayerObject(IRVideoPlayerObject2* newVideoPlayer);
    // ==============================
    IRVideoPlayerObject2* getVideoPlayerObject();
    
    void setFixObjectSizeRatioWithOriginalSize(bool flag, Rectangle<float> originalSize);
    
    // ==============================
    std::function<void()> videoLoadCompletedCallback;
    std::function<void(double)> videoPlayingUpdateCallback;

    // ==============================
    // Annotation
    void createAnnotationComponent(VideoAnnotationEventComponent* event);
    void createTEXTAnnotationComponent(VideoAnnotationEventComponent* event);
    // ==============================
    IRNodeObject* createTextObject(Component* event);
    IRNodeObject* createShapeObject(Component* event);
    IRNodeObject* createImageObject(Component* event);
    
    void createAnnotationObject(IRNodeObject* obj, Component* event);
    // ==============================
    
    void updateVideoPlayingPos(double pos);
    
    // ==============================

    void loadAndApplyIRSRT(t_json data);

private:
    // ==============================

    void createNodeObjectOnWorkspace(IRNodeObject* obj);
    void createNodeObjectOnworkspaceAsSubtitle(IRNodeObject* obj);
    // ==============================

    void videoLoadCompletedAction();
    void videoPlayingUpdateAction(double pos);
    std::shared_ptr<IRVideoPlayerObject2> videoPlayerObj;
    
    // ==============================

    
};
#endif /* IRVideoAnnotaterWorkspace_hpp */
