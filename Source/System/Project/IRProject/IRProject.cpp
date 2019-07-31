
#include "IRProject.hpp"




IRProject::IRProject(std::string projectName, Rectangle<int> frameRect,
                     PreferenceWindow* preferenceWindow,
                     DocumentWindow* parentWindow,
                     ChangeListener* listener)
{
    this->projectName = projectName;
    this->preferenceWindow = preferenceWindow;
    this->parentWindow = parentWindow;
    this->listener = listener;
    this->frameRect = frameRect;
    
    setSize(frameRect.getWidth(), frameRect.getHeight());
    
    // ==========
    // create File Manager
    
    createFileManager();
    
    // ==========
    
    //create menu
    this->menuBar.reset(new MenuBarComponent(this));
    
    // for mac only. Windows takes window menu
#if JUCE_MAC
    //setMenuBarPosition(MenuBarPosition::global);
#endif
    
    addAndMakeVisible(this->menuBar.get());
    setApplicationCommandManagerToWatch(&this->commandManager);
    
    
    this->commandManager.registerAllCommandsForTarget (this);
    
    addKeyListener(this->commandManager.getKeyMappings());
    // this->editCommandTarget = new EditCommandTarget( commandManager );
    this->editCommandTarget = std::make_unique<EditCommandTarget>(commandManager);
    this->editCommandTarget->addListener(this);
    
    addAndMakeVisible(this->editCommandTarget.get());

    
    // setup
    setAudioChannels(0, 2);
}


IRProject::~IRProject()
{
    std::cout << "beg of IR Project destructor" << std::endl;
    
    shutdownAudio();
    
    if(this->fileInspecterWindow != nullptr)
        delete this->fileInspecterWindow;
    
    for (auto w : this->workspaces)
    {
        delete w;
    }
    
    std::cout << "end of IR Project destructor" << std::endl;
}


void IRProject::resized()
{
    
    for(auto space : this->workspaces)
    {
        Rectangle<int> frameSize(this->workspaceListWidth, 0,
                                 getWidth()-this->workspaceListWidth, getHeight());
        space->setBounds(frameSize);
    }
    
    if(this->workspaceList != nullptr)
    {        this->workspaceList->setBounds(Rectangle<int>(0,0,this->workspaceListWidth,getHeight()));
        
    }
}


void IRProject::createProject()
{
    createWorkspaceList();
}


// call listener object to fire signal to IRMAIN
void IRProject::createNewProject()
{
    callCreateNewProjectAction();
}


// call listener object to fire signal to IRMAIN
void IRProject::saveProject()
{
    callSaveProjectAction();
}


void IRProject::saveAsProject()
{
    callSaveAsProjectAction();
}


// call listener object to fire signal to IRMAIN
void IRProject::openProject()
{
    callOpenProjectAction();
}


void IRProject::closeProject()
{
    this->menu_action_status = MenuActionStatus::CloseProjectAction;
    sendChangeMessage();
    callCloseProjectAction();
}


