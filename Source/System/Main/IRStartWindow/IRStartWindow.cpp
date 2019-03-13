
#include "IRStartWindow.hpp"



IRStartWindow::IRStartWindow(String name, Rectangle<int> frameRect) :
    DocumentWindow(name, Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),
        DocumentWindow::allButtons)
{
    // do not show a title bar
    setUsingNativeTitleBar(false);
    setTitleBarHeight(0);
    
    // setup system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    SYSTEMCOLOUR.set(IR::darkBrownSet());
    
    Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    int x = r.getWidth();
    int y = r.getHeight();
    setBounds(0, 0, frameRect.getWidth(), frameRect.getHeight());
    setCentrePosition(x / 2, y / 2);
    setResizable(false, false);
    //setName(this->workspace->getTitle());
    
    setVisible(true);
    
    this->starter = new IRStarter(frameRect);
    this->starter->addChangeListener(this);
    
    setContentOwned(this->starter, true);
}


IRStartWindow::~IRStartWindow()
{
    delete starter;
}


MenuActionStatus IRStartWindow::getMenuActionStatus() const
{
    return this->starter->getMenuActionStatus();
}


void IRStartWindow::closeButtonPressed()
{
    
}





// **** **** PRIVATE METHODS **** **** //


void IRStartWindow::changeListenerCallback (ChangeBroadcaster* source)
{
    if (this->starter == source)
    {
        MenuActionStatus status = this->starter->getMenuActionStatus();
        if (status == MenuActionStatus::CreateNewProjectAction)
        {
            sendChangeMessage();
        }
        else if (status == MenuActionStatus::OpenProjectAction)
        {
            sendChangeMessage();
        }
    }
}




