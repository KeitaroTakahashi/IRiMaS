
#include "IRiMaSMain.hpp"





IRMAIN::IRMAIN(const String applicationName)
{
    
    this->applicationName = applicationName;
    
    // initialize object factory of IRObjectFactory
    // IRObjectFactory is created as a singleton manner
    init_factory f;
    
}


IRMAIN::~IRMAIN()
{
    delete this->preferenceWindow;
    for(auto win : this->projectLib)
    {
        delete win;
    }
    
    this->projectLib.clear();
    
}


void IRMAIN::initialize()
{
    // create window for preference
    this->preferenceWindow = new PreferenceWindow (applicationName);
    // create initial project window
    //this->projectLib.push_back(new IRProjectWindow(applicationName,this->preferenceWindow));
    
    this->startWindow.reset(new IRStartWindow(applicationName, Rectangle<int>(640,480)));
    this->startWindow->addChangeListener(this);
    
    
}


void IRMAIN::createNewProject()
{
    printf("Creating new project... projectWindow\n");
    IRProjectWindow* project = new IRProjectWindow("Untitled",this->preferenceWindow);
    
    // create a Workspace as default
    project->getProjectComponent()->createNewWorkspace();
    //add Listener to receive signal to open close and save projects
    project->getProjectComponent()->addListener(this);
    this->projectLib.push_back(project);
    
    // bring the new window at top of all other windows.
    project->toFront(true);
    
    //hide startWindow when a project window opens.
    // startWindow only appears when no project window stays opened.
    this->startWindow->setVisible(false);
    
}


void IRMAIN::createNewProjectFromSaveData(std::string path)
{
    printf("========== createNewProjectFromSaveData ==========\n");
    IRSaveLoadSystem::dataStr data = this->saveLoadClass.getSaveDataStr();
    IRSaveLoadSystem::headerStr header = data.header;
    
    t_json saveData = this->saveLoadClass.getSaveData();
    
    IRProjectWindow* project = new IRProjectWindow(applicationName,this->preferenceWindow);
    
    
    Rectangle<int>bounds = header.bounds;
    
    project->setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
    
    project->getProjectComponent()->addListener(this);
    project->getProjectComponent()->setProjectPath(path);
    project->setWindowTitle(header.projectName);
    
    this->projectLib.push_back(project);
    
    // bring the new window in front of all other windows.
    project->toFront(true);
    
    //remove startWindow when a project window opens.
    this->startWindow->setVisible(false);
    
    printf("========== loadWorkspaces ==========\n");
    
    for(auto it = saveData["Workspaces"].object_items().cbegin(); it != saveData["Workspaces"].object_items().cend(); ++it)
    {
        std::string id = static_cast<std::string>(it->first);
        std::cout << id << std::endl;
        
        // ===== create worksapce =====
        project->getProjectComponent()->createNewWorkspace();
        
        // get created workspace
        IRWorkSpace* currentSpace = project->getProjectComponent()->getTopWorkspace();
        
        // retrieve save data of the workspace
        json11::Json data = saveData["Workspaces"][id].object_items();
        
        // retrieve save data of worksapce appearance
        json11::Json appearance = data["Appearance"];
        std::cout << "Appearance : backgroundColour = " << appearance["backgroundColour"].string_value() << std::endl;
        
        // retrieve save data of objects on the workspace
        json11::Json Objects = data["Objects"];
        // the object data is stored in array
        json11::Json::array objectArray = Objects.array_items();
        
        std::cout << "array count = " << objectArray.size() << std::endl;
        
        
        for(int i =0; i< objectArray.size(); i++) // for each item of the array...
        {
            for(auto it = objectArray[i].object_items().cbegin(); it != objectArray[i].object_items().cend(); ++it)
            {
                
                std::cout << " ===== "<< it->first << " ===== "<< std::endl;
                std::cout << "object type= "<< it->second["objectType"].string_value() << std::endl;
                std::cout << "object uniqueID= "<< it->second["objectUniqueID"].string_value() << std::endl;
                std::cout << "object status= "<< it->second["status"].string_value() << std::endl;
                
                
                // ===== create object =====
                std::string objectTypeId = it->second["objectType"].string_value();
                auto* obj = IRFactory.createObject(objectTypeId, currentSpace);
                
                json11::Json::array b = it->second["bounds"].array_items();
                obj->setBounds(b[0].int_value(), b[1].int_value(),
                               b[2].int_value(), b[3].int_value());
                
                std::cout << "object bounds = " << obj->getWidth() << ", " << obj->getHeight() << std::endl;
                
                obj->setUniqueID(it->second["objectUniqueID"].string_value());
                //
                
                // currently no status implemented!
                
                currentSpace->createObject(obj);
                
                // load save dada
                obj->loadThisFromSaveData(it->second["ObjectDefined"]);
                
                // ===== END =====
                /*
                 for(auto it2 = it->second["ObjectDefined"].object_items().cbegin(); it2 != it->second.object_items().cend(); ++it2)
                 {
                 std::cout << it2->first << std::endl;
                 
                 }*/
                
                //std::cout << it->first["ObjectType"].string_value() << std::endl;
                std::cout << objectArray[i][it->first]["ObjectType"].string_value() << std::endl;
                
                //currentSpace->createObject()
                
            }
            
        }
        
        //as default, a workspace is in control mode
        //currentSpace->setEditMode(false);
        
    }
    
    // initialize this Project
    project->getProjectComponent()->initProjectAfterLoading();
    // set project save path
    project->getProjectComponent()->setProjectPath(path);
    
}