void IRProject::createNewWorkspace()
{
    std::cout << "createNewWorkspace() : " << std::endl;
    std::string title = this->projectName + "_" + std::to_string(this->workspaces.size()+1);
    
    Rectangle<int> frameSize(this->workspaceListWidth, 0,
                             getWidth()-this->workspaceListWidth, getHeight());
    IRWorkSpace* space = new IRWorkSpace(title, frameSize, this->preferenceWindow);
    
    space->requestWorkspaceListUpdate = [this] { updateWorkspaceList(); };
    space->requestNewProject = [this] { callCreateNewProjectAction(); };
    space->requestSaveProject = [this] { callSaveProjectAction(); };
    space->requestSaveAsProject = [this] { callSaveAsProjectAction(); };
    space->requestOpenProject = [this] { callOpenProjectAction(); };
    space->requestCloseProject = [this] { callCloseProjectAction(); };
    space->requestOpenFileInspecter = [this] { openFileInspecterWindow(); };
    space->requestOpenPreferenceWindow = [this] { openPreferenceWindow(); };

    space->notifyEditModeChanged = [this] { notifyEditModeChange(); };
    space->notifyLinkModeChanged = [this] { notifyLinkModeChange(); };

    space->notifyNodeObjectModification = [this](IRNodeObject* obj) { receiveNodeObjectModification(obj); };
    
    space->addChangeListener(this->listener);
    this->mixer.addAudioSource(&space->getMixer());
    
    // IRFileManager
    space->setIRFileManager(getFileManager());
    
    addAndMakeVisible(space);
    this->workspaces.push_back(space);
    
    std::cout << "new workspace created " << space << std::endl;
    if(this->workspaceList != nullptr)
    {
        // if there is already a topSpace, then treat heavy components
        if(this->topSpace != nullptr)
        {
            // hide heavey components on the previous topSpace.
            this->topSpace->manageHeavyWeightComponents(false);
        }
        
        this->topSpace = space;
        
        // bring the new workspace on top.
        this->topSpace->toFront(true);
        this->topSpace->manageHeavyWeightComponents(true);
        space->toFront(true);
        repaint();
        // show heavey components on the current topSpace
        
        this->workspaceList->addWorkspace(space);
        this->workspaceList->setSelectedComponentIndex(this->workspaceList->getWorkspaceCount()-1);
        
        
        std::cout << "top space=  " << this->topSpace << std::endl;
    }
    else
    {
        std::cout << "Error : createNewWorkspace() : could not add new workspace, workspaceList null" << std::endl;
    }
}


void IRProject::createWorkspaceList()
{
    this->workspaceList = std::make_shared<IRWorkspaceList>(Rectangle<int>(0, 0, this->workspaceListWidth, getHeight()));
    this->workspaceList->addListener(this);
    this->workspaceList->addChangeListener(this);
    
    // IRFileManager
    this->workspaceList->setIRFileManager(getFileManager());
    addAndMakeVisible(this->workspaceList.get());
}


// IRWorkspaceList Listener callback
void IRProject::removeWorkspace(IRWorkSpace* workspace)
{
    auto it = std::find(this->workspaces.begin(), this->workspaces.end(), workspace);
    if (it != this->workspaces.end())
    {
        this->workspaces.erase(it);
    }
}


// update workspace list. warning. this may be a little bit expensive task
void IRProject::updateWorkspaceList()
{
    this->workspaceList->updateList();
}


void IRProject::performEditModeChange()
{
    // change the status
    this->EditModeFlag = !this->topSpace->isEditMode();

    //apply the editMode status to all workspaces
    for(auto space : this->workspaces)
    {
        space->setEditMode(this->EditModeFlag);
    }
    
    // notify IRProjectWindow change of the edit mode
    if(this->notifyEditModeChanged != nullptr)
    {
        this->notifyEditModeChanged();
    }
}

void IRProject::performLinkModeChange()
{
    std::cout << "performLinkModeChange\n";
    // change the status
    this->LinkModeFlag = !this->topSpace->isLinkMode();
    
    //apply the editMode status to all workspaces
    for(auto space : this->workspaces)
    {
        space->setLinkMode(this->LinkModeFlag);
    }
    
    // notify IRProjectWindow change of the edit mode
    if(this->notifyLinkModeChanged != nullptr)
    {
        this->notifyLinkModeChanged();
    }
}

void IRProject::notifyEditModeChange()
{
    // DO NOT change the status
    this->EditModeFlag = this->topSpace->isEditMode();
    
    //apply the editMode status to all workspaces
    for(auto space : this->workspaces)
    {
        if(space != this->topSpace)
            space->setEditMode(this->EditModeFlag);
    }
    
    // notify IRProjectWindow change of the edit mode
    if(this->notifyEditModeChanged != nullptr)
    {
        this->notifyEditModeChanged();
    }
}

void IRProject::notifyLinkModeChange()
{
    
}

void IRProject::receiveNodeObjectModification(IRNodeObject* obj)
{
    updateFileInspecterWindow();
}


IRWorkSpace* IRProject::getTopWorkspace()
{
    return this->topSpace;
}


