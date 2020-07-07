//
//  IROnVideoPlayerControllerWindow.h
//  IRiMaS
//
//  Created by Keitaro on 06/07/2020.
//

#ifndef IROnVideoPlayerControllerWindow_h
#define IROnVideoPlayerControllerWindow_h


#include "IROnVideoPlayerController.hpp"

class IROnVideoPlayerControllerWindow : public DocumentWindow,
public IRStrComponent,
public ChangeBroadcaster
{
public:
    
    IROnVideoPlayerControllerWindow(IRStr* str, String name, Rectangle<int>frameRect) :
    DocumentWindow(name, Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId), DocumentWindow::allButtons),
    IRStrComponent(str)

    {
        setOpaque(false);
        
        setUsingNativeTitleBar(false);
        setResizable(false, false);
        setTitleBarHeight(0);
        setBounds(frameRect);
        
        

        
        setVisible(true);
        
        this->component.reset( new IROnVideoPlayerController(str) );
        this->component->setBounds(frameRect);
        setContentOwned(this->component.get(), true);
      
    }
    
    ~IROnVideoPlayerControllerWindow()
    {
        this->component.reset();
    }
    
    std::function<void()> closeMenuWindow;
    void closeAction()
    {
        if(this->closeMenuWindow != nullptr)
            this->closeMenuWindow();
    }
    
    void setChangeListener(ChangeListener* listener)
    {
        this->component->addChangeListener(listener);
    }
    
    IROnVideoPlayerController* getPlayerController() { return this->component.get(); }
    
private:
    std::shared_ptr<IROnVideoPlayerController> component;
};
#endif /* IROnVideoPlayerControllerWindow_h */