void IRMAIN::openProject()
{
    printf("Opening a project...\n");
    
    FileChooser chooser("Select an audio file to play...",
                        File::nonexistent,
                        "*.irimas");
    
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        auto path = file.getFullPathName();
        path.swapWith(path);
        std::string p = path.toStdString();
        std::cout << "load file " << p << std::endl;
        this->saveLoadClass.readSaveData(p);
        createNewProjectFromSaveData(p);
        
    }
    else
    {
        printf("Could not open any files.\n");
    }
}

void IRMAIN::closeProject(DocumentWindow* closingWindow)
{
    auto it = std::find(this->projectLib.begin(), this->projectLib.end(), closingWindow);
    if (it != this->projectLib.end()) { this->projectLib.erase(it); }
    
    
    if(this->projectLib.size() == 0)
    {
        this->startWindow->setVisible(true);
    }
}


// from IRProject Listener
void IRMAIN::createNewProjectAction()
{
    createNewProject();
}


void IRMAIN::openProjectAction()
{
    openProject();
}


void IRMAIN::closeProjectAction(DocumentWindow* closingWindow)
{
    closeProject(closingWindow);
}


void IRMAIN::saveProjectAction(IRProject* project)
{
    //if saveDataPath is not yet set.
    std::cout << "save project Action path = "<< project->getProjectPath() << std::endl;
    if(project->getProjectPath().size() == 0){
        
        // create new save data file with an extension of ".irimas by default
        FileChooser chooser("Save project...",
                            File::nonexistent,
                            "");
        
        if(chooser.browseForFileToSave(true))
        {
            auto file = chooser.getResult();
            auto path = file.getFullPathName();
            auto filename = file.getFileName();
            std::cout << "file path = " << path << std::endl;
            this->saveDataPath = path.toStdString();
            project->setProjectName(filename.toStdString());
            this->saveData = project->saveAction(this->saveDataPath);
            IRProjectWindow* w = static_cast<IRProjectWindow*>(project->getParentWindow());
            w->setWindowTitle(filename);
            
        }
        else
        {
            printf("Could not open any files.\n");
        }
    }
    else
    {
        this->saveData = project->saveAction(project->getProjectPath());
    }
}


void IRMAIN::saveAsProjectAction(IRProject* project)
{
    FileChooser chooser("Save project...",
                        File::nonexistent,
                        "*.irimas");
    
    if (chooser.browseForFileToSave(true))
    {
        auto file = chooser.getResult();
        auto path = file.getFullPathName();
        std::cout << "file path = " << path << std::endl;
        this->saveDataPath = path.toStdString();
        this->saveData = project->saveAction(this->saveDataPath);
    }
    else
    {
        printf("Could not open any files.\n");
    }
}


// *** PRIVATE METHODS


void IRMAIN::changeListenerCallback (ChangeBroadcaster* source)
{
    printf("changeListener \n");
    if (this->startWindow.get() == source)
    {
        if (this->startWindow->getMenuActionStatus() == IRStarter::MenuActionStatus::CreateNewProjectAction)
        {
            createNewProject();
        }
        else if (this->startWindow->getMenuActionStatus() == IRStarter::MenuActionStatus::OpenProjectAction)
        {
            openProject();
        }
    }
    else if(dynamic_cast<IRProjectWindow*>(source) != nullptr)
    {
        printf("source == IRProjectWindow object\n");
    }
}




