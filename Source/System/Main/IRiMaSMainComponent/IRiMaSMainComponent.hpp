
#ifndef IRiMaSMainComponent_hpp
#define IRiMaSMainComponent_hpp

#include "JuceHeader.h"

#include "json11.hpp"

#include "IRProject.hpp"
#include "IRProjectWindow.hpp"
#include "PreferenceWindow.hpp"
#include "IRStartWindow.hpp"
#include "IRSaveLoadSystem.hpp"
#include "IRObjectFactory.hpp"
#include "ObjectFactory.h" // sic





/*
 IRMAIN
 |                  |
 IRStartWindow      IRProjectWindow
 |                  |
 IRStarter          IRProject
                    |
                    IRWorkspace

IRiMaSMainComponent - formerly IRMAIN class operates
    creating new project
    opening a saved project

 */


class IRiMaSMainComponent : public Component,
                            public ChangeListener,
                            public IRProject::Listener
{
    
public:
    
    IRiMaSMainComponent(const String applicationName);
    ~IRiMaSMainComponent();
    
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
    // std::vector<std::unique_ptr<IRProjectWindow>> projectLib;
    
    // storing a currently active project window
    IRProjectWindow* activeProjectWindow;
    
    // preference window
    // PreferenceWindow* preferenceWindow;
    std::unique_ptr<PreferenceWindow> preferenceWindow;
    
    // start window initially opened when launching this app
    std::unique_ptr<IRStartWindow> startWindow;
    
    // for save and load projects
    IRSaveLoadSystem saveLoadClass;
    json11::Json saveData;
    
    // IRObjectFactory
    IRObjectFactory& IRFactory = singleton<IRObjectFactory>::get_instance();
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRiMaSMainComponent)
};


#endif /* IRiMaSMainComponent_hpp */