json11::Json IRProject::saveAction(std::string filePath)
{
    
    this->projectPath = filePath;

    // the project path is not yet given.
    if (filePath.size() == 0)
    {
        std::cout << "projectPath 0" << std::endl;
        return false;
    }
    
    // if no contents in this project
    if (this->workspaces.size() == 0)
    {
        std::cout << "workspace size 0" << std::endl;
        return false;
    }
    
    // try to make project directories
    if(! this->saveLoadClass.createProjectDirectory(filePath))
    {
        std::cout << "could not make project directory." << std::endl;
        return false;
    }
    
    Rectangle<int> b = getBounds();
    
    t_json header = json11::Json::object({
        {"Project", json11::Json::object({
            {"projectName",     this->projectName},
            {"author",          "Keitaro Takahashi"},
            {"date",            "20.04.1986"},
            {"osType",          "macOS"},
            {"osVersion",       "10.13.6"},
            {"IRVersion",       "0.0.1"},
            {"bounds",          json11::Json::array({b.getX(), b.getY(), b.getWidth(), b.getHeight()})},
        })},
    });
    

    
    this->saveLoadClass.setHeader(header);
    
    
    std::vector<json11::Json> saveBuffer;
    
    std::map<std::string, std::string> buf;
    
    json11::Json::object buffer;
    
    int index = 1;
    
    
    for(auto workspace : this->workspaces)
    {
        
        json11::Json object = json11::Json::object({
            {"workspace-" + std::to_string(index), workspace->makeSaveDataOfThis()}
        });
        
        buffer["workspace-" + std::to_string(index)] = workspace->makeSaveDataOfThis();
        
        saveBuffer.push_back(object);
        
        buf["workspace-"+std::to_string(index)] = workspace->makeSaveDataOfThis().dump();
        
        
        
        index++;
    }
    
    json11::Json::object jo(buffer.begin(), buffer.end());
    
    saveData = jo;
    
    std::cout << "========== save data ========== \n" << std::endl;
    std::cout << saveData.dump() << std::endl;
    std::cout << "========== end ========== \n" << std::endl;
    
    this->saveLoadClass.createWorkspaces(saveData);
    
    auto sb = KLib().StringSplit(filePath, '/');
    std::string filename = sb[sb.size()-1] + ".irimas"; // same to the project name
    filename = filePath + "/" + filename;
    std::cout << "save file name = " << filename << std::endl;
    this->saveLoadClass.writeSaveData(filename);
    
    return saveData;
}


void IRProject::callCreateNewProjectAction()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.createNewProjectAction(); });
    //check again
    if(checker.shouldBailOut()) return;
    //std::function
    if(this->createNewProjectActionCompleted != nullptr) this->createNewProjectActionCompleted();
}


void IRProject::callOpenProjectAction()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.openProjectAction(); });
    //check again
    if(checker.shouldBailOut()) return;
    //std::function
    if(this->openProjectActionCompleted != nullptr) this->openProjectActionCompleted();
}


void IRProject::callCloseProjectAction()
{
    
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.closeProjectAction(this->parentWindow); });
    //check again
    if(checker.shouldBailOut()) return;
    //std::function
    if(this->closeProjectActionCompleted != nullptr) this->closeProjectActionCompleted();
}


void IRProject::callSaveProjectAction()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.saveProjectAction(this); });
    //check again
    if(checker.shouldBailOut()) return;
    //std::function
    if(this->saveProjectActionCompleted != nullptr) this->saveProjectActionCompleted();
}


void IRProject::callSaveAsProjectAction()
{
    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.saveAsProjectAction(this); });
    //check again
    if(checker.shouldBailOut()) return;
    //std::function
    if(this->saveAsProjectActionCompleted != nullptr) this->saveAsProjectActionCompleted();
}


// menu bar methods
StringArray IRProject::getMenuBarNames()
{
    return { "File", "Edit" , "Window", "Help" };
}


