//
//  NodeComponent.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 20/08/2018.
//

#ifndef NodeComponent_hpp
#define NodeComponent_hpp

#include "JuceHeader.h"
// #include "juce_audio_utils/juce_audio_utils.h"
#include "json11.hpp"
#include "IRFoundation.h"
#include "IRSaveLoadSystem.hpp"



enum IRNodeComponentSetUp
{
    // resizing
    IREnableResizableFlag,
    IREnableWidthResizingFlag,
    IREnableHeightResizingFlag,
    // selecting
    IREnableSelectableFlag,
    
    //drag
    IREnableMovableFlag,
    
    //copy
    IREnableCopiableFlag,
    
    //delete
    IREnableDeletableFlag,
    
    IRNodeComponentSetUpUndefined
};


enum IRNodeComponentStatus
{
    EditModeStatus, // global status for workspace

    SelectableStatus,
    CopiableStatus,
    DeletableStatus,
    MovableStatus,

    ResizableStatus,
    HeightResizableStatus,
    WidthResizableStatus,
    WillResizeStatus,
    HasResizedStatus,
    
    IRNodeComponentStatusUndefined
};


// heavy light weight component
// lightWeightComponent is set as a default
enum IRNodeComponentType
{
    // juce oriented component
    lightWeightComponent,
    // os oriented object such as VideoComponent, embbed component, OpenGL etc.
    heavyWeightComponent
};


struct NodeObjectType
{
    IRNodeComponentType componentType = IRNodeComponentType::lightWeightComponent;
    NodeObjectType(){}
    
    NodeObjectType(IRNodeComponentType type)
    {
        this->componentType = type;
    }
};



// ===========================================================================
// IRNodeComponent
// fundamental system of the IRNodeObject
// ===========================================================================

