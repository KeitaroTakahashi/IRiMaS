//
//  IRWorkspaceComponent.hpp
//  IRiMaS
//
//  Created by Keitaro on 18/09/2019.
//

#ifndef IRWorkspaceComponent_hpp
#define IRWorkspaceComponent_hpp


#include <map>
#include "IRStr.h"

#include "IRNodeObject.hpp"
#include "AudioEngine.h"
#include "IRNodeObjectSelector.hpp"
#include "IRSaveLoadSystem.hpp"
#include "KLib.h"
#include "Benchmark.h"
#include "KTextLoader.h"
#include "IRWorkspaceCover.hpp"


class IRWorkspaceComponent : public AudioAppComponent,
public IRComponents,
public IRStrComponent,
public ChangeBroadcaster,
public ChangeListener,
public IRNodeObject::Listener,
public IRHeavyWeightComponent,
public KeyListener
{
    
public:
    
    IRWorkspaceComponent(String title, Rectangle<int> draggableMargin, IRStr* str);
    ~IRWorkspaceComponent();
    
    // ==================================================

public:
    
    // paint Graphics called by paint() private method
    virtual void onPaint(Graphics& g) {}
    virtual void onResized() {}
    // ==================================================
    
    virtual void copyAllDataToWorkspace(IRWorkspaceComponent* newWorkspace);
    // ==================================================

    bool isDrawGrids() const { return this->isdrawGridsFlag; }
    void enableDrawGrids(bool flag) { this->isdrawGridsFlag = flag; }
    void setGridsBackgroundAlpha(float alpha) { this->gridsBackgroundAlpha = alpha; };
    
    // ==================================================
    void setFixObjectSizeRatio(bool flag, Rectangle<float> initialBounds);
    void setPreviousBounds(Rectangle<float> previousBounds);
    bool isPreviousBoundsAlreadyGiven() const;
private:
    void paint (Graphics& g) override;
    void drawShadows(Graphics& g);
   // void drawGrids(Graphics& g);
    
    // ==================================================

    void resized() override;
    void resizeNodeObjectsRelativeToWorkspaceSizeChange();
    
    // ==================================================

    String getTitle() { return this->title; }
    
    // interaction
    void mouseDown(const MouseEvent& e) override; // JUCE oriented
    void mouseMove(const MouseEvent& e) override; // JUCE oriented
    void mouseUp(const MouseEvent& e)override; // JUCE oriented
    void mouseDrag(const MouseEvent& e) override; // JUCE oriented
    void mouseDoubleClick(const MouseEvent& e) override; // JUCE oriented
    void modifierKeysChanged(const ModifierKeys &mod) override; // JUCE oriented
    bool keyPressed(const KeyPress& key, Component* originatingComponent) override; // JUCE oriented
public:
    void registerKeyListener(KeyListener* key);
    void registerMouseListener(MouseListener* mouse);
    
private:
    //Listener
    void changeListenerCallback(ChangeBroadcaster* source) override; // JUCE oriented
    
    // AudioAppComponent
    void AudioSetup();
    void closeAudioSetup();
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    virtual void releaseResources() override;
    
    
    // ==================================================
    // APPEARANCE
public:
    void setBackgroundColour(Colour colour);
private:
    Colour backgroundColour = Colours::white;
public:

    AudioSource& getMixer();
    
    // IRNodeObject Listener
    void dragoutNodeObjectFromParent(IRNodeObject* obj) override;
    void dropoutNodeObjectFromParent(IRNodeObject* obj) override;
    void editModeChangedInNodeObject(bool editMode) override;
    
    void saveProject() override;
    void saveAsProject() override;
    void closeProject() override;
    void createNewProject() override;
    void openProject() override;
    void openFileInspecter() override;
    //void openPreferenceWindow() override;
    
    // get to know when NodeObject is modified e.g. loading new file.
    void nodeObjectModifiedNotification(IRNodeObject* obj) override;
    void nodeObjectSelectionChange(IRNodeObject* obj) override;
    void nodeObjectGetFocused(IRNodeObject* obj) override;
    void nodeObjectMoveToFront(IRNodeObject* obj) override;
    void nodeObjectMoveToBack(IRNodeObject* obj) override;
    void reorderZIndex() override;


    void heavyComponentCreated(IRNodeObject* obj)override;
    void addHeavyCopmonentToTopZOrder(IRNodeObject* obj) override;

    
    // object control
    void copySelectedObjects();
    void pasteSelectedObjects();
    void duplicateSelectedObjects();
    void deleteSelectedObjects();
    
    void deselectAllObjects();
    
    
    // manage the heavy weight components which are always drown on the top of all other components
    // the heavy weights components will be hidden or shown according to this workspace status
    void manageHeavyWeightComponents(bool flag);
    // ==================================================


    // ==================================================

    // object management
    void createObject(IRNodeObject* obj, bool shouldSort = true);
    // void createObject(std::string objName);
    void copyObject(IRNodeObject *obj, bool clearCopied);
    void pasteObject(IRNodeObject *obj, bool addToSelected);
    void duplicateObject(IRNodeObject *obj);
    void deleteObject(IRNodeObject *obj);
    
    // for parent node object, use this instead of createObject()
    void createParentObject(IRNodeObject* obj, bool shouldSort = true);
    
    // call this after complete creting the heavy objects
    void resortHeavyObject();

    // ==================================================

    //resizing
    void setResizing(bool flag) { this->isResizingFlag = flag; }
    bool isResizing() const { return this->isResizingFlag; }
    
    // should be called before resized
    void setDraggableMargin(Rectangle<int> newMargin);
    // ==================================================

    // ==================================================

    // save load
    json11::Json makeSaveDataOfThis();
    // ==================================================

    //flag
    bool isEditMode() const;
    void setEditMode(bool flag, bool notification = false);

    // ==================================================

    // getter
    Array<IRNodeObject*> getObjectList();
    Image getSnap();
    // ==================================================
    
    
    // ==================================================

    // ==================================================

    //##### Time Code Animation ######
public:
    
    void resetAnimatedObjectList();
    void enableTimeCodeAnimation(bool flag);
    bool isTimeCodeAnimationEnabled() const { return this->timeCodeAnimationEnable; }
    
    void setCurrentTimeCode(float currentTime);
    
    // update animation with already set current Time
    void updateCurrentAnimation();
    // update animation with currentTime
    void updateCurrentAnimation(float currentTime);
    
private:
    bool timeCodeAnimationEnable = false;
    // store objects currently animated by IRNodeObjectAnimation such as VideoAnnotater
    Array<IRNodeObject* > currentlyAnimatedObjects;
    
    void addCurrentlyAnimatedObjectList(IRNodeObject* obj);
    void removeFromCurrentlyAnmatedObjectList(IRNodeObject* obj);
    
    void startAnimated(IRNodeObject* obj);
    void endAnimated(IRNodeObject* obj);
    
    float currentTime = 0;
    // ==================================================
    // ==================================================
public:

    //Listener
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        virtual void nodeObjectSelectionChange(IRNodeObject* obj) {};
        virtual void nodeObjectGetFocused(IRNodeObject* obj) {};
        
        virtual void nodeObjectCopied(IRNodeObject* obj) {};
        virtual void nodeObjectPasted(IRNodeObject* obj) {};
        virtual void nodeObjectWillDeleted(IRNodeObject* obj) {};

        
        virtual void nothingSelected() {};
        
        virtual void editModeChanged(IRWorkspaceComponent* changedSpace) = 0;
        
        virtual void heavyObjectCreated(IRNodeObject* obj) {};
        
    };
    
    ListenerList<Listener> listeners;
    
    virtual void addListener(Listener* newListener);
    virtual void removeListener(Listener* listener);
    
    void callNodeObjectSelectionChange(IRNodeObject* obj);
    void callNodeObjectGetFocused(IRNodeObject* obj);
    void callEditModeChanged();
    void callHeavyObjectCreated(IRNodeObject* obj);
    void callNothingSelected();
    
    void callNodeObjectCopied(IRNodeObject* obj);
    void callNodeObjectPasted(IRNodeObject* obj);
    void callNodeObjectWillDeleted(IRNodeObject* obj);


    // Callback
    std::function<void()> requestWorkspaceListUpdate;
    std::function<void()> requestNewProject;
    std::function<void()> requestSaveProject;
    std::function<void()> requestSaveAsProject;
    std::function<void()> requestCloseProject;
    std::function<void()> requestOpenProject;
    std::function<void()> requestOpenFileInspecter;
    //std::function<void()> requestOpenPreferenceWindow;

    std::function<void()> notifyEditModeChanged;

    std::function<void(IRNodeObject*)> notifyNodeObjectModification;
    
    

    
