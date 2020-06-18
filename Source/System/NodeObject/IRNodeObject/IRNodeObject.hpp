
#ifndef NodeObject_hpp
#define NodeObject_hpp

#include "JuceHeader.h"

#include "IRNodeComponent.hpp"
#include "IRFileManager.hpp"
#include "IRNodeObjectType.h"
#include "IRPreferenceObject.hpp"
#include "IRObjectController.hpp"
#include "IREnclosedObject.hpp"

#include "ObjectArranger.hpp"

class IRNodeObject : public IRNodeComponent,
public ChangeListener
{
    
public:
    // ==================================================

    enum IRNodeObjectStatus
    {
        ORDINARY,
        ENCLOSE
    };
    // ==================================================

    IRNodeObject(Component* parent,
                 String name,
                 IRStr* str,
                 NodeObjectType objectType = NodeObjectType());
    ~IRNodeObject();
    // ==================================================

    // something should be painted on the workspace.
    // @param1 Graphics from the workspace
    // @param2 frame rect of the workspace
    virtual void paintOnWorkspace(Graphics& g, Component* workspace) {};
    // paint oparated by parent
    void initialPaintOnWorkspace(Graphics& g, Component* workspace);
    // ==================================================

    virtual IRNodeObject* copyThis(); //copy constructor
    virtual IRNodeObject* copyContents(IRNodeObject* object); // copy constructor with contents
    virtual IRNodeObject* copyDragDropContents(IRNodeObject* object); // copy draged and dropped contents
    
    // ==================================================

    virtual t_json saveThisToSaveData();
    virtual void loadThisFromSaveData(t_json saveData);
    // ==================================================
    // Controller
    // to controll position of this project etc.
    void setArrangeController(ArrangeController* controller);
    virtual void arrangeControllerChangedNotify() {};
    
    t_json getArrangeControllerSaveData();
    // ==================================================

    virtual void IRChangeListenerCallback(ChangeBroadcaster* source);
    // ==================================================


    // mouse events for its child class
    virtual void mouseDownEvent(const MouseEvent& e) override;
    virtual void mouseUpEvent(const MouseEvent& e) override;
    virtual void mouseMoveEvent(const MouseEvent& e) override;
    virtual void mouseDoubleClickEvent(const MouseEvent& e) override;
    virtual void mouseDragEvent(const MouseEvent& e) override;
    
    void mouseUpCompleted(const MouseEvent& e) override;
    // ==================================================

    
    // selected event from IRNodeComponent
private:
    void selectedChangeEvent() override;
public:
    virtual void selectedChangedAction(bool flag) {}

    void editModeChangedEvent() override;
    //void linkModeChangedEvent() override;
    
    void thisObjectGetFocused() override;
    
    // ==================================================
    void setObjController(IRObjectController* objCtl);
    IRObjectController* getObjController() const { return this->objController; }
    
    // ==================================================
    // move to Front action
    // only modify z order
    void moveThisToFrontZIndex();
    // also update
    void moveToFrontEvent(bool isRefreshHeavyComponent, bool registerZindex) override;
    // only modify z order
    void moveThisToBackIndex();
    // also update
    void moveToBackEvent() override;
    
    // ## ========== Obligatory to implement! ========== ##
    virtual void moveToFrontAction() {}; // for its Child component
    virtual void moveToBackAction() {};
    // ==================================================
    
    // called when this object position is changed
    virtual void ObjectPositionChanged(int x, int y) override {};
    virtual void ObjectBoundsChanged(Rectangle<int> bounds) override {};
    // ==================================================
    
    // ###### Time Code Anomation ######
    
    //void StatusUpdated() override;
    
    // ==================================================

    // STATUS
    
    IRNodeObjectStatus getStatus() const { return this->status; }
    void setStatus(IRNodeObjectStatus newStatus);
    
    void transformStatusToOrdinary();
    void transformStatusEnclose();
    void setEncloseMode(bool flag);
    IRNodeObjectStatus getEncloseMode() const { return this->getStatus(); }
    void createEnclosedObject();
    void setEncloseColour(Colour colour);
    // ==================================================

    // ==================================================

    class Listener
    {
    public:
        virtual ~Listener() {}
        
        //write methods called by IRNodeComponent objects.
        virtual void dragoutNodeObjectFromParent(IRNodeObject* obj) {};
        virtual void dropoutNodeObjectFromParent(IRNodeObject* obj) {};
        
        // save action from IRNodeObject
        virtual void saveProject(){};
        
        virtual void saveAsProject() {};
        
        // close Project
        virtual void closeProject() {};
        
        virtual void openProject() {};
        
        virtual void createNewProject() {};
        
        virtual void openFileInspecter() {};
        virtual void openPreferenceWindow() {};
        
        //object selection
        virtual void nodeObjectSelectionChange(IRNodeObject* obj) {};
        virtual void nodeObjectGetFocused(IRNodeObject* obj) {}
        
        //Front
        virtual void nodeObjectMoveToFront(IRNodeObject* obj) {}
        //Back
        virtual void nodeObjectMoveToBack(IRNodeObject* obj) {}

        // notification to IRWorkspace
        virtual void nodeObjectModifiedNotification(IRNodeObject* obj) {};
        
        //inform its parent that edit mode status changed
        virtual void editModeChangedInNodeObject(bool editMode) {};
        
       //inform its position change
        virtual void nodeObjectPositionChanged(IRNodeObject* obj) {};
        
        
        // give its IRFileManager when it is given or modified.
        // this is used for IRUIFoundation to receive IRFileManager
        virtual void updateIRFileManager(IRFileManager* fileManager) {};
        
        // add object to a global space in Workspace
        virtual void addObjectGlobal(IRObjectPtr obj, String id) {};
        virtual void getObjectGlobal(IRNodeObject *obj) {};
        
        
        //heavy
        // used for example by IRVideoPlayer when it completes loading a movie file and avoid overlapping with
        // the menu bars because VideoComponent automatically comes foreground when it loads any movie files.
        // It emit the recreation of Bars and thus bring them foreground to IRVideoPlayer and other possible IRNodeObjects.
        // NOTE : this function does not change objectZOrder and if you need then use addHeavyComponentToTopZOrder
        virtual void heavyComponentCreated(IRNodeObject* obj) {};
        // Invoke an action to move IRNodeObject to the top of objectZOrder on the workspace.
        virtual void addHeavyCopmonentToTopZOrder(IRNodeObject* obj) {};
        // simply reorganize z-order of objects
        virtual void reorderZIndex() {};

    };
    
    virtual void addListener(Listener* newListener) { this->listeners.add(newListener); }
    virtual void removeListener(Listener* listener) { this->listeners.remove(listener);}
    ListenerList<Listener>& getListenerList() { return this->listeners; }
    
    // ---------------------------------------------------------------------------
    
    std::function<void()> dragoutNodeObjectCompleted;
    std::function<void()> dropoutNodeObjectCompleted;
    std::function<void()> editModeChangedCompleted;
    std::function<void()> linkModeChangedCompleted;

    //std::function<void()> addOBjectGlobalCompleted;
    
    // requests to IRProject
    std::function<void()> saveProjectCompleted;
    std::function<void()> saveAsProjectCompleted;
    std::function<void()> closeProjectCompleted;
    std::function<void()> openProjectCompleted;
    std::function<void()> createNewProjectCompleted;

    //std::function<void()> getObjectGlobalCompleted;
    

    // fire dragoutNodeObjectFromParent() methods in Listener
    void callDragOutNodeObjectFromParent();
    // fire dropoutNodeObjectFromParent() methods in Listener
    void callDropOutNodeObjectFromParent();
    // fire editModeChangedInNodeObject() methods in Listener
    void callEditModeChangedInNodeObject();
    // fire heavyComponentCreated
    void callHeavyComponentCreated(IRNodeObject* obj);
    
    // ask IRWorkspace to Recorder objects according to the new ZIndex
    void callReorderZIndex();

    
    // from IRNodeComponent to call callHeavyComponentCreated
    void heavyComponentCreatedFunc() override
    {
        getStr()->projectOwner->rebindOpenGLContents();
        //callHeavyComponentCreated(this);
    }
    
    void callNodeObjectPositionChanged();
    
    void callAddHeavyComponentToTopZOrder(IRNodeObject* obj);

    
    void callLinkModeChangedInNodeObject();
    /*
    void receiveSelectedLinkMenuItemEvent() override;// from IRLinkFoundation
    void setLinkModeEvent() override {};
    void setLinkActivationEvent() override;
    
    void setLinkedEvent() override {};*/
    //void callGetSelectedLinkSystemFlag();
    
    // fire addObjectGlobal() method in Listener
    void callAddObjectGlobal(IRObjectPtr obj, String id);
    // fire getObjectGlobal() method in Listener
    IRObjectPtr callGetObjectGlobal(String id);
    // fire saveProject() method in Listener
    void callSaveProject();
    void callSaveAsProject();
    // fire CloseProject() method in Listener
    void callCloseProject();
    void callCreateNewProject();
    void callOpenProject();
    
    void callNodeObjectSelectionChange();
    void callNodeObjectGetFocused();
    
    void callNodeObjectMoveToFront();
    void callNodeObjectMoveToBack();
    
    void callOpenFileInspecter();
    void callOpenPreferenceWindow();

    
    void notifyNodeObjectModification();
    
    /*
    void callReceiveAudioLink(IRAudio *obj);
    void callReceiveTextLink(IRText *obj);
    void callReceiveImageLink(IRImage* obj);
    void callReceiveDataLink(IRData *obj);
    void callReceiveVideoLink(IRVideo *obj);
     */
    
    void callUpdateIRFileManager(IRFileManager* fileManager);

    // ===========================================================================
        // Global Object
    
    String getGlobalObjectID() const { return this->p_id; }
    void setGlobalObject(IRObjectPtr obj) { this->p_obj = obj; }
    IRObjectPtr getGlobalObject() { return this->p_obj; }
    // ===========================================================================

    // methods for save and load functions. Developers need to define the behavior of objects when save or load project files.
    // otherwise, save data does not contain any information about the object setting but only the objectType and its bounds.
    // The save method must follow the syntax of Json using json11 library.
    
    virtual void saveObjectContents();
    
    virtual void loadObjectContents();
    
    // ============================================================
    
    
    
    

    
    // ============================================================

    
/*
    virtual bool keyPressed (const KeyPress& key,
                     Component* originatingComponent) override
    {
        return true;
    }*/
    
    // ============================================================

    Component* getParent() { return this->parent; }
protected:
    
    Component* parent;
    
    void ObjectPositionChanged4IRNodeObject(int x, int y) override;
    void ObjectBoundsChanged4IRNodeObject(Rectangle<int> bounds) override;

private:
    // ==================================================
    // STATUS //

    IRNodeObjectStatus status = ORDINARY;
    std::shared_ptr<IREnclosedObject> enclosedObject;
    void enclosedObjectClickedAction();
    
    Rectangle<int> ordinaryBounds;
    Rectangle<int> encloseBounds;
public:
    void setOrdinaryBounds(Rectangle<int> bounds);
    void setEncloseBounds(Rectangle<int> bounds);
    
    Rectangle<int> getOrdinaryBounds() const { return this->ordinaryBounds; }
    Rectangle<int> getEncloseBounds() const { return this->encloseBounds; }
private:
    
    // flag to inform if the encloseObject is already created and has encloseBounds.
    bool isEncloseObjectAlreadyDefined = false;
    // called when the position of this object is changed
    void encloseObjectPositionChangedAction(int x, int y);
    void encloseObjectBoundsChangedAction(Rectangle<int> bounds);

    // ==================================================

    // ObjectController
    IRObjectController* objController = nullptr;
    // Object Arrange Controller//
    ArrangeController* arrangeController = nullptr;
    // called when the position of this object is changed
    void arrangeControllerPositionChangedAction(int x, int y);
    void arrangeControllerBoundsChangedAction(Rectangle<int> bounds);
    void arrangeControllerChangedCallback(ChangeBroadcaster* source);
public:
    void loadArrangeControllerSaveData(t_json arrangeCtl);
private:
    // ==================================================
    // EVENT COMPONENT //
    // for videoAnnotater or Any other objects
    Component* eventComponent = nullptr;
public:
    void setEventComponent(Component* event) { this->eventComponent = event; this->hasEventComponentFlag = true; }
    void removeEventComponent() { this->eventComponent = nullptr; this->hasEventComponentFlag = false;}
    bool hasEventComponent() const { return this->hasEventComponentFlag; }
    Component* getEventComponent() { return this->eventComponent; }
    
private:
    bool hasEventComponentFlag = false;
    
    // in sec. Annoation time code
    float StartTimeSec = 0.0;
    float EndTimeSec = 0.0;
    // ==================================================
public:
    void setStartTimeSec(float sec) { this->StartTimeSec = sec; getStatusStr()->startTimeCode = sec; }
    void setEndTimeSec(float sec) { this->EndTimeSec = sec; getStatusStr()->endTimeCode = sec; }
    float getStartTimeSec() const { return this->StartTimeSec; }
    float getEndTimeSec() const { return this->EndTimeSec; }
    
private:

    // ==================================================
    // LISTENER //
    ListenerList<Listener> listeners;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    IRObjectPtr p_obj;
    String p_id;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRNodeObject)
};


#endif /* NodeObject_hpp */




