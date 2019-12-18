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

class IRVideoPlayerObject : public IRNodeObject
{
    
public:
    
    IRVideoPlayerObject(Component* parent, IRStr* str, bool withOpenButton = true);
    ~IRVideoPlayerObject();
    
    
    // --------------------------------------------------
    // copy related methods
    
    IRNodeObject* copyThis() override;
    
    IRNodeObject* copyContents(IRNodeObject* object) override;
    
    IRNodeObject* copyDragDropContents(IRNodeObject* object) override;
    
    // share object contents owned by this obejct to twithObject
    void shareContentsWith(IRVideoPlayerObject* withObject);
    
    // --------------------------------------------------
    
    t_json saveThisToSaveData() override;
    
    // --------------------------------------------------
    
    void loadThisFromSaveData(t_json data) override;
    
    // --------------------------------------------------
    // resize reated methods
    void resized() override;
    
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
