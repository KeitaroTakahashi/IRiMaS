
#ifndef NodeObject_hpp
#define NodeObject_hpp

#include "JuceHeader.h"

#include "IRNodeComponent.hpp"
//#include "IRUIFoundation.hpp"
#include "IRFileManager.hpp"

class IRNodeObject : public IRNodeComponent,
public IRLinkFoundation
{
    
public:
    
    IRNodeObject(Component* parent, String name, NodeObjectType objectType = NodeObjectType());
    ~IRNodeObject();
    
    
    // something should be painted on the workspace.
    // @param1 Graphics from the workspace
    // @param2 frame rect of the workspace
    virtual void paintOnWorkspace(Graphics& g, Component* workspace) {};
    // paint oparated by parent
    void initialPaintOnWorkspace(Graphics& g, Component* workspace);
    
    virtual IRNodeObject* copyThis(); //copy constructor
    virtual IRNodeObject* copyContents(IRNodeObject* object); // copy constructor with contents
    virtual IRNodeObject* copyDragDropContents(IRNodeObject* object); // copy draged and dropped contents
    
    
    virtual t_json saveThisToSaveData();
    virtual void loadThisFromSaveData(t_json saveData);
    
    // mouse events for its child class
    virtual void mouseDownEvent(const MouseEvent& e) override;
    virtual void mouseUpEvent(const MouseEvent& e) override;
    virtual void mouseMoveEvent(const MouseEvent& e) override;
    virtual void mouseDoubleClickEvent(const MouseEvent& e) override;
    virtual void mouseDragEvent(const MouseEvent& e) override;
    
    void mouseUpCompleted(const MouseEvent& e) override;
    
    
    // selected event from IRNodeComponent
    void selectedChangeEvent() override;
    void editModeChangedEvent() override;
    void linkModeChangedEvent() override;
    
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

        // notification to IRWorkspace
        virtual void nodeObjectModifiedNotification(IRNodeObject* obj) {};
        
        //inform its parent that edit mode status changed
        virtual void editModeChangedInNodeObject(bool editMode) {};
        
        // link
        virtual void linkModeChangedInNodeObject(bool linkMode) {};
        
        virtual void getSelectedLinkSystemFlag(IRNodeObject* obj) {};
        // overriden by IRUIFoundation
        virtual void receiveAudioLink(IRNodeObject* obj) {};
        virtual void receiveTextLink(IRNodeObject* obj) {};
        virtual void receiveImageLink(IRNodeObject* obj) {};
        virtual void receiveDataLink(IRNodeObject* obj) {};
        virtual void receiveVideoLink(IRNodeObject* obj) {};

        //
        // give its IRFileManager when it is given or modified.
        // this is used for IRUIFoundation to receive IRFileManager
        virtual void updateIRFileManager(IRFileManager* fileManager) {};
        
        // add object to a global space in Workspace
        virtual void addObjectGlobal(IRObjectPtr obj, String id) {};
        virtual void getObjectGlobal(IRNodeObject *obj) {};
    };
    
    virtual void addListener(Listener* newListener) { this->listeners.add(newListener); }
    virtual void removeListener(Listener* listener) { this->listeners.remove(listener);}
    ListenerList<Listener>& getListenerList() { return this->listeners; }
    
    // ---------------------------------------------------------------------------
    
    std::function<void()> dragoutNodeObjectCompleted;
    std::function<void()> dropoutNodeObjectCompleted;
    std::function<void()> editModeChangedCompleted;
    std::function<void()> linkModeChangedCompleted;

    std::function<void()> addOBjectGlobalCompleted;
    
    // requests to IRProject
    std::function<void()> saveProjectCompleted;
    std::function<void()> saveAsProjectCompleted;
    std::function<void()> closeProjectCompleted;
    std::function<void()> openProjectCompleted;
    std::function<void()> createNewProjectCompleted;

    std::function<void()> getObjectGlobalCompleted;
    

    // fire dragoutNodeObjectFromParent() methods in Listener
    void callDragOutNodeObjectFromParent();
    // fire dropoutNodeObjectFromParent() methods in Listener
    void callDropOutNodeObjectFromParent();
    // fire editModeChangedInNodeObject() methods in Listener
    void callEditModeChangedInNodeObject();
    
    void callLinkModeChangedInNodeObject();
    void receiveSelectedLinkMenuItemEvent() override;// from IRLinkFoundation
    void setLinkModeEvent() override {};
    void setLinkActivationEvent() override;
    
    void setLinkedEvent() override {};
    void callGetSelectedLinkSystemFlag();
    
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
    
    void callOpenFileInspecter();
    void callOpenPreferenceWindow();

    
    void notifyNodeObjectModification();
    
    
    void callReceiveAudioLink(IRAudio *obj);
    void callReceiveTextLink(IRText *obj);
    void callReceiveImageLink(IRImage* obj);
    void callReceiveDataLink(IRData *obj);
    void callReceiveVideoLink(IRVideo *obj);
    
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
    // Linking System
    
    
    
    

    
    // ============================================================

    
/*
    virtual bool keyPressed (const KeyPress& key,
                     Component* originatingComponent) override
    {
        return true;
    }*/
    
    // ============================================================

protected:
    
    Component* parent;

private:
        
    
    ListenerList<Listener> listeners;
    

    
    //link System
    
    IRObjectPtr p_obj;
    String p_id;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRNodeObject)
};


#endif /* NodeObject_hpp */