private:
    // ==================================================

    String name = "";
    String title = "";
    
    IRStr* ir_str;
    
    Rectangle<int> draggableMargin;
    
    // resizing...
    bool isResizingFlag = false;

    // ==================================================
    //store the original bound size
    Rectangle<float> initialBounds;
    
    void initialBoundsUpdated(IRNodeObject* obj) override;
public:
    void setInitialBounds(Rectangle<float> initialBounds) { this->initialBounds = initialBounds; }
    Rectangle<float> getInitialBounds() const { return this->initialBounds; }
    // ==================================================
private:
    // grids
    int thin_grids_pixel = 10;
    int thick_grids_interval = 50;
    float grid_thickness = 1.0;
    float grid_thickness2 = 0.5;
    
    float gridsBackgroundAlpha = 0.1;
    // ==================================================

    // Objects
    Array<IRNodeObject* > objects;
protected:
    Array<IRNodeObject* > getObjects() const { return this->objects; }
private:

    IRNodeObjectSelector *selector;
    
    Array<IRNodeObject* > copiedObjects;
    
    AudioEngine mixer;
    
    // dummy object for drag drop action
    Array<IRNodeObject* > dummy;
    
    // IRObjectPtr for Global values between objects binded in Workspace
    std::map<String, IRObjectPtr> p_obj;
    
    
    // parent node object has role of fundamental object in the annotation mode
    IRNodeObject* parentNodeObject = nullptr;
    bool hasParentNodeObjectFlag = false;
    void bringParentNodeObjectToFront();
    void bringParentNodeObjectToBack();
