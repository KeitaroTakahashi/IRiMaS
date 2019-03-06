
#ifndef NodeObject_hpp
#define NodeObject_hpp

#include "JuceHeader.h"

#include "IRNodeComponent.hpp"
#include "IRFileManager.hpp"





class IRNodeObject : public IRNodeComponent
{
    
public:
    
    IRNodeObject(Component* parent, String name, NodeObjectType objectType = NodeObjectType());
    ~IRNodeObject();
    
    
    virtual IRNodeObject* copyThis(); //copy constructor
    virtual IRNodeObject* copyThisWithContents(); // copy constructor with contents
    virtual IRNodeObject* copySelectedContents(); // copy constructor with irregular contents
    
    virtual t_json saveThisToSaveData();
    virtual void loadThisFromSaveData(t_json saveData);
    
    // mouse events for its child class
    virtual void mouseDownEvent(const MouseEvent& e) override;
    virtual void mouseUpEvent(const MouseEvent& e) override;
    virtual void mouseMoveEvent(const MouseEvent& e) override;
    virtual void mouseDoubleClickEvent(const MouseEvent& e) override;
    virtual void mouseDragEvent(const MouseEvent& e) override;

    class Listener
    {
    public:
        virtual ~Listener() {}
        
        //write methods called by IRNodeComponent objects.
        virtual void dragoutNodeObjectFromParent(IRNodeObject* obj) {};
        virtual void dropoutNodeObjectFromParent(IRNodeObject* obj) {};
        
        // save action from IRNodeObject
        virtual void saveProject(){};
        
        //inform its parent that edit mode status changed
        virtual void editModeChangedInNodeObject(bool editMode) {};
        
        // add object to a global space in Workspace
        virtual void addObjectGlobal(IRObjectPtr obj, String id) {};
        virtual IRObjectPtr getObjectGlobal(String id) { return nullptr;};
    };
    
    virtual void addListener(Listener* newListener) { this->listeners.add(newListener); }
    virtual void removeListener(Listener* listener) { this->listeners.remove(listener);}
    ListenerList<Listener>& getListenerList() { return this->listeners; }
    
    // ---------------------------------------------------------------------------
    
    std::function<void()> dragoutNodeObjectCompleted;
    std::function<void()> dropoutNodeObjectCompleted;
    std::function<void()> editModeChangedCompleted;
    std::function<void()> addOBjectGlobalCompleted;
    std::function<void()> saveProjectCompleted;
    std::function<void()> getObjectGlobalCompleted;
    

    // fire dragoutNodeObjectFromParent() methods in Listener
    void callDragOutNodeObjectFromParent();
    // fire dropoutNodeObjectFromParent() methods in Listener
    void callDropOutNodeObjectFromParent();
    // fire editModeChangedInNodeObject() methods in Listener
    void callEditModeChangedInNodeObject();
    // fire addObjectGlobal() method in Listener
    void callAddObjectGlobal(IRObjectPtr obj, String id);
    // fire getObjectGlobal() method in Listener
    IRObjectPtr callGetObjectGlobal(String id);
    // fire saveProject() method in Listener
    void callSaveProject();
    // ===========================================================================
    
    // methods for save and load functions. Developers need to define the behavior of objects when save or load project files.
    // otherwise, save data does not contain any information about the object setting but only the objectType and its bounds.
    // The save method must follow the syntax of Json using json11 library.
    
    virtual void saveObjectContents();
    
    virtual void loadObjectContents();
    
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
    
    IRObjectPtr p_obj;
    String p_id;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRNodeObject)
};


#endif /* NodeObject_hpp */




