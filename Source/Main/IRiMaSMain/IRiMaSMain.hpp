//
//  IRiMaSMain.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 15/01/2019.
//

#ifndef IRiMaSMain_hpp
#define IRiMaSMain_hpp

#include "JuceHeader.h"
#include "IRWorkSpace.hpp"
#include "ObjectFactory.h"
#include "IRFoundation.h"
#include "IRProjectWindow.h"
#include "IRStarter.h"
#include "Menus.h"
#include "IRObjectCreater.hpp"

/*
 IRMAIN
 |                  |
 IRStartWindow      IRProjectWindow
 |                  |
 IRStarter          IRProject
                    |
                    IRWorkspace

IRMAIN class operates
    creating new project
    opening a saved project

 */



class IRMAIN : public Component,
public ChangeListener,
public IRProject::Listener
{
public:
    
    IRMAIN(const String applicationName)
    {
        
        this->applicationName = applicationName;
        
        // initialize object factory of IRObjectFactory
        // IRObjectFactory is created as a singleton manner
        init_factory f;
        


    }
    
    ~IRMAIN()
    {
        delete this->preferenceWindow;
        for(auto win : this->projectLib)
        {
            delete win;
        }
        
        this->projectLib.clear();
        
    }
    
    void initialize()
    {
        // create window for preference
        this->preferenceWindow = new PreferenceWindow (applicationName);
        // create initial project window
        //this->projectLib.push_back(new IRProjectWindow(applicationName,this->preferenceWindow));
        
        this->startWindow.reset(new IRStartWindow(applicationName, Rectangle<int>(640,480)));
        this->startWindow->addChangeListener(this);
        
        
    }
    // ============================================================
    
    void createNewProject()
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
    void createNewProjectFromSaveData(std::string path);
    
    void openProject()
    {
        printf("Opening a project...\n");
        
        FileChooser chooser("Select an audio file to play...",
                            File::nonexistent,
                            "*.irimas");
        
        if(chooser.browseForFileToOpen())
        {
            auto file = chooser.getResult();
            auto path = file.getFullPathName();
            path.swapWith(path);
            std::string p = path.toStdString();
            std::cout << "load file " << p << std::endl;
            this->saveLoadClass.readSaveData(p);
            createNewProjectFromSaveData(p);
            
        }else
        {
            printf("Could not open any files.\n");
        }
    }
    
    void closeProject(DocumentWindow* closingWindow)
    {
        auto it = std::find(this->projectLib.begin(), this->projectLib.end(), closingWindow);
        if (it != this->projectLib.end()) { this->projectLib.erase(it); }
        
        
        if(this->projectLib.size() == 0)
        {
            this->startWindow->setVisible(true);
        }
    }
    
    // ============================================================
    
    // from IRProject Listener
    void createNewProjectAction() override
    {
        createNewProject();
    }
    void openProjectAction() override
    {
        openProject();
    }
    void closeProjectAction(DocumentWindow* closingWindow) override
    {
        closeProject(closingWindow);
    }
    void saveProjectAction(IRProject* project) override
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
                
            }else{
                printf("Could not open any files.\n");
            }
        }else{
            this->saveData = project->saveAction(project->getProjectPath());
            
        }
    }
    
    void saveAsProjectAction(IRProject* project) override
    {
        FileChooser chooser("Save project...",
                            File::nonexistent,
                            "*.irimas");
        
        if(chooser.browseForFileToSave(true))
        {
            auto file = chooser.getResult();
            auto path = file.getFullPathName();
            std::cout << "file path = " << path << std::endl;
            this->saveDataPath = path.toStdString();
            this->saveData = project->saveAction(this->saveDataPath);
        }else{
            printf("Could not open any files.\n");
        }
    }
    
    // ============================================================
    
private:
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        printf("changeListener \n");
        if(this->startWindow.get() == source)
        {
            if(this->startWindow->getMenuActionStatus() == IRStarter::MenuActionStatus::CreateNewProjectAction)
            {
                createNewProject();
            }else if(this->startWindow->getMenuActionStatus() == IRStarter::MenuActionStatus::OpenProjectAction)
            {
                openProject();
            }
            
        }else if(dynamic_cast<IRProjectWindow*>(source) != nullptr)
        {
            printf("source == IRProjectWindow object\n");
        }
        
    }
    
    // application name
    String applicationName;
    std::string saveDataPath = "";
    
    // storing all project windows
    std::vector<IRProjectWindow* >projectLib;
    
    // storing a currently active project window
    IRProjectWindow* activeProjectWindow;
    
    // preference window
    PreferenceWindow* preferenceWindow;
    
    // start window initially opened when launching this app
    std::unique_ptr<IRStartWindow> startWindow;
    
    // for save and load projects
    IRSaveLoadSystem saveLoadClass;
    json11::Json saveData;
    
    // IRObjectFactory
    IRObjectFactory& IRFactory = singleton<IRObjectFactory>::get_instance();
    
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    
};


#endif /* IRiMaSMain_hpp */
