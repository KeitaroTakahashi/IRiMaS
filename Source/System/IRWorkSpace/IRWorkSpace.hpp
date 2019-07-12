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

#include "PreferenceWindow.hpp"

#include "IRNodeObject.hpp"
#include "IRWorkSpaceLinkManager.hpp"
#include "AudioEngine.h"
#include "IRNodeObjectSelector.hpp"
#include "ObjectMenuWindow.hpp"

#include "ExternalObjectHeader.h"
#include "IRSaveLoadSystem.hpp"

#include "KLib.h"


class IRWorkSpace : public AudioAppComponent,
                    public IRComponents,
                    public IRWorkSpaceLinkManager,
                    public ChangeBroadcaster,
                    public ChangeListener,
                    public IRNodeObject::Listener,
                    public ObjectListMenu::Listener,
                    private KeyListener
{
    
public:
    
    IRWorkSpace(String title, Rectangle<int> frameRect, PreferenceWindow* preferenceWindow);
    ~IRWorkSpace();
    
    void paint (Graphics&) override;
    void drawShadows(Graphics& g);
    void drawGrids(Graphics& g);
    void resized() override;
    String getTitle() { return this->title; }
    
    // interaction
    void mouseDown(const MouseEvent& e) override; // JUCE oriented
    void mouseMove(const MouseEvent& e) override; // JUCE oriented
    void mouseUp(const MouseEvent& e)override; // JUCE oriented
    void mouseDrag(const MouseEvent& e) override; // JUCE oriented
    void mouseDoubleClick(const MouseEvent& e) override; // JUCE oriented
    void modifierKeysChanged(const ModifierKeys &mod) override;
    bool keyPressed(const KeyPress& key, Component* originatingComponent) override;
    
    //Listener
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
    // AudioAppComponent
    void AudioSetup();
    void closeAudioSetup();
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    virtual void releaseResources() override;
    AudioSource& getMixer();
    
    // IRNodeObject Listener
    void dragoutNodeObjectFromParent(IRNodeObject* obj) override;
    void dropoutNodeObjectFromParent(IRNodeObject* obj) override;
    void editModeChangedInNodeObject(bool editMode) override;
    void linkModeChangedInNodeObject(bool editMode) override;
    void getSelectedLinkSystemFlag(IRNodeObject* obj) override;

    void saveProject() override;
    void saveAsProject() override;
    void closeProject() override;
    void createNewProject() override;
    void openProject() override;
    void openFileInspecter() override;
    void openPreferenceWindow() override;

    // get to know when NodeObject is modified e.g. loading new file.
    void nodeObjectModifiedNotification(IRNodeObject* obj) override;

    void addObjectGlobal(IRObjectPtr obj, String id) override;
    void getObjectGlobal(IRNodeObject* obj) override;
    
    // object control
    void copySelectedObjects();
    void pasteSelectedObjects();
    void duplicateSelectedObjects();
    void deleteSelectedObjects();
    
    // manage the heavy weight components which are always drown on the top of all other components
    // the heavy weights components will be hidden or shown according to this workspace status
    void manageHeavyWeightComponents(bool flag);
    
    // object management
    void createObject(IRNodeObject* obj);
    // void createObject(std::string objName);
    void copyObject(IRNodeObject *obj, bool clearCopied);
    void pasteObject(IRNodeObject *obj, bool addToSelected);
    void duplicateObject(IRNodeObject *obj);
    void deleteObject(IRNodeObject *obj);
    
    // object menu
    void openObjectListMenu(Point<int>Pos);
    void closeObjectListMenu();
    
    void itemSelectionAction(ObjectListMenu* menu) override;
    void itemHasSelectedAction(ObjectListMenu* menu) override;
    
    
    // Link Menu
    void openLinkMenuOfSelectedObject();
    void openAllLinkMenu();
    void closeLinkMenu();
    void closeLinkMenu(IRNodeObject* obj);

    
    // save load
    json11::Json makeSaveDataOfThis();
    
    //flag
    bool isEditMode() const;
    void setEditMode(bool flag);
    
    bool isLinkMode() const;
    void setLinkMode(bool flag);
    
    // getter
    Array<IRNodeObject*> getObjectList();
    Image getSnap();
    
    //Listener
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        
    };
    
    ListenerList<Listener> listeners;
    
    virtual void addListener(Listener* newListener);
    virtual void removeListener(Listener* listener);
    
    // Callback
    std::function<void()> requestWorkspaceListUpdate;
    std::function<void()> requestNewProject;
    std::function<void()> requestSaveProject;
    std::function<void()> requestSaveAsProject;
    std::function<void()> requestCloseProject;
    std::function<void()> requestOpenProject;
    std::function<void()> requestOpenFileInspecter;
    std::function<void()> requestOpenPreferenceWindow;

    std::function<void()> notifyEditModeChanged;
    std::function<void()> notifyLinkModeChanged;

    std::function<void(IRNodeObject*)> notifyNodeObjectModification;
    
private:
    
    String name = "";
    String title = "";
    
    // grids
    int thin_grids_pixel = 10;
    int thick_grids_interval = 50;
    float grid_thickness = 1.0;
    float grid_thickness2 = 0.5;

    
    Array<IRNodeObject* > objects;
    IRNodeObjectSelector *selector;
    
    Array<IRNodeObject* > copiedObjects;
    
    SelectedItemSet<IRNodeObject*> selectedItemList;
    AudioEngine mixer;
    
    // dummy object for drag drop action
    Array<IRNodeObject* > dummy;
    
    // IRObjectPtr for Global values between objects binded in Workspace
    std::map<String, IRObjectPtr> p_obj;
    
    bool isMultiSelectMode = false;
    bool isPointAlreadySelected = false;
    bool isNewSelectedObjectFound = false;
    
    bool isShiftPressed = false;
    bool isCommandPressed = false;
    bool isControlPressed = false;
    bool isAltPressed = false;
    bool isOptionPressed = false;
    
    Point<int> currentMousePosition {0, 0};
    
    // workspace status
    bool editModeFlag = true;
    bool linkModeFlag = false;
    // snapshot
    Image snap;
    
    // Window for the preference
    PreferenceWindow* preferenceWindow;
    
    // Object list menu
    ObjectListMenu* objectMenuComponent;
    std::unique_ptr<ObjectMenuWindow> objMenuwindow;
    
    // IRObjectFactory
    IRObjectFactory& IRFactory = singleton<IRObjectFactory>::get_instance();
    
    Image background_image_link;
    void loadBackgroundImageLink();
    Image loadImage(String url);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRWorkSpace)
};





#endif /* IRWorkSpace_hpp */