PopupMenu IRProject::getMenuForIndex(int menuIndex, const String& menuName)
{
    PopupMenu menu;
    
    if(menuIndex == 0)
    {
        menu.addCommandItem(&commandManager, CommandIDs::NewProject);
        menu.addCommandItem(&commandManager, CommandIDs::OpenProject);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, CommandIDs::CloseProject);
        menu.addCommandItem(&commandManager, CommandIDs::SaveProject);
        menu.addCommandItem(&commandManager, CommandIDs::RenameProject);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, CommandIDs::NewWorkspace);
        
    }else if(menuIndex == 1)
    {
        menu.addCommandItem(&commandManager, CommandIDs::EditMode);
        menu.addCommandItem(&commandManager, CommandIDs::LinkMode);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, CommandIDs::Undo);
        menu.addCommandItem(&commandManager, CommandIDs::Redo);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, CommandIDs::Cut);
        menu.addCommandItem(&commandManager, CommandIDs::Copy);
        menu.addCommandItem(&commandManager, CommandIDs::Paste);
        menu.addCommandItem(&commandManager, CommandIDs::Duplicate);
        
    }else if(menuIndex == 2)
    {
        menu.addCommandItem(&commandManager, CommandIDs::menuPreferenceWindow);
        menu.addCommandItem(&commandManager, CommandIDs::fileInspecterWindow);

        
    }else if(menuIndex == 3)
    {
        
    }
    
    return menu;
}


void IRProject::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
    
}


IRProject::MenuActionStatus IRProject::getMenuActionStatus() const
{
    return this->menu_action_status;
}


IRWorkspaceList* IRProject::getWorkspaceList()
{
    return this->workspaceList.get();
}


void IRProject::setProjectPath(std::string path)
{
    this->projectPath = path;
}


std::string IRProject::getProjectPath() const
{
    return this->projectPath;
}


void IRProject::setProjectName(std::string name)
{
    this->projectName = name;
}


std::string IRProject::getProjectName() const
{
    return this->projectName;
}


// get parent ProjectWindow
DocumentWindow* IRProject::getParentWindow()
{
    return this->parentWindow;
}


// initialize projects. This method is called after loading action to set up workspaces
void IRProject::initProjectAfterLoading()
{
    int index = 0;
    for (auto space : this->workspaces)
    {
        space->setEditMode(false);
        
        std::cout << "workspace[" << index << "] = " << space << std::endl;
        index++;
    }
    
    this->topSpace = this->workspaces[0];
    this->topSpace->toFront(true);
    getWorkspaceList()->setSelectedComponentIndex(0);
    
    // dont update here, it may cause crash
    //getWorkspaceList()->updateList();
}


bool IRProject::isEditMode() const
{
    return this->EditModeFlag;
}

bool IRProject::isLinkMode() const
{
    return this->LinkModeFlag;
}

bool IRProject::isNonSavedChange() const
{
    return this->nonSavedChanges;
}

void IRProject::setNonSavedChange(bool newStatus)
{
    this->nonSavedChanges = newStatus;
}

// ==================================================
// windows

void IRProject::openFileInspecterWindow()
{
    if(this->fileInspecterWindow == nullptr)
    {
        this->fileInspecterWindow = new IRFileInspecterWindow("File Inspecter");
        this->fileInspecterWindow->setFileManager(getFileManager());
        this->fileInspecterWindow->updateInspecter();
    }
    else
    {
        this->fileInspecterWindow->updateInspecter();
    }
    this->fileInspecterWindow->show();
}

void IRProject::openPreferenceWindow()
{
    if(this->preferenceWindow == nullptr)
    {
        this->preferenceWindow = new PreferenceWindow("");
    }
    
    this->preferenceWindow->setVisible(true);
    this->preferenceWindow->toFront(true);
}


void IRProject::updateFileInspecterWindow()
{
    if(this->fileInspecterWindow != nullptr)
        this->fileInspecterWindow->updateInspecter();
}
// ==================================================




// **** **** PRIVATE METHODS **** **** //

