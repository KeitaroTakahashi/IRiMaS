
#include "PreferenceWindow.hpp"




PreferenceWindow::PreferenceWindow(String name) : DocumentWindow(name,
                                               Desktop::getInstance().getDefaultLookAndFeel()
                                               .findColour (ResizableWindow::backgroundColourId),
                                               DocumentWindow::allButtons)
{
    
    setUsingNativeTitleBar (true);
    
    this->preferenceSpace = new IRPreferenceSpace("IRiMaS : Object Preference",
                                                  Rectangle<int>(0,0,400,720));
    
    this->preferenceSpace->addChangeListener(this);
    setContentOwned(this->preferenceSpace, true);
    
    setResizable(true,true);
    
    setName(this->preferenceSpace->getTitle());
    
    Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    //int x = r.getX();
    int y = r.getY();
    int w = r.getWidth();
    // int h = r.getHeight();
    
    
    setBounds(w - getWidth(),y,getWidth(),getHeight());
    
    setVisible(true);
    
}


PreferenceWindow::~PreferenceWindow()
{
    delete preferenceSpace;
    
}


void PreferenceWindow::changeListenerCallback(ChangeBroadcaster* source)
{
    if(source == this->preferenceSpace)
    {
        
    }
}


void PreferenceWindow::closeButtonPressed()
{
    // This is called when the user tries to close this window. Here, we'll just
    // ask the app to quit when this happens, but you can change this to do
    // whatever you need.
    JUCEApplication::getInstance()->systemRequestedQuit();
}


IRPreferenceSpace* PreferenceWindow::getPreferenceSpace()
{
    return this->preferenceSpace;
}
