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
    
    void onResized() override;
    void resizeThisComponentEvent(const MouseEvent& e) override;
    void resizeThisComponent(Rectangle<int> rect);
    void resizeAndCentredThisComponent(Rectangle<int> rect);

    void setFixObjectSizeRatioWithOriginalSize(bool flag, Rectangle<float> originalSize);

    void paint(Graphics& g) override;
    // --------------------------------------------------

    void mouseEnterEvent(const MouseEvent& e) override;
    void mouseExitEvent(const MouseEvent& e) override;
    
    void mouseMove(const MouseEvent& e) override;
    // --------------------------------------------------

    virtual void videoLoadCompletedCallback();
    
    bool hasVideo() const;
    
    // --------------------------------------------------
    
    virtual IRNodeObject* copyThisObject() override;
    virtual t_json saveThisToSaveData() override;
    virtual void loadThisFromSaveData(t_json data) override;
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

    bool enableControllerFlag = true;
    
    // --------------------------------------------------
    // Object
    IRNodeObject* createTextObject(Component* event);
    IRNodeObject* createShapeObject(Component* event);
    IRNodeObject* createImageObject(Component* event);
    
    void moveToFrontAction() override;
    void moveToBackAction() override;
    
    // --------------------------------------------------

    IROnVideoPlayerController* getPlayerController() { return this->playerController.get(); }
    // --------------------------------------------------
private:
    
    std::shared_ptr<IROnVideoPlayerController> playerController;
    void showPlayerController(bool flag);
    // --------------------------------------------------

    void statusChangedCallback(IRNodeComponentStatus status) override;
    
    void eidtModeChangedAction();

    // --------------------------------------------------

    std::shared_ptr<IRVideoAnnotaterWorkspace> workspace;
    
    void videoLoadCompletedAction();
public:
    void videoPlayingUpdateAction(double pos);
    
};
#endif /* IRVideoAnnotaterObject2_hpp */
