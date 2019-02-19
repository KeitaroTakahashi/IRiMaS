//
//  NSNodeComponent.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 06/02/2019.
//

#ifndef NSNodeComponent_hpp
#define NSNodeComponent_hpp

#include "IRNodeComponent.hpp"

class NSNodeComponent : public NSViewComponent,
public ChangeBroadcaster
{
public:
    NSNodeComponent(Component *parent, String name);
    ~NSNodeComponent();

    // ===========================================================================
    // basics
    void resized() override;
    void setSize(float width, float height);
    
    void setEnableParameters(IRNodeComponentSetUp id...) {}
    void setDisableParameters(IRNodeComponentSetUp id...) {}
    
    int getPreviousWidth() const { return this->previousWidth; }
    int getPreviousHeight() const { return this->previousHeight; }
    
    void setPreferenceWindow(PreferenceWindow* preferenceWindow) { this->preferenceWindow = preferenceWindow; }
    PreferenceWindow* getPreferenceWindow() { return this->preferenceWindow; }
    
    // ===========================================================================
    // paint
    virtual void paint(Graphics& g) override;

    // ===========================================================================
    // managing child components binded on the NodeObject
    // this method operates following
    // # addMouseListner -> status changed by Edit mode of workspace
    // # setInterceptsMouseClicks -> status changed by Edit mode of workspace
    void childComponentManager(Component* comp);
    
    // ===========================================================================
    // Audio Source Management
    // Use addAudioComponent() to add any AudioSource made in the NodeObject
    // This method adds a given AudioSource to a mixer which connects to the global mixer to the audio output.
    void addAudioComponent(AudioSource *source);
    // Workspace uses this method to get AudioSources added in this NodeObject
    MixerAudioSource* getAudioSource() const { return this->mixer; }
    // check if any AudioSources are added in this NodeObject.
    bool isContainAudioSource() const { return this->containAudioSourceFlag; }
    
    // ===========================================================================
    // interaction
    // # these JUCE oriented methods are not intended to be overriden in the IRNodeObject
    // # override mouseXXXEvent() method insted to add any interactive tasks.
    // ===========================================================================
    void mouseDown(const MouseEvent& e) override; // JUCE oriented
    // ---------------------------------------------------------------------------
    void mouseMove(const MouseEvent& e) override; // JUCE oriented
    // ---------------------------------------------------------------------------
    void mouseUp(const MouseEvent& e)override; // JUCE oriented
    // ---------------------------------------------------------------------------
    void mouseDoubleClick(const MouseEvent& e) override; // JUCE oriented
    // ---------------------------------------------------------------------------
    void mouseDrag(const MouseEvent& e) override; // JUCE oriented
    // ---------------------------------------------------------------------------
    // # controlling Node Object
    // ---------------------------------------------------------------------------
    void mouseDownNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
    // ---------------------------------------------------------------------------
    void mouseMoveNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
    // ---------------------------------------------------------------------------
    void mouseDoubleClickNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
    // ---------------------------------------------------------------------------
    void mouseUpNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
    // ---------------------------------------------------------------------------
    void mouseDragNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
    // ---------------------------------------------------------------------------
    void pasteThisComponentEvent(const MouseEvent& e); // process of paste this Component
    // ---------------------------------------------------------------------------
    void deleteThisComponentEvent(const MouseEvent& e);
    // ---------------------------------------------------------------------------
    void copyThisComponentEvent(const MouseEvent& e); // process of copying this Component

    // ---------------------------------------------------------------------------
    // resizing method
    // this is virtual method so that you can adjust its behavior to your NodeObject
    virtual void resizeThisComponentEvent(const MouseEvent& e);
    Point<float> getResizingArea() const { return this->resizingArea; }
    void setResizingArea(Point<float> area) { this->resizingArea = area; }
    void recoverEventStatus();
    
    // min max setter
    void setMinimumWidth(const float newMin)     { this->minWidth = newMin; }
    void setMinimumHeight(const float newMin)    { this->minHeight = newMin; }
    void setMaximumWidth(const float newMax)     { this->maxWidth = newMax; }
    void setMaximumHeight(const float newMax)    { this->maxHeight = newMax; }
    
    // ===========================================================================
    // Events for override
    // # add any codes here
    // ===========================================================================
    // ---------------------------------------------------------------------------
    virtual void mouseDownEvent(const MouseEvent& e) {}
    // ---------------------------------------------------------------------------
    virtual void mouseMoveEvent(const MouseEvent& e) {}
    // ---------------------------------------------------------------------------
    virtual void mouseDoubleClickEvent(const MouseEvent& e) {}
    // ---------------------------------------------------------------------------
    virtual void mouseUpEvent(const MouseEvent& e) {}
    // ---------------------------------------------------------------------------
    virtual void mouseDragEvent(const MouseEvent& e) {}
    // ---------------------------------------------------------------------------
    void moveThisComponentEvent(const MouseEvent& e); // process of moving this Component
    // ---------------------------------------------------------------------------
    
    // ===========================================================================
    // CALL BACK FUNCTIONS
    // to inform any changes to the registered object
    // advanced call back functions should be written in the IRNodeObject class not here.
    // ===========================================================================
    // change status call back for its childs class
    virtual void statusChangedWrapper(IRNodeComponentStatus status);
    // this method is used as a override function
    virtual void statusChangedCallback(IRNodeComponentStatus status) {}
    
    // ===========================================================================
    // change status
    // return a flag shows whether this Component is movable or not. Default is true.
    bool isMovable() const { return this->isMovableFlag;}
    bool isVerticalMovable() const { return this->isVerticalMovableFlag; }
    bool isHorizontalMovable() const { return this->isHorizontalMovableFlag; }
    // all setter
    void setMovable(bool movable, bool verticalMovable, bool horizontalMovable)
    {
        this->isMovableFlag = movable;
        this->isVerticalMovableFlag = verticalMovable;
        this->isHorizontalMovableFlag = horizontalMovable;
    }
    // ---------------------------------------------------------------------------
    bool isMoving() const { return this->movingFlag; }
    // ---------------------------------------------------------------------------
    bool isDragging() const { return this->draggingFlag; }
    // ---------------------------------------------------------------------------
    // return a flag shows whether this Component is resizable or not. Default is true.
    bool isResizable() const { return this->isResizableFlag; }
    // ---------------------------------------------------------------------------
    // return a flag shows whether this Component is being resized or not. Default is false.
    bool isResizing() const { return this->resizingFlag; }
    // ---------------------------------------------------------------------------
    // return a flag shows whether this Component is being selected or not. Default is false.
    bool isSelected() const { return this->selectedFlag; }
    void setSelected(bool flag);
    // ---------------------------------------------------------------------------
    // return a flag shows whether the belonging workspace is on edit mode or not.
    bool isEditMode() const { return this->editModeFlag; }
    // if edit mode is true, this object does not receive any Mouse/Keyboard events
    // if false, this object receive Mouse/Keyboard events
    void setEditMode(bool flag) {
        this->editModeFlag = flag;
        statusChangedWrapper(IRNodeComponentStatus::EditModeStatus);
        repaint();
    }
    // ---------------------------------------------------------------------------
    bool isCopied() const { return this->copiedFlag; }
    bool isCopiable() const { return this->isCopiableFlag; }
    // ---------------------------------------------------------------------------
    bool isMouseListenerFlag() const { return this->mouseListenerFlag; }
    void setMouseListenerFlag(bool flag) { this->mouseListenerFlag = flag; }
    
    // ===========================================================================
    //unique ID
    void setUniqueID(String id) { this->uniqueID = id; }
    void setUniqueID(std::string id) { this->uniqueID = String(id); }
    
    String getUniqueID() const  { return this->uniqueID; }

  
    
    
    // ===========================================================================
    // object menu appreas by ctl click
    PopupMenu menu;
    // system defined popup menu events
    void defaultPopupMenuEvents();
    // user defined popup menu events
    virtual void popupMenuEvents() {};
    
    // ===========================================================================
    // object name
    String name;
    
    // ===========================================================================
    // system appearance
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    
    
    // ===========================================================================
    Component* parent;
private:
    // ===========================================================================
    
    // unique id to identify this object
    String uniqueID;
    
    // save data
    t_json saveData;
    
    // mixer for audio
    MixerAudioSource *mixer;
    // if this object contains any AudioSource
    bool containAudioSourceFlag = false;
    
    // ===========================================================================
    // ---------------------------------------------------------------------------
    // Object appearance setup
    float minWidth = 50;
    float minHeight = 25;
    
    float maxWidth = 3000;
    float maxHeight = 3000;
    
    float previousWidth = 0;
    float previousHeight = 0;
    
    
    
    // ===========================================================================
    // ---------------------------------------------------------------------------
    // Interaction
    ComponentBoundsConstrainer constrainer;
    ComponentDragger dragger;
    
    // ===========================================================================
    bool draggingFlag = false;
    // ---------------------------------------------------------------------------
    bool isResizableFlag = true;
    bool resizingFlag = false;
    bool isWidthResizableFlag = true;
    bool isHeightResizableFlag = true;
    // ---------------------------------------------------------------------------
    bool isMovableFlag = true;
    bool isVerticalMovableFlag = true;
    bool isHorizontalMovableFlag = true;
    bool movingFlag = false;
    // ---------------------------------------------------------------------------
    //copy related flag
    bool isCopiableFlag = true;
    bool copiedFlag = false;
    // ---------------------------------------------------------------------------
    bool isDeletableFlag = true;
    // ---------------------------------------------------------------------------
    bool isSelectable = true;
    bool selectedFlag = false;
    // ---------------------------------------------------------------------------
    
    bool mouseListenerFlag = false;
    
    // ===========================================================================
    Point<float> resizingArea;
    
    // ===========================================================================
    // global mode
    bool editModeFlag = true;
    
    PreferenceWindow* preferenceWindow;
     

    
    // ===========================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NSNodeComponent)
    
};
#endif /* NSNodeComponent_hpp */
