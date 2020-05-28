//
//  IRVideoAnnotaterObject2.hpp
//  IRiMaS
//
//  Created by Keitaro on 10/05/2020.
//

#ifndef IRVideoAnnotaterObject2_hpp
#define IRVideoAnnotaterObject2_hpp

#include "IRVideoAnnotaterWorkspace.hpp"


class IRVideoAnnotaterObject2 : public IRNodeObject
{
public:
    IRVideoAnnotaterObject2(Component* parent, IRStr* str, bool withOpenButton = true);

    ~IRVideoAnnotaterObject2();
    
    void resized() override;
    void resizeThisComponentEvent(const MouseEvent& e) override;
    void resizeThisComponent(Rectangle<int> rect);
    void resizeAndCentredThisComponent(Rectangle<int> rect);

    void setFixObjectSizeRatioWithOriginalSize(bool flag, Rectangle<float> originalSize);

    void paint(Graphics& g) override;
    
    virtual void videoLoadCompletedCallback();
    
    // --------------------------------------------------
    
    IRNodeObject* copyThis() override;
    t_json saveThisToSaveData() override;
    void loadThisFromSaveData(t_json data) override;
    // --------------------------------------------------
    // define if call videoLoadCompletedCallbackFunc();
    void openFile(File file, bool isCallback = true);
    void openFile(bool isCallbback = true);

    std::function<void()> videoLoadCompletedCallbackFunc;
    std::function<void(double pos)> videoPlayingUpdateCallbackFunc;
   // virtual void videoLoadCompletedCallback() {}
    //void videoPlayingUpdateCallback(double pos) {}
    // --------------------------------------------------
    void play();
    void stop();
    void setPlayPosition(double newPositionInSec);
    
    juce::Point<int> getVideoSize();
    // --------------------------------------------------

    IRVideoPlayerObject2* getVideoPlayerObject();

    IRVideoAnnotaterWorkspace* getWorkspace() { return this->workspace.get(); }
    // --------------------------------------------------
    void enableController(bool flag);

    
    // --------------------------------------------------
    // Object
    void createTextObject(Component* event);
    void createShapeObject(Component* event);
    void createImageObject(Component* event);
    
    void moveToFrontAction() override;
    void moveToBackAction() override;


private:
    // --------------------------------------------------

    
    
    // --------------------------------------------------

    std::shared_ptr<IRVideoAnnotaterWorkspace> workspace;
    
    void videoLoadCompletedAction();
    void videoPlayingUpdateAction(double pos);
    
};
#endif /* IRVideoAnnotaterObject2_hpp */
