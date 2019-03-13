
#ifndef IRStartWindow_hpp
#define IRStartWindow_hpp

#include "JuceHeader.h"

#include "IRStarter.hpp"




class IRStartWindow : public DocumentWindow,
                      public ChangeBroadcaster,
                      public ChangeListener
{
    
public:
    
    IRStartWindow(String name, Rectangle<int> frameRect);
    ~IRStartWindow();
    
    MenuActionStatus getMenuActionStatus() const;
    void closeButtonPressed() override;
    
    
private:
    
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
    IRStarter* starter;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRStartWindow)
};





#endif /* IRStartWindow_hpp */




