/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "JuceHeader.h"
#include "MainComponent.h"
#include "IRWorkSpace.hpp"
#include "IRFoundation.h"

#include "IRiMaSMain.hpp"

//==============================================================================
class NodeComponentObject_StudyApplication  : public JUCEApplication
{
public:
    //==============================================================================
    NodeComponentObject_StudyApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..
        
        //this->preferenceWindow = new PreferenceWindow (getApplicationName());
        
        this->mainWindow.reset(new IRMAIN(getApplicationName()));
        this->mainWindow->initialize();
        
        //this->projectLib.push_back(new IRProjectWindow(getApplicationName(),this->preferenceWindow));
        
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..
        
        delete this->preferenceWindow;
        this->mainWindow = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    //==============================================================================

    void createNewProject()
    {
        
    }
    
    
    //==============================================================================

    
private:
    std::unique_ptr<IRMAIN> mainWindow;

    PreferenceWindow* preferenceWindow;

};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (NodeComponentObject_StudyApplication)
