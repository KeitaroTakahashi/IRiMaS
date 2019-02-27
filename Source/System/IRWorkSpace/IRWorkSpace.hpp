//
//  IRWorkSpace.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 20/08/2018.
//

#ifndef IRWorkSpace_hpp
#define IRWorkSpace_hpp

#include <map>
#include "JuceHeader.h"
// #include "juce_audio_utils/juce_audio_utils.h"
#include "PreferenceWindow.hpp"

#include "IRNodeObject.hpp"
#include "AudioEngine.h"
#include "IRNodeObjectSelector.hpp"
#include "SelectNodeObjects.hpp"
// #include "CreateObjectMenu.h"
#include "ObjectMenuWindow.hpp"

#include "ExternalObjectHeader.h"
#include "IRSaveLoadSystem.hpp"

#include "KLib.h"


class IRWorkSpace : public AudioAppComponent,
public ChangeBroadcaster,
public ChangeListener,
public IRNodeObject::Listener,
public ObjectListMenu::Listener,
private KeyListener

{
public:
    IRWorkSpace(String title, Rectangle<int> frameRect, PreferenceWindow* preferenceWindow);
    ~IRWorkSpace() {}
    
    //==================================================
    void paint (Graphics&) override;
    void drawShadows(Graphics& g);
    void resized() override;
    String getTitle() { return this->title; }
    // ===========================================================================
    // interaction
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
    void modifierKeysChanged(const ModifierKeys &mod) override;
    //bool keyPressed (const KeyPress& key)override;
    bool keyPressed (const KeyPress& key, Component* originatingComponent) override;
    
    
    // ---------------------------------------------------------------------------
    //==================================================
    //Listener
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    //==================================================
    // AudioAppComponent
    void AudioSetup()
    {
        // setAudio
        setAudioChannels(0, 2);
    }
    void closeAudioSetup()
    {
        shutdownAudio();
    }
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        this->mixer.getAudioSource().prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
    virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override
    {
        this->mixer.getAudioSource().getNextAudioBlock(bufferToFill);
    }
    virtual void releaseResources() override
    {
        this->mixer.getAudioSource().releaseResources();
    }
    
    AudioSource &getMixer() { return this->mixer.getAudioSource(); }
    
    //==================================================    // IRNodeObject Listener
    void dragoutNodeObjectFromParent(IRNodeObject* obj) override;
    void dropoutNodeObjectFromParent(IRNodeObject* obj) override;
    void editModeChangedInNodeObject(bool editMode) override;
    void saveProject() override;
    
    void addObjectGlobal(IRObjectPtr obj, String id) override;
    IRObjectPtr getObjectGlobal(String id) override;
    //==================================================
    // object control
    
    void copySelectedObjects();
    void pasteSelectedObjects();
    void duplicateSelectedObjects();
    void deleteSelectedObjects();
    
    // manage the heavy weight components which are always drown on the top of all other components
    // the heavy weights components will be hidden or shown according to this workspace status
    void manageHeavyWeightComponents(bool flag);
    
    // -------------------------------------------------
    
    // object management
    void createObject(IRNodeObject* obj);
    void createObject(std::string objName);
    void copyObject(IRNodeObject *obj, bool clearCopied);
    void pasteObject(IRNodeObject *obj, bool addToSelected);
    void duplicateObject(IRNodeObject *obj);
    void deleteObject(IRNodeObject *obj);
    
    
    
    // object menu
    
    void openObjectListMenu(Point<int>Pos);
    void closeObjectListMenu();
    
    void itemSelectionAction(ObjectListMenu* menu) override;
    void itemHasSelectedAction(ObjectListMenu* menu) override;
    
    //==================================================
    // save load
    
    json11::Json makeSaveDataOfThis();
    
    
    
    //==================================================    //flag
    bool isEditMode() const { return this->editModeFlag; }
    void setEditMode(bool flag) {
        this->editModeFlag = flag;
        
        if(flag){
            this->title = this->name + " (EDIT MODE)";
            this->setInterceptsMouseClicks(true, true);
            setWantsKeyboardFocus(true);
        }
        else{
            this->title = this->name;
            this->setInterceptsMouseClicks(true, false);
        }
        
        //std::cout << "edit mode changed " << flag << " : " << this->title << std::endl;
        
        for(auto obj : this->objects)
        {
            obj->setEditMode(flag);
        }
        // send change message to IRProject
        sendChangeMessage();
    }
    
    //==================================================     // getter
    
    Array<IRNodeObject*> getObjectList() { return this->objects; }
    
    Image getSnap()
    {
        this->snap = createComponentSnapshot(Rectangle<int>(0,0,this->getWidth(), this->getHeight()),
                                             false, 0.4);
        return this->snap;
        
    }
    //==================================================
    //Listener
    class Listener
    {
    public:
        virtual ~Listener() {}
        
    };
    
    ListenerList<Listener> listeners;
    
    virtual void addListener(Listener* newListener) { this->listeners.add(newListener); }
    virtual void removeListener(Listener* listener) { this->listeners.remove(listener);}
    // -------------------------------------------------
    // Callback
    std::function<void()> requestWorkspaceListUpdate;
    std::function<void()> requestSaveProject;
    std::function<void()> notifyEditModeChanged;
    
    //==================================================
    
private:
    
    String name = "";
    String title = "";
    
    Array<IRNodeObject* > objects;
    IRNodeObjectSelector *selector;
    
    Array<IRNodeObject* > copiedObjects;
    
    SelectedItemSet<IRNodeObject*> selectedItemList;
    AudioEngine mixer;
    
    //==================================================
    // dummy object for drag drop action
    Array<IRNodeObject* > dummy;
    
    // IRObjectPtr for Global values between objects binded in Workspace
    std::map<String, IRObjectPtr> p_obj;
    //==================================================
    bool isMultiSelectMode = false;
    bool isPointAlreadySelected = false;
    bool isNewSelectedObjectFound = false;
    
    bool isShiftPressed = false;
    bool isCommandPressed = false;
    bool isControlPressed = false;
    bool isAltPressed = false;
    bool isOptionPressed = false;
    
    Point<int> currentMousePosition{0,0};
    
    //======================================================================
    // workspace status
    bool editModeFlag = true;
    
    //======================================================================
    
    // snapshot
    Image snap;
    
    //======================================================================
    
    // Window for the preference
    PreferenceWindow* preferenceWindow;
    
    // Object list menu
    ObjectListMenu* ObjectMenuComponent;
    std::unique_ptr<ObjectMenuWindow> objMenuwindow;
    //======================================================================
    
    // IRObjectFactory
    IRObjectFactory& IRFactory = singleton<IRObjectFactory>::get_instance();
    //======================================================================
    
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRWorkSpace)
    
};

#endif /* IRWorkSpace_hpp */
