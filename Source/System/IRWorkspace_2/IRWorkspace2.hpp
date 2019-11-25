//
//  IRWorkspace.hpp
//  IRiMaS
//
//  Created by Keitaro on 18/09/2019.
//

#ifndef IRWorkspace_hpp
#define IRWorkspace_hpp


#include <map>
#include "IRStr.h"

#include "PreferenceWindow.hpp"

#include "IRNodeObject.hpp"
#include "AudioEngine.h"
#include "IRNodeObjectSelector.hpp"
#include "ObjectMenuWindow.hpp"

#include "ExternalObjectHeader.h"
#include "IRSaveLoadSystem.hpp"

#include "KLib.h"
#include "IRMessage.h"

#include "Benchmark.h"


class IRWorkspace : public AudioAppComponent,
public IRComponents,
public IRStrComponent,
public ChangeBroadcaster,
public ChangeListener,
public IRNodeObject::Listener,
public ObjectListMenu::Listener,
public OpenGLRenderer,
private KeyListener
{
    
public:
    
    IRWorkspace(String title, IRStr* str, PreferenceWindow* preferenceWindow);
    ~IRWorkspace();
    
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
    void nodeObjectSelectionChange(IRNodeObject* obj) override;
    void nodeObjectGetFocused(IRNodeObject* obj) override;
    void nodeObjectMoveToFront(IRNodeObject* obj) override;
    void heavyComponentCreated(IRNodeObject* obj)override;
    void addHeavyCopmonentToTopZOrder(IRNodeObject* obj) override;

    
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
    // ==================================================


    // ==================================================

    // object management
    void createObject(IRNodeObject* obj);
    // void createObject(std::string objName);
    void copyObject(IRNodeObject *obj, bool clearCopied);
    void pasteObject(IRNodeObject *obj, bool addToSelected);
    void duplicateObject(IRNodeObject *obj);
    void deleteObject(IRNodeObject *obj);
    // ==================================================

    // object menu
    void openObjectListMenu(Point<int>Pos);
    void closeObjectListMenu();
    // ==================================================

    void itemSelectionAction(ObjectListMenu* menu) override;
    void itemHasSelectedAction(ObjectListMenu* menu) override;
    // ==================================================

    //resizing
    void setResizing(bool flag) { this->isResizingFlag = flag; }
    bool isResizing() const { return this->isResizingFlag; }
    // ==================================================

    // Link Menu
    void openLinkMenuOfSelectedObject();
    void openAllLinkMenu();
    void closeLinkMenu();
    void closeLinkMenu(IRNodeObject* obj);
    
    // ==================================================

    // save load
    json11::Json makeSaveDataOfThis();
    // ==================================================

    //flag
    bool isEditMode() const;
    void setEditMode(bool flag, bool notification = false);
    
    bool isLinkMode() const;
    void setLinkMode(bool flag);
    // ==================================================

    // getter
    Array<IRNodeObject*> getObjectList();
    Image getSnap();
    // ==================================================

    //Listener
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        virtual void nodeObjectSelectionChange(IRNodeObject* obj) {};
        virtual void nodeObjectGetFocused(IRNodeObject* obj) {};
        
        virtual void editModeChanged(IRWorkspace* changedSpace) = 0;
        
        virtual void heavyObjectCreated(IRNodeObject* obj) {};
        
    };
    
    ListenerList<Listener> listeners;
    
    virtual void addListener(Listener* newListener);
    virtual void removeListener(Listener* listener);
    
    void callNodeObjectSelectionChange(IRNodeObject* obj);
    void callNodeObjectGetFocused(IRNodeObject* obj);
    void callEditModeChanged();
    void callHeavyObjectCreated(IRNodeObject* obj);

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
    // ==================================================

    String name = "";
    String title = "";
    
    IRStr* ir_str;
    
    // resizing...
    bool isResizingFlag = false;
    // ==================================================

    // grids
    int thin_grids_pixel = 10;
    int thick_grids_interval = 50;
    float grid_thickness = 1.0;
    float grid_thickness2 = 0.5;
    
    // ==================================================

    Array<IRNodeObject* > objects;
    IRNodeObjectSelector *selector;
    
    Array<IRNodeObject* > copiedObjects;
    
    AudioEngine mixer;
    
    // dummy object for drag drop action
    Array<IRNodeObject* > dummy;
    
    // IRObjectPtr for Global values between objects binded in Workspace
    std::map<String, IRObjectPtr> p_obj;
    // ==================================================

    bool isMultiSelectMode = false;
    bool isPointAlreadySelected = false;
    bool isNewSelectedObjectFound = false;
    
    bool isShiftPressed = false;
    bool isCommandPressed = false;
    bool isControlPressed = false;
    bool isAltPressed = false;
    bool isOptionPressed = false;
    // ==================================================

    Point<int> currentMousePosition {0, 0};
    
    // workspace status
    bool editModeFlag = true;
    bool linkModeFlag = false;
    
    // message
    IRMessage message;
    // snapshot
    Image snap;
    // ==================================================

    // Window for the preference
    PreferenceWindow* preferenceWindow;
    // ==================================================

    // Object list menu
    ObjectListMenu* objectMenuComponent;
    std::unique_ptr<ObjectMenuWindow> objMenuwindow;
    // ==================================================

    // IRObjectFactory
    IRObjectFactory& IRFactory = singleton<IRObjectFactory>::get_instance();
    
    //Z-order of the objects
    std::vector<IRNodeObject* > ObjectZorder;
    void insertObjectAtTopZOrder(IRNodeObject* obj);
    void removeObjectFromZOrder(IRNodeObject* obj);
    
    // ==================================================

    Image background_image_link;
    void loadBackgroundImageLink();
    Image loadImage(String url);
    
// ==================================================
   //OpenGL
   OpenGLContext openGLContext;
   std::unique_ptr<OpenGLGraphicsContextCustomShader> shader;
   String fragmentCode;
   GLuint textureID;
   IRTextLoader fragmentText;
   bool isTextureCreated = false;
   bool updateTexture = false;
   float* buffer = nullptr;
   
   bool isOpenGLComponentClosed = false;
   
   String fragURL;
   
   bool fragmentRefreshed = false;
    
    void openGLInit();
    void shaderTask(Graphics& g);
    void update();
   void createTexture();
   //void createDemoTexture();
   void updateFragment();
   void setUniform(OpenGLShaderProgram& program);
    
    int sp_w = 100;
    int sp_h = 100;
    // max size of w and h of Spectrogram
    // 1024
    int MAX_TEXTURE_SIZE = 512;
    
    float ratio_x = 1.0;
    float ratio_y = 1.0;
// ==================================================
    //benchmark
    StopWatch bench;
    // ==================================================
    
    // ----------
    //OpenGL
    //void setOpenGLContextSurfaceOpacityToZero();
    
    void newOpenGLContextCreated()override
    {
    }
       
    virtual void renderOpenGL()override
    {
        OpenGLHelpers::clear(Colours::transparentBlack);
    }
    
    virtual void openGLContextClosing() override
    {
        
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRWorkspace)
        
};


#endif /* IRWorkspace_hpp */
