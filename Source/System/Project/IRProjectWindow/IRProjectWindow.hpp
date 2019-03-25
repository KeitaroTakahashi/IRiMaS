
#ifndef IRProjectWindow_h
#define IRProjectWindow_h

#include "IRProject.hpp"





class IRProjectWindow : public DocumentWindow,
                        public ChangeListener
{
    
public:
    
    IRProjectWindow(String name, PreferenceWindow *preferenceWindow);
    ~IRProjectWindow();
    
    void setWindowTitle(String name);
    
    void closeWindow();
    
    void eidtModeChangedAction();
    
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
    void closeButtonPressed() override;
    
    /* Note: Be careful if you override any DocumentWindow methods - the base
     class uses a lot of them, so by overriding you might break its functionality.
     It's best to do all your work in your content component instead, but if
     you really have to override any DocumentWindow methods, make sure your
     subclass also calls the superclass's method.
     */
    
    IRProject* getProjectComponent();
    
private:
    
    PreferenceWindow *preferenceWindow;
    IRProject *project;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRProjectWindow)
    
};

#endif /* IRProjectWindow_h */



