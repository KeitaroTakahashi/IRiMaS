//
//  IRVideoPlayerObject22.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/01/2019.
//

#ifndef IRVideoPlayerObject2_hpp
#define IRVideoPlayerObject2_hpp

#include "IRNodeObject.hpp"
#include "IRVideoPlayer.h"
#include "IRNodeObjectWorkspace.hpp"
#include "IRTextEditorObject.hpp"

class IRVideoPlayerObject2 : public IRNodeObject

{
    
public:
    
    IRVideoPlayerObject2(Component* parent, IRStr* str, bool withOpenButton = true);
    ~IRVideoPlayerObject2();
    
    
    // --------------------------------------------------
    // copy related methods
    
    virtual IRNodeObject* copyThis() override;
    
    virtual IRNodeObject* copyContents(IRNodeObject* object) override;
    
    virtual IRNodeObject* copyDragDropContents(IRNodeObject* object) override;
    
    // share object contents owned by this obejct to twithObject
    void shareContentsWith(IRVideoPlayerObject2* withObject);
    
    // --------------------------------------------------
    
    virtual t_json saveThisToSaveData() override;
    
    // --------------------------------------------------
    
    virtual void loadThisFromSaveData(t_json data) override;
    
    // --------------------------------------------------
    // resize reated methods
    virtual void resized() override;
    
    
    void setBoundsInWorkspace(Rectangle<int> rect) { this->boundsInWorkspace = rect; }
    Rectangle<int> getBoundsInWorkspace() const { return this->boundsInWorkspace; }
    
    // --------------------------------------------------
    
    void mouseUpEvent(const MouseEvent& e) override;
    
    // --------------------------------------------------

    void paint(Graphics& g) override;
    
    // --------------------------------------------------
    // define if call videoLoadCompletedCallbackFunc();
    void openFile(File file, bool isCallback = true);
    void openFile(bool isCallbback = true);

    std::function<void()> videoLoadCompletedCallbackFunc;
    virtual void videoLoadCompletedCallback() {}
    virtual void videoPlayingUpdateCallback(double pos) {}
    // --------------------------------------------------
    void play();
    void stop();
    void setPlayPosition(double newPositionInSec);
    
    juce::Point<int> getVideoSize();
    // --------------------------------------------------

    IRVideoPlayer* getVideoPlayer() { return this->videoPlayer.get(); }

    // --------------------------------------------------
    void enableController(bool flag);
    // --------------------------------------------------

    // refresh OpenGL when added
    void heavyComponentRefreshed() override;
    // --------------------------------------------------


    // --------------------------------------------------

    std::shared_ptr<IRVideoPlayer> videoPlayer;

    
    void resizeThisComponentEvent(const MouseEvent& e) override;
    void resizeThisComponent(Rectangle<int> rect);
    
    
private:
    // --------------------------------------------------
  
    // --------------------------------------------------
    // ResizingSquare

    void resizingSquareClickedAction(IRResizeSquare2::MovableDirection direction) override;
    void resizingSquareReleasedAction(IRResizeSquare2::MovableDirection direction) override;
    void resizingSquareDraggedAction(MouseEvent e) override;
    
    // --------------------------------------------------
    // call back function by IRVideoPlayer
    // use videoLoadCompletedCallback() to overload
    void videoLoadCompletedAction();
    void videoPlayingUpdateAction(double pos);
    
    // --------------------------------------------------
    void refreshZIndex();
    
    // --------------------------------------------------
    bool enableControllerFlag = true;
    // --------------------------------------------------

    void statusChangedCallback(IRNodeComponentStatus status) override;
    // --------------------------------------------------

    Rectangle<int> boundsInWorkspace;
    // --------------------------------------------------

    // called when this object is brought to the most Front of all other objects
    void moveToFrontAction() override;
    
    // --------------------------------------------------
    // call videoLoadCompletedCallbackFunc if true
    bool isCallback = false;
    
    bool resizing = false;
    // IRVideoPlayer *videoPlayer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoPlayerObject2)
};

#endif /* IRVideoPlayerObject2_hpp */
