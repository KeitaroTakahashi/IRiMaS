
#ifndef IRProject_hpp
#define IRProject_hpp

#include <stdio.h>
#include "JuceHeader.h"
#include "PreferenceWindow.hpp"
#include "IRFileInspecterWindow.hpp"
#include "IRWorkSpace.hpp"
#include "IRWorkspaceList.hpp"
#include "IRFoundation.h"
#include "Menus.h"
#include "EditCommandTarget.hpp"

class IRProject : public AudioAppComponent,
                  public IRComponents,
                  public ApplicationCommandTarget,
                  public MenuBarModel,
                  public ChangeListener,
                  public ChangeBroadcaster,
                  private EditCommandTarget::Listener,
                  private IRWorkspaceList::Listener
{
public:
    
    enum class MenuBarPosition
    {
        window, // for windows
        global, // for mac
        burger
    };
    
    enum MenuActionStatus
    {
        CreateNewProjectAction,
        OpenProjectAction,
        CloseProjectAction,
        SaveProjectAction
    };
    
    
    IRProject(std::string projectName, Rectangle<int> frameRect,
              PreferenceWindow* preferenceWindow,
              DocumentWindow* parentWindow,
              ChangeListener* listener);
    
    ~IRProject();
    
    void resized() override;
    
    void createProject(); // call listener object to fire signal to IRMAIN
    void createNewProject();
    void saveProject();
    void saveAsProject();
    void openProject();
    void closeProject();
    
    void createNewWorkspace();
    void createWorkspaceList();
    void removeWorkspace(IRWorkSpace* workspace) override;
    void updateWorkspaceList();
    
    
    // ==================================================
    // windows
    
    void openFileInspecterWindow();
    void openPreferenceWindow();

    void updateFileInspecterWindow();
    
    // ==================================================

    // change edit mode : signal received from Menu object
    // this is only fired when Menu obejct has keyboard Focus
    // otherwise it doesn't work.
    void performEditModeChange() override;
    
    // notify the status change of edit mode.
    // this is called when IRUIFoundation, IRNodeObject, IRWorkspace or any other object
    // has changed the edit mode
    void notifyEditModeChange();
    
    void performLinkModeChange() override;
    
    void notifyLinkModeChange();
    
    // receive notifications from IRWorkspaces reporting any modifications of their NodeObjects
    void receiveNodeObjectModification(IRNodeObject* obj);
    
    // return top workspace which is currently displayed
    IRWorkSpace* getTopWorkspace();
    
    // return true if success!
    json11::Json saveAction(std::string filePath);
    
    // Menu bar methods
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex(int menuIndex, const String& menuName) override;
    void menuItemSelected (int menuItemID, int topLevelMenuIndex) override;
    MenuActionStatus getMenuActionStatus() const;
    
    IRWorkspaceList* getWorkspaceList();
    
    void setProjectPath(std::string path);
    std::string getProjectPath() const;
    
    void setProjectName(std::string name);
    std::string getProjectName() const;
    
    // get parent ProjectWindow
    DocumentWindow* getParentWindow();
    
    // initialize projects. This method is called after loading action to set up workspaces
    void initProjectAfterLoading();
    
    // ==================================================
    
    // Listener for Menu action
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        virtual void createNewProjectAction() {};
        virtual void openProjectAction() {};
        virtual void closeProjectAction(DocumentWindow* closingWindow) {};
        virtual void saveProjectAction(IRProject* project) {};
        virtual void saveAsProjectAction(IRProject* project) {};
    };
    
    virtual void addListener(Listener* newListener) { this->listeners.add(newListener); }
    virtual void removeListener(Listener* listener) { this->listeners.remove(listener); }
    
    ListenerList<Listener> listeners;
    
    std::function<void()> createNewProjectActionCompleted;
    std::function<void()> openProjectActionCompleted;
    std::function<void()> closeProjectActionCompleted;
    std::function<void()> saveProjectActionCompleted;
    std::function<void()> saveAsProjectActionCompleted;
    
    // edit mode changed call back
    std::function<void()> notifyEditModeChanged;
    std::function<void()> notifyLinkModeChanged;
    
    void callCreateNewProjectAction(); //fire createNewProjectAction() method in the Listener
    void callOpenProjectAction(); //fire openProjectAction() method in the Listener
    void callCloseProjectAction(); //fire closeProjectAction() method in the Listener
    void callSaveProjectAction(); // fire saveProjectAction() method in the Listener
    void callSaveAsProjectAction(); // fire saveAsProjectAction() method in the Listener
    
    bool isEditMode() const;
    bool isLinkMode() const;
    bool isNonSavedChange() const;
    void setNonSavedChange(bool newStatus);
    
private:
    
    
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
    // Menu bar
    void setMenuBarPosition(MenuBarPosition newPosition);
    ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands(Array<CommandID>&c) override;
    void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
    bool perform(const InvocationInfo& info) override;
    
    // AudioAppComponent
    void audioSetup();
    void closeAudioSetup();
    
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    virtual void releaseResources() override;
    
    std::string projectPath;
    std::string projectName;
    
    Rectangle<int> frameRect;
    DocumentWindow* parentWindow = nullptr;
    std::vector<IRWorkSpace*> workspaces;
    IRFileInspecterWindow* fileInspecterWindow = nullptr;
    
    
    
    // handle a currently displayed workspace
    // do not forget to call toFront() and manageHeavyWeightCoponent() methods when update it.
    IRWorkSpace* topSpace = nullptr;
    
    // IRWorkspaceList *workspaceList = nullptr;
    std::shared_ptr<IRWorkspaceList> workspaceList { nullptr };
    
    int workspaceListWidth = 100;
    
    PreferenceWindow* preferenceWindow;
    
    ChangeListener* listener;
    
    IRSaveLoadSystem saveLoadClass;
    
    AudioEngine mixer;
    
    // Edit Mode flag
    bool EditModeFlag = true;
    bool LinkModeFlag = false;
    
    // is there any changes which have not been saved yet?
    bool nonSavedChanges = false;
    
    ApplicationCommandManager commandManager;
    
    std::unique_ptr<MenuBarComponent> menuBar;
    MenuBarPosition menuBarPosition = MenuBarPosition::window;
    // EditCommandTarget* editCommandTarget;
    std::unique_ptr<EditCommandTarget> editCommandTarget;
    
    SidePanel sidePanel { "Menu", 300, false };
    
    MenuActionStatus menu_action_status;
    
    json11::Json saveData;
    
    // --------------------------------------------------    
};


#endif /* IRProject_hpp */




