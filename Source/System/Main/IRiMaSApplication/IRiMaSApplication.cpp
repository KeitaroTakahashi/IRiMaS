
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


void IRiMaSApplication::initialise(const String& commandLine)
{
    this->mainWindow.reset(new IRiMaSMainComponent(getApplicationName()));
    this->mainWindow->initialize();
}


void IRiMaSApplication::shutdown()
{
    this->mainWindow = nullptr;
}


void IRiMaSApplication::systemRequestedQuit()
{
    quit();
}


void IRiMaSApplication::anotherInstanceStarted(const String& commandLine)
{
    
}




