//
//  LeftBarSlideMenu.hpp
//  IRiMaS
//
//  Created by Keitaro on 17/09/2019.
//

#ifndef LeftBarSlideMenu_hpp
#define LeftBarSlideMenu_hpp

#include "JuceHeader.h"
#include "IRViewPort.hpp"
#include "SlideMenu.hpp"
#include "IRStrComponent.hpp"

class LeftBarSlideMenu : public IRViewPort,
public IRStrComponent,
public ChangeBroadcaster
{
public:
    LeftBarSlideMenu(IRStr* str);
    ~LeftBarSlideMenu();
    //==================================================

    
    void resized() override;
    //==================================================

    //==================================================

    //==================================================

    //==================================================
private:
    
    void visibleAreaChangedAction(const Rectangle<int> &newVisibleArea) override;
    //==================================================

    class Component4ViewPort : public Component
    {
    public:
        Component4ViewPort(Component* main) : main(main)
        {
            addAndMakeVisible(main);
        }
        
        ~Component4ViewPort() {}
        
        void resized() override
        {
            this->main->setBounds(0, 0, getWidth(), getHeight());
        }
        
        
    private:
        Component* main;
        
    };
    
    //==================================================

    std::shared_ptr<Component4ViewPort> viewPort;
    
    std::shared_ptr<SlideMenu> slideMenu;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LeftBarSlideMenu)

};

#endif /* LeftBarSlideMenu_hpp */
