//
//  IRProject.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 06/11/2018.
//

#ifndef IRProject_hpp
#define IRProject_hpp

#include <stdio.h>
#include "JuceHeader.h"
#include "PreferenceWindow.hpp"
#include "IRWorkSpace.hpp"
#include "IRWorkspaceList.hpp"
#include "IRFoundation.h"
#include "Menus.h"

class IRProject : public AudioAppComponent,
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
    
    // ========================================
    
    void createProject() { createWorkspaceList(); }
    // call listener object to fire signal to IRMAIN
    void createNewProject() { callCreateNewProjectAction(); }
    // call listener object to fire signal to IRMAIN
    void saveProject() { callSaveProjectAction(); }
    void saveAsProject() { callSaveAsProjectAction(); }
    // call listener object to fire signal to IRMAIN
    void openProject() { callOpenProjectAction(); }
    void closeProject()
    {
        this->menu_action_status = MenuActionStatus::CloseProjectAction;
        sendChangeMessage();
        callCloseProjectAction();
    }
    // ========================================
    // create new workspace
    void createNewWorkspace();
    void createWorkspaceList()
    {
        this->workspaceList = new IRWorkspaceList(Rectangle<int>(0,0,this->workspaceListWidth,getHeight()));
        this->workspaceList->addListener(this);
        this->workspaceList->addChangeListener(this);
        addAndMakeVisible(this->workspaceList);
    }
    // IRWorkspaceList Listener callback
    void removeWorkspace(IRWorkSpace* workspace) override
    {
        auto it = std::find(this->workspaces.begin(), this->workspaces.end(), workspace);
        if (it != this->workspaces.end()) {
            this->workspaces.erase(it);
        }
    }
    // update workspace list. warning. this may be a little bit expensive task
    void updateWorkspaceList()
    {
        this->workspaceList->updateList();
    }
    // ========================================
    
    // change edit mode : signal received from Menu object
    // this is only fired when Menu obejct has keyboard Focus
    // otherwise it doesn't work.
    void performEditModeChange() override;
    
    // return top workspace which is currently displayed space
    IRWorkSpace* getTopWorkspace() { return this->topSpace;}
    
    // return true if success!
    json11::Json saveAction(std::string filePath);
    //json11::Json loadAction(std::string filePath);
    
    // ========================================
    // Menu bar methods
    // ========================================
    StringArray getMenuBarNames() override
    {
        return { "File", "Edit" , "Window", "Help" };
    }
    
    PopupMenu getMenuForIndex(int menuIndex, const String& menuName) override;;
    
    void menuItemSelected (int /*menuItemID*/, int /*topLevelMenuIndex*/) override {}
    
    MenuActionStatus getMenuActionStatus() const { return this->menu_action_status; }
    
    IRWorkspaceList* getWorkspaceList() { return this->workspaceList; }
    
    void setProjectPath(std::string path) { this->projectPath = path; }
    std::string getProjectPath() const { return this->projectPath; }
    
    void setProjectName(std::string name) { this->projectName = name; }
    std::string getProjectName() const { return this->projectName; }
    
    // get parent ProjectWindow
    DocumentWindow* getParentWindow() { return this->parentWindow; }
    
    // initialize projects. This method is called after loading action to set up workspaces
    void initProjectAfterLoading()
    {
        for(auto space : this->workspaces)
        {
            space->setEditMode(false);
        }
        
        this->topSpace = this->workspaces[0];
        this->topSpace->toFront(true);
        getWorkspaceList()->setSelectedComponentIndex(0);
        
        getWorkspaceList()->updateList();
    }
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
    virtual void removeListener(Listener* listener) { this->listeners.remove(listener);}
    
    ListenerList<Listener> listeners;
    
    std::function<void()> createNewProjectActionCompleted;
    std::function<void()> openProjectActionCompleted;
    std::function<void()> closeProjectActionCompleted;
    std::function<void()> saveProjectActionCompleted;
    std::function<void()> saveAsProjectActionCompleted;
    
    // edit mode changed call back
    std::function<void()> notifyEditModeChanged;
    
    //fire createNewProjectAction() method in the Listener
    void callCreateNewProjectAction();
    //fire openProjectAction() method in the Listener
    void callOpenProjectAction();
    //fire closeProjectAction() method in the Listener
    void callCloseProjectAction();
    // fire saveProjectAction() method in the Listener
    void callSaveProjectAction();
    // fire saveAsProjectAction() method in the Listener
    void callSaveAsProjectAction();
    
    
    
    // ==================================================
    
    bool isEditMode() const { return this->EditModeFlag; }
    
    
private:
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        if(source == this->workspaceList)
        {
            // if workspace item is selected
            if(this->workspaceList->getEventStatus() == IRWorkspaceList::listEventStatus::listEventSelected){
                IRWorkSpace* space = static_cast<IRWorkSpace* >(this->workspaceList->getSelectedComponent());
                
                if(space != this->topSpace){
                    
                    // first hide all heavy weight component
                    this->topSpace->manageHeavyWeightComponents(false);
                    this->topSpace = space;
                    // show all heavy weight component
                    this->topSpace->manageHeavyWeightComponents(true);
                    // bring the selected workspace on the top of all
                    space->toFront(true);
                    repaint();
                    
                    // refocus keyboard to workspacelist
                    this->workspaceList->workspaceListFocused();
                }
            }
        }
    }
    // ==================================================
    // Menu bar
    // ==================================================
    
    void setMenuBarPosition ( MenuBarPosition newPosition );
    
    ApplicationCommandTarget* getNextCommandTarget() override;
    
    void getAllCommands(Array<CommandID>&c) override;
    
    void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
    
    bool perform(const InvocationInfo& info) override;
    
    //==============================================================================
    
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
    
    //==============================================================================
    std::string projectPath;
    std::string projectName;
    
    Rectangle<int> frameRect;
    DocumentWindow* parentWindow;
    std::vector<IRWorkSpace*> workspaces;
    
    // handle a currently displayed workspace
    // do not forget to call toFront() and manageHeavyWeightCoponent() methods when update it.
    IRWorkSpace* topSpace = nullptr;
    
    IRWorkspaceList *workspaceList = nullptr;
    
    int workspaceListWidth = 150;
    
    PreferenceWindow* preferenceWindow;
    
    ChangeListener* listener;
    
    IRSaveLoadSystem saveLoadClass;
    
    AudioEngine mixer;
    
    // Edit Mode flag
    bool EditModeFlag = true;
    
    ApplicationCommandManager commandManager;
    
    std::unique_ptr<MenuBarComponent> menuBar;
    MenuBarPosition menuBarPosition = MenuBarPosition::window;
    EditCommandTarget* editCommandTarget;
    
    SidePanel sidePanel { "Menu", 300, false };
    
    MenuActionStatus menu_action_status;
    
    //==============================================================================
    
    json11::Json saveData;
    
    //==============================================================================
    
    
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    
};


#endif /* IRProject_hpp */
