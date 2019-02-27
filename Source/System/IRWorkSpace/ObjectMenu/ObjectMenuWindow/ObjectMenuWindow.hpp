
#ifndef ObjectMenuWindow_hpp
#define ObjectMenuWindow_hpp

#include "JuceHeader.h"

#include "ObjectListMenu.hpp"

/*
#include "ColourLib.h"
#include "MenusListBox.h"
#include "IRObjectCreater.hpp"
*/





class ObjectMenuWindow : public DocumentWindow
{
public:
    ObjectMenuWindow(String name, Rectangle<int> frameRect, ObjectListMenu* menu) :
    DocumentWindow (name,
                    Desktop::getInstance().getDefaultLookAndFeel()
                    .findColour (ResizableWindow::backgroundColourId),
                    DocumentWindow::allButtons)
    {
        // do not show a title bar
        setUsingNativeTitleBar (false);
        setTitleBarHeight(0);
        setBounds(frameRect);
        setVisible(true);
        // do not deallocate menu object here.
        setContentNonOwned(menu,true);
        
    }
    ~ObjectMenuWindow()
    {
        
    }
    
private:
    
    // setup systme colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObjectMenuWindow)
};




#endif /* ObjectMenuWindow_hpp */
