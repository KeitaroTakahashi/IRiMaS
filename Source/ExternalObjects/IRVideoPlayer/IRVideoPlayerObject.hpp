//
//  IRVideoPlayerObject.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/01/2019.
//

#ifndef IRVideoPlayerObject_hpp
#define IRVideoPlayerObject_hpp

#include "IRNodeObject.hpp"
#include "IRVideoPlayer.hpp"

#include "IRVideoAnnotationComponentHeader.h"

class IRVideoPlayerObject : public IRNodeObject
{
    
public:
    
    IRVideoPlayerObject(Component* parent, IRStr* str, bool withOpenButton = true);
    ~IRVideoPlayerObject();
    
    
    // --------------------------------------------------
    // copy related methods
    
    virtual IRNodeObject* copyThis() override;
    
    virtual IRNodeObject* copyContents(IRNodeObject* object) override;
    
    virtual IRNodeObject* copyDragDropContents(IRNodeObject* object) override;
    
    // share object contents owned by this obejct to twithObject
    void shareContentsWith(IRVideoPlayerObject* withObject);
    
    // --------------------------------------------------
    
    virtual t_json saveThisToSaveData() override;
    
    // --------------------------------------------------
    
    virtual void loadThisFromSaveData(t_json data) override;
    
    // --------------------------------------------------
    // resize reated methods
    virtual void resized() override;
    
    void resizeThisComponentEvent(const MouseEvent& e) override;
    
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
    
    // --------------------------------------------------

    IRVideoPlayer* getVideoPlayer() { return this->videoPlayer.get(); }

    // --------------------------------------------------
    
    // --------------------------------------------------

    // refresh OpenGL when added
    void heavyComponentRefreshed() override;
    // --------------------------------------------------


    // --------------------------------------------------

    std::shared_ptr<IRVideoPlayer> videoPlayer;

private:
    // --------------------------------------------------
    // call back function by IRVideoPlayer
    // use videoLoadCompletedCallback() to overload
    void videoLoadCompletedAction();
    void videoPlayingUpdateAction(double pos);
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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoPlayerObject)
};

#endif /* IRVideoPlayerObject_hpp */