void IRProject::changeListenerCallback(ChangeBroadcaster* source)
{
    if(source == this->workspaceList.get())
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


void IRProject::setMenuBarPosition ( MenuBarPosition newPosition )
{
    if(menuBarPosition != newPosition)
    {
        menuBarPosition = newPosition;
        if(menuBarPosition != MenuBarPosition::burger)
            sidePanel.showOrHide(false);
        
#if JUCE_MAC
        MenuBarModel::setMacMainMenu (menuBarPosition == MenuBarPosition::global ? this : nullptr);
#endif
        menuBar->setVisible   (menuBarPosition == MenuBarPosition::window);
        menuItemsChanged();
        resized();
    }
}


ApplicationCommandTarget* IRProject::getNextCommandTarget()
{
    return editCommandTarget.get();
}


void IRProject::getAllCommands(Array<CommandID>&c)
{
    Array<CommandID> commands
    {
        CommandIDs::NewProject,
        CommandIDs::OpenProject,
        CommandIDs::CloseProject,
        CommandIDs::SaveProject,
        CommandIDs::RenameProject,
        CommandIDs::NewWorkspace,
        CommandIDs::fileInspecterWindow,
        CommandIDs::menuPreferenceWindow
    };
    c.addArray (commands);
}


void IRProject::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
    switch (commandID)
    {
        case CommandIDs::NewProject:
            result.setInfo ("New Project", "Sets the outer colour to red", "File", 0);
            result.addDefaultKeypress ('n', ModifierKeys::shiftModifier);
            break;
        case CommandIDs::OpenProject:
            result.setInfo ("Open Project", "Sets the outer colour to red", "File", 0);
            result.addDefaultKeypress ('o', ModifierKeys::commandModifier);
            break;
        case CommandIDs::CloseProject:
            result.setInfo ("Close Project", "Sets the outer colour to red", "File", 0);
            result.addDefaultKeypress ('w', ModifierKeys::commandModifier);
            break;
        case CommandIDs::SaveProject:
            result.setInfo ("Save Project", "Sets the outer colour to red", "File", 0);
            result.addDefaultKeypress ('s', ModifierKeys::commandModifier);
            break;
        case CommandIDs::RenameProject:
            result.setInfo ("Rename Project", "Sets the outer colour to red", "File", 0);
            result.addDefaultKeypress ('r', ModifierKeys::commandModifier);
            result.setActive(true);
            break;
            
        case CommandIDs::NewWorkspace:
            result.setInfo ("New Workspace", "Sets the outer colour to red", "File", 0);
            result.addDefaultKeypress ('n', ModifierKeys::commandModifier);
            break;
            
        case CommandIDs::fileInspecterWindow:
            result.setInfo ("fileInspecter Window", "Sets the outer colour to red", "Window", 0);
            result.addDefaultKeypress ('8', ModifierKeys::commandModifier);
            break;
            
        case CommandIDs::menuPreferenceWindow:
            result.setInfo ("Preference Window", "Sets the outer colour to red", "Window", 0);
            result.addDefaultKeypress ('9', ModifierKeys::commandModifier);
            break;
            
        default:
            break;
    }
}


bool IRProject::perform(const InvocationInfo& info)
{
    switch(info.commandID)
    {
        case CommandIDs::NewProject:
            createNewProject();
            break;
        case CommandIDs::OpenProject:
            openProject();
            break;
        case CommandIDs::CloseProject:
            closeProject();
            break;
        case CommandIDs::SaveProject:
            saveProject();
            break;
        case CommandIDs::RenameProject:
            saveAsProject();
            break;
        case CommandIDs::NewWorkspace:
            createNewWorkspace();
            break;
            
        case CommandIDs::fileInspecterWindow:
            std::cout << "window menu perform in IRProject" << std::endl;
            openFileInspecterWindow();
            break;
        case CommandIDs::menuPreferenceWindow:
            this->preferenceWindow->setVisible(true);
            this->preferenceWindow->toFront(true);
            break;
        default:
            return false;
    }
    repaint();
    return true;
}


// AudioAppComponent
void IRProject::audioSetup()
{
    setAudioChannels(0, 2);
}


void IRProject::closeAudioSetup()
{
    shutdownAudio();
}


void IRProject::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    this->mixer.getAudioSource().prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void IRProject::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    this->mixer.getAudioSource().getNextAudioBlock(bufferToFill);
}


void IRProject::releaseResources()
{
    this->mixer.getAudioSource().releaseResources();
}
// --------------------------------------------------