class IRNodeComponent : public Component,
                        public ChangeBroadcaster
{
public:
    IRNodeComponent(Component *parent, String name, NodeObjectType objectType = NodeObjectType());
    ~IRNodeComponent();
    
    // basics
    void resized() override;
    void setSize(float width, float height);
    
    void setEnableParameters(IRNodeComponentSetUp id...);
    void setDisableParameters(IRNodeComponentSetUp id...);
    
    int getPreviousWidth() const;
    int getPreviousHeight() const;
    
    void setPreferenceWindow(PreferenceWindow* preferenceWindow);
    PreferenceWindow* getPreferenceWindow();
    
    // paint
    virtual void paint(Graphics& g) override;
    
    
    // managing child components binded on the NodeObject
    // this method operates following
    // # addMouseListner -> status changed by Edit mode of workspace
    // # setInterceptsMouseClicks -> status changed by Edit mode of workspace
    void childComponentManager(Component* comp);
    
    
    // Audio Source Management
    // Use addAudioComponent() to add any AudioSource made in the NodeObject
    // This method adds a given AudioSource to a mixer which connects to the global mixer to the audio output.
    void addAudioComponent(AudioSource *source);
    void removeAudioComponent(AudioSource *source);
    // Workspace uses this method to get AudioSources added in this NodeObject
    MixerAudioSource* getAudioSource() const;
    // check if any AudioSources are added in this NodeObject.
    bool isContainAudioSource() const;
    

    // interaction
    // # these JUCE oriented methods are not intended to be overriden in the IRNodeObject
    // # override mouseXXXEvent() method insted to add any interactive tasks.
    void mouseDown(const MouseEvent& e) override; // JUCE oriented
    void mouseMove(const MouseEvent& e) override; // JUCE oriented
    void mouseUp(const MouseEvent& e)override; // JUCE oriented
    void mouseDoubleClick(const MouseEvent& e) override; // JUCE oriented
    void mouseDrag(const MouseEvent& e) override; // JUCE oriented
    
    // # controlling Node Object
    void mouseDownNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
    void mouseMoveNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
    void mouseDoubleClickNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
    void mouseUpNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
    void mouseDragNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
    
    void pasteThisComponentEvent(const MouseEvent& e); // process of paste this Component
    void deleteThisComponentEvent(const MouseEvent& e);
    void copyThisComponentEvent(const MouseEvent& e); // process of copying this Component

    // resizing method
    // this is virtual method so that you can adjust its behavior to your NodeObject
    virtual void resizeThisComponentEvent(const MouseEvent& e);
    Point<float> getResizingArea() const;
    void setResizingArea(Point<float> area);
    void recoverEventStatus();
    
    // min max setter
    void setMinimumWidth(const float newMin);
    void setMinimumHeight(const float newMin);
    void setMaximumWidth(const float newMax);
    void setMaximumHeight(const float newMax);
    
    // ===========================================================================
    // Events for override
    // # add any codes here
    // ===========================================================================
    
    virtual void mouseDownEvent(const MouseEvent& e);
    virtual void mouseMoveEvent(const MouseEvent& e);
    virtual void mouseDoubleClickEvent(const MouseEvent& e);
    virtual void mouseUpEvent(const MouseEvent& e);
    virtual void mouseDragEvent(const MouseEvent& e);

    void moveThisComponentEvent(const MouseEvent& e); // process of moving this Component
    
    // ===========================================================================
    // CALL BACK FUNCTIONS
    // to inform any changes to the registered object
    // advanced call back functions should be written in the IRNodeObject class not here.
    // ===========================================================================
    // change status call back for its childs class
    virtual void statusChangedWrapper(IRNodeComponentStatus status);
    // this method is used as a override function
    virtual void statusChangedCallback(IRNodeComponentStatus status);
    // callback informing
    std::function<void(IRNodeComponentStatus)> statusChangeCompleted;

    
    // change status
    // return a flag shows whether this Component is movable or not. Default is true.
    bool isMovable() const;
    bool isVerticalMovable() const;
    bool isHorizontalMovable() const;
    // all setter
    void setMovable(bool movable, bool verticalMovable, bool horizontalMovable);
    bool isMoving() const;
    bool isDragging() const;
    // return a flag shows whether this Component is resizable or not. Default is true.
    bool isResizable() const;
    // return a flag shows whether this Component is being resized or not. Default is false.
    bool isResizing() const;
    // return a flag shows whether this Component is being selected or not. Default is false.
    bool isSelected() const; // FD - THIS HAS NO IMPLEMENTATION
    void setSelected(bool flag);
    // return a flag shows whether the belonging workspace is on edit mode or not.
    bool isEditMode() const;
    // if edit mode is true, this object does not receive any Mouse/Keyboard events
    // if false, this object receive Mouse/Keyboard events
    void setEditMode(bool flag);
    
    bool isCopied() const;
    bool isCopiable() const;
    
    bool isMouseListenerFlag() const;
    void setMouseListenerFlag(bool flag);

    //unique ID
    void setUniqueID(String id);
    void setUniqueID(std::string id);

    String getUniqueID() const;
    
    // set IRFileManager from IRProject
    void setIRFileManager(IRFileManager* manager) { this->fileManager = manager; }
    IRFileManager* getFileManager() { return this->fileManager; }
    
    // object type
    NodeObjectType getObjectType() const;
    
    // Save System
    
    /*
    void param_register(std::string id, unsigned char data);
    void param_register(std::string id, int data);
   
    void param_register(std::string id, unsigned int data);
    void param_register(std::string id, long data);
    void param_register(std::string id, float data);
    void param_register(std::string id, double data);
    void param_register(std::string id, std::string data);

    */
    
    // object menu appreas by ctl click
    PopupMenu menu;
    // system defined popup menu events
    void defaultPopupMenuEvents();
    // user defined popup menu events
    virtual void popupMenuEvents();
    
    // object name 
    String name;

    // system appearance
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    // parent
    Component* parent;
    
    
private:
    
    // unique id to identify this object
    String uniqueID;
    
    // object type
    NodeObjectType objectType;
    
    // save data
    t_json saveData;
    
    // mixer for audio
    MixerAudioSource *mixer;
    // if this object contains any AudioSource
    bool containAudioSourceFlag = false;
    
    
    // Object appearance setup
    float minWidth = 50;
    float minHeight = 25;
    
    float maxWidth = 3000;
    float maxHeight = 3000;
    
    float previousWidth = 0;
    float previousHeight = 0;

    
    // Interaction
    ComponentBoundsConstrainer constrainer;
    ComponentDragger dragger;
    
    
    bool draggingFlag = false;
    
    bool isResizableFlag = true;
    bool resizingFlag = false;
    bool isWidthResizableFlag = true;
    bool isHeightResizableFlag = true;
    
    bool isMovableFlag = true;
    bool isVerticalMovableFlag = true;
    bool isHorizontalMovableFlag = true;
    bool movingFlag = false;
    
    //copy related flag
    bool isCopiableFlag = true;
    bool copiedFlag = false;
    
    bool isDeletableFlag = true;
    
    bool isSelectable = true;
    bool selectedFlag = false;
    
    bool mouseListenerFlag = false;

    Point<float> resizingArea;
    
    // global mode
    bool editModeFlag = true;
    
    PreferenceWindow* preferenceWindow;

    // IRFileManager is exclusive for each Project
    IRFileManager* fileManager = nullptr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRNodeComponent)
};



#endif /* NodeComponent_hpp */




