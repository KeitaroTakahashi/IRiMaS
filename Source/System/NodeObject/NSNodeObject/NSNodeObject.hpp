//
//  NSNodeObject.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 06/02/2019.
//

#ifndef NSNodeObject_hpp
#define NSNodeObject_hpp

#include "NSNodeComponent.hpp"

class NSNodeObject : public NSNodeComponent,
public KeyListener
{
public:
    NSNodeObject(Component* parent, String name);
    ~NSNodeObject();
    
    //copy constructor
    virtual NSNodeObject* copyThis() { return new NSNodeObject(this->parent, "IRNodeObject"); }
    // copy constructor with contents
    virtual NSNodeObject* copyThisWithContents() { return new NSNodeObject(this->parent, "IRNodeObject"); }
    // copy constructor with irregular contents
    virtual NSNodeObject* copySelectedContents() { return new NSNodeObject(this->parent, "IRNodeObject"); }
    
    virtual t_json saveThisToSaveData() { t_json a; return a; }
    // load data contents from save data
    virtual void loadThisFromSaveData(t_json saveData) {}
    // ============================================================
    // mouse events for its child class
    virtual void mouseDownEvent(const MouseEvent& e) override {}
    virtual void mouseUpEvent(const MouseEvent& e) override {}
    virtual void mouseMoveEvent(const MouseEvent& e) override {}
    virtual void mouseDoubleClickEvent(const MouseEvent& e) override {}
    virtual void mouseDragEvent(const MouseEvent& e) override {}
    
    // ===========================================================================
    // Listener
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        //write methods called by IRNodeComponent objects.
        virtual void dragoutNodeObjectFromParent(NSNodeObject* obj) {};
        virtual void dropoutNodeObjectFromParent(NSNodeObject* obj) {};
        
        // save action from IRNodeObject
        virtual void saveProject(){};
        
        //inform its parent that edit mode status changed
        virtual void editModeChangedInNodeObject(bool editMode) {};
        
        // add object to a global space in Workspace
        virtual void addObjectGlobal(IRObjectPtr obj, String id) {};
        virtual IRObjectPtr getObjectGlobal(String id) { return nullptr; };
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
    
    virtual void saveObjectContents() {}
    
    virtual void loadObjectContents() {}
    
    // ============================================================
    virtual bool keyPressed (const KeyPress& key,
                             Component* originatingComponent) override
    {
        std::cout << "key pressed\n";
        
        return true;
    }
    
    // ============================================================
    
protected:
    Component* parent;
    
private:
    ListenerList<Listener> listeners;
    
    IRObjectPtr p_obj;
    String p_id;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NSNodeObject)
};


#endif /* NSNodeObject_hpp */