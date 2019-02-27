
#include "IRiMaSApplication.hpp"





IRiMaSApplication::IRiMaSApplication()
{
    
}


const String IRiMaSApplication::getApplicationName()
{
    return ProjectInfo::projectName;
}


const String IRiMaSApplication::getApplicationVersion()
{
    return ProjectInfo::versionString;
}


bool IRiMaSApplication::moreThanOneInstanceAllowed()
{
    return true;
}


void IRiMaSApplication::initialise (const String& commandLine)
{
    //this->preferenceWindow = new PreferenceWindow (getApplicationName());
    
    this->mainWindow.reset(new IRMAIN(getApplicationName()));
    this->mainWindow->initialize();
    
    //this->projectLib.push_back(new IRProjectWindow(getApplicationName(),this->preferenceWindow));
    
}


void IRiMaSApplication::shutdown()
{
    delete this->preferenceWindow;
    this->mainWindow = nullptr;
}


void IRiMaSApplication::systemRequestedQuit()
{
    quit();
}

void IRiMaSApplication::anotherInstanceStarted (const String& commandLine)
{
    
}


/*
void createNewProject()
{
    
}
*/