public:
    bool hasParentNodeObject() const { return this->hasParentNodeObjectFlag; }
    void setParentNodeObject(IRNodeObject* newParentNodeObject);
    void removeParentNodeObject();
    // ==================================================
private:
    // cover with grids
    std::shared_ptr<IRWorkspaceCover> cover;
    void createCover();
    void bringCoverToFront();
    void bringCoverToBack();
    void setCoverEditMode(bool editMode);
    // ==================================================


    bool isMultiSelectMode = false;
    bool isPointAlreadySelected = false;
    bool isNewSelectedObjectFound = false;
    
    bool isShiftPressed = false;
    bool isCommandPressed = false;
    bool isControlPressed = false;
    bool isAltPressed = false;
    bool isOptionPressed = false;
    
    bool isdrawGridsFlag = true;

    // ==================================================

    juce::Point<int> currentMousePosition {0, 0};
    
    // workspace status
    bool editModeFlag = true;
    
    //resize object position
    bool isFixObjectSizeRatio = true;
    
    
    
    Rectangle<float> previousBounds;

    // snapshot
    Image snap;
    // ==================================================

    // Window for the preference
    //PreferenceWindow* preferenceWindow;
    // ==================================================

    // Object list menu
    //ObjectListMenu* objectMenuComponent;
    //std::unique_ptr<ObjectMenuWindow> objMenuwindow;
    // ==================================================

    
    //Z-order of the objects
    std::vector<IRNodeObject* > ObjectZorder;
    void insertObjectAtTopZOrder(IRNodeObject* obj);
    void insertObjectAtEndZOrder(IRNodeObject* obj);
    void removeObjectFromZOrder(IRNodeObject* obj);
    
    // ==================================================

    Image loadImage(String url);
    
// ==================================================
   //OpenGL
   //OpenGLContext openGLContext;
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
    StopWatch bench2;
    StopWatch bench3;
    StopWatch bench4;

    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRWorkspaceComponent)
        
};


#endif /* IRWorkspaceComponent_hpp */
