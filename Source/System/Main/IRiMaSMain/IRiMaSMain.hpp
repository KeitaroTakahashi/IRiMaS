
#ifndef IRiMaSMain_hpp
#define IRiMaSMain_hpp

#include "JuceHeader.h"

//#include "IRWorkSpace.hpp"
#include "ObjectFactory.h"
//#include "IRFoundation.h"
#include "IRProjectWindow.h"
#include "IRStarter.hpp"
#include "IRStartWindow.hpp"
//#include "Menus.h"
//#include "IRObjectCreater.hpp"

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
    
    IRMAIN(const String applicationName);
    ~IRMAIN();
    
    void initialize();
    
    void createNewProject();
    void createNewProjectFromSaveData(std::string path);
    
    void openProject();
    void closeProject(DocumentWindow* closingWindow);
    
    void createNewProjectAction() override;
    void openProjectAction() override;
    void closeProjectAction(DocumentWindow* closingWindow) override;
    void saveProjectAction(IRProject* project) override;
    void saveAsProjectAction(IRProject* project) override;
    
private:
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
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
