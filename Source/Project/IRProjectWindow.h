//
//  IRProjectWindow.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 15/01/2019.
//

#ifndef IRProjectWindow_h
#define IRProjectWindow_h

#include "IRProject.hpp"

class IRProjectWindow : public DocumentWindow, public ChangeListener
{
public:
    IRProjectWindow(String name,
                    PreferenceWindow *preferenceWindow)  :
    DocumentWindow (name,
                    Desktop::getInstance().getDefaultLookAndFeel()
                    .findColour (ResizableWindow::backgroundColourId),
                    DocumentWindow::allButtons)
    {
        
        // show a title bar
        setUsingNativeTitleBar (true);
        
        // setup systme colour
        IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
        SYSTEMCOLOUR.set(IR::darkBrownSet());
        
        setBounds(0, 0, getWidth(), getHeight());
        
        // create empty project initially.
        
        int h = 720;
        int w = h * sqrt(2);
        this->project = new IRProject("Untitled",
                                      Rectangle<int>(0,0,w,h),
                                      preferenceWindow,
                                      this,
                                      this);
        this->project->addChangeListener(this);
        this->project->notifyEditModeChanged = [this]{eidtModeChangedAction(); };
        
        setContentOwned(this->project, true);
        setResizable(true, true);
        
        setVisible (true);
        
        //create project
        this->project->createProject();
        
        
    }
    
    ~IRProjectWindow()
    {
        
    }
    // ==================================================
    void setWindowTitle(String name)
    {
        // check edit mode
        this->project->setProjectName(name.toStdString());
        if(this->project->isEditMode())
        {
            setName(name + " : EDIT MODE");
        }else
        {
            setName(name + " : CONTROL MODE");
        }
    }
    // ==================================================
    void closeWindow()
    {
        delete this->project;
        removeFromDesktop();
    }
    
    void eidtModeChangedAction()
    {
        std::cout << "eidtModeChangedAction\n";
        // check edit mode
        std::string name = this->project->getProjectName();
        if(this->project->isEditMode())
        {
            setName(name + " : EDIT MODE");
        }else
        {
            setName(name + " : CONTROL MODE");
        }
    }
    
    // ==================================================
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        if(source == this->project)
        {
            if(this->project->getMenuActionStatus() == IRProject::MenuActionStatus::CloseProjectAction)
            {
                closeWindow();
            }
        }
    }
    
    void closeButtonPressed() override
    {
        // This is called when the user tries to close this window. Here, we'll just
        // ask the app to quit when this happens, but you can change this to do
        // whatever you need.
        //JUCEApplication::getInstance()->systemRequestedQuit();
    }
    
    /* Note: Be careful if you override any DocumentWindow methods - the base
     class uses a lot of them, so by overriding you might break its functionality.
     It's best to do all your work in your content component instead, but if
     you really have to override any DocumentWindow methods, make sure your
     subclass also calls the superclass's method.
     */
    
    IRProject* getProjectComponent() { return this->project; }
    // ==================================================
    
private:
    PreferenceWindow *preferenceWindow;
    IRProject *project;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRProjectWindow)
};

#endif /* IRProjectWindow_h */
