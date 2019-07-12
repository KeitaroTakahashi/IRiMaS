
#include "IRiMaSMainComponent.hpp"

#include "ObjectFactoryInitializer.hpp"




IRiMaSMainComponent::IRiMaSMainComponent(const String applicationName)
{
    this->applicationName = applicationName;
    
    ObjectFactoryInitializer(); // this will create a singleton and initialise the object palette
    
    this->initialise();
}


IRiMaSMainComponent::~IRiMaSMainComponent()
{

    for (auto win : this->projectLib)
    {
        delete win;
    }
    
    this->projectLib.clear();
    
    singleton<IRObjectFactory>::explicitlyDestroy(); // OK - THAT IS THE THING TO DO. THIS ULTIMATELY DESTROYS THE T_OBJECTs AND AVOIDS THE LEAK OF THE ENBEDDED IMAGES.
    
    // finalize singlton
    SingletonFinalizer::finalize();
    
    std::cout << "end call destructor IRiMaSMainComponent" << std::endl;
}


void IRiMaSMainComponent::initialise()
{
    this->preferenceWindow = std::make_shared<PreferenceWindow>(applicationName);
    
    this->startWindow.reset(new IRStartWindow(applicationName, Rectangle<int>(640, 480)));
    this->startWindow->addChangeListener(this);
}


void IRiMaSMainComponent::createNewProject()
{
    std::cout << "Creating new project... projectWindow" << std::endl;
    IRProjectWindow* projectWindow = new IRProjectWindow("Untitled", this->preferenceWindow.get());
    // std::shared_ptr<IRProjectWindow> projectWindow = std::make_shared<IRProjectWindow>("Untitled", this->preferenceWindow.get());
    
    // create a Workspace as default
    projectWindow->getProjectComponent()->createNewWorkspace(); // DISABLED NEW WORKSPACE FOR NOW
    //add Listener to receive signal to open close and save projects
    projectWindow->getProjectComponent()->addListener(this);
    this->projectLib.push_back(projectWindow);
    
    // bring the new window at top of all other windows.
    projectWindow->toFront(true);
    
    //hide startWindow when a project window opens.
    // startWindow only appears when no project window stays opened.
    this->startWindow->setVisible(false);
    this->preferenceWindow->setVisible(true);
}


void IRiMaSMainComponent::createNewProjectFromSaveData(std::string path)
{
    std::cout << "========== createNewProjectFromSaveData ==========" << std::endl;
    IRSaveLoadSystem::dataStr data = this->saveLoadClass.getSaveDataStr();
    IRSaveLoadSystem::headerStr header = data.header;
    
    t_json saveData = this->saveLoadClass.getSaveData();
    
    IRProjectWindow* projectWindow = new IRProjectWindow(applicationName, this->preferenceWindow.get());
    
    
    Rectangle<int>bounds = header.bounds;
    
    projectWindow->setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
    
    projectWindow->getProjectComponent()->addListener(this);
    // remove file name
    File f(path);
    std::string directoryPath = f.getParentDirectory().getFullPathName().toStdString();
    std::cout << "create new project from save data  "<< directoryPath << std::endl;
    projectWindow->getProjectComponent()->setProjectPath(directoryPath);
    projectWindow->setWindowTitle(header.projectName);
    
    this->projectLib.push_back(projectWindow);
    
    // bring the new window in front of all other windows.
    projectWindow->toFront(true);
    
    //remove startWindow when a project window opens.
    this->startWindow->setVisible(false);
    
    std::cout << "========== loadWorkspaces ==========" << std::endl;
    
    for (auto it = saveData["Workspaces"].object_items().cbegin(); it != saveData["Workspaces"].object_items().cend(); ++it)
    {
        std::string id = static_cast<std::string>(it->first);
        std::cout << id << std::endl;
        
        // ===== create worksapce =====
        projectWindow->getProjectComponent()->createNewWorkspace();
        
        // get created workspace
        IRWorkSpace* currentSpace = projectWindow->getProjectComponent()->getTopWorkspace();
        
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
        
        
        for (int i = 0; i < objectArray.size(); i++) // for each item of the array...
        {
            for (auto it = objectArray[i].object_items().cbegin(); it != objectArray[i].object_items().cend(); ++it)
            {
                
                std::cout << " ===== " << it->first << " ===== " << std::endl;
                std::cout << "object type= " << it->second["objectType"].string_value() << std::endl;
                std::cout << "object uniqueID= " << it->second["objectUniqueID"].string_value() << std::endl;
                std::cout << "object status= " << it->second["status"].string_value() << std::endl;
                
                
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
    projectWindow->getProjectComponent()->initProjectAfterLoading();
    // set project save path
    projectWindow->getProjectComponent()->setProjectPath(directoryPath);
    
}


void IRiMaSMainComponent::openProject()
{
    std::cout << "Opening a project..." << std::endl;
    
    FileChooser chooser("Select a project file...",
                        {},
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
        std::cout << "Could not open any files." << std::endl;
    }
}

void IRiMaSMainComponent::closeProject(DocumentWindow* closingWindow)
{
    auto it = std::find(this->projectLib.begin(), this->projectLib.end(), closingWindow);
    if (it != this->projectLib.end())
    {
        this->projectLib.erase(it);
    }
    else
    {
        std::cout << "IRiMaSMainComponent : closeProject : Could not find window of " << closingWindow << std::endl;
    }
    
    
    if (this->projectLib.size() == 0)
    {
        this->startWindow->setVisible(true);
    }
}


// from IRProject Listener
void IRiMaSMainComponent::createNewProjectAction()
{
    createNewProject();
}


void IRiMaSMainComponent::openProjectAction()
{
    openProject();
}


void IRiMaSMainComponent::closeProjectAction(DocumentWindow* closingWindow)
{
    std::cout << "IRiMaSMainComponent closeProjectAction: " << closingWindow << std::endl;

    closeProject(closingWindow);
}


void IRiMaSMainComponent::saveProjectAction(IRProject* project)
{
    //if saveDataPath is not yet set.
    std::cout << "save project Action path = " << project->getProjectPath() << std::endl;
    if (project->getProjectPath().size() == 0)
    {
        
        // create new save data file with an extension of ".irimas by default
        FileChooser chooser("Save project...",
                            {},
                            "");
        
        if (chooser.browseForFileToSave(true))
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
            std::cout << "Could not open any files." << std::endl;
        }
    }
    else
    {
        this->saveData = project->saveAction(project->getProjectPath());
    }
}


void IRiMaSMainComponent::saveAsProjectAction(IRProject* project)
{
    FileChooser chooser("Save project...",
                        {},
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
        std::cout << "Could not open any files." << std::endl;
    }
}


// *** PRIVATE METHODS


void IRiMaSMainComponent::changeListenerCallback (ChangeBroadcaster* source)
{
    std::cout << "changeListener" << std::endl;
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
    else if (dynamic_cast<IRProjectWindow*>(source) != nullptr)
    {
        std::cout << "source == IRProjectWindow object" << std::endl;
    }
}




