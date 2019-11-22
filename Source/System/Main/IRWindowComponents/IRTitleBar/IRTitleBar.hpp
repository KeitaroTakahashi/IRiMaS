//
//  IRTitleBar.hpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#ifndef IRTitleBar_hpp
#define IRTitleBar_hpp

#include "IRTitleBarComponent.hpp"
#include "IRwindowHeader.h"
#include "IRHeavyWeightComponent.h"

class IRTitleBar : public Component,
public IRStrComponent,
public ChangeBroadcaster,
public IRHeavyWeightComponent,
public ChangeListener
{
public:
    //==================================================
    IRTitleBar(IRStr* str, String title);
    ~IRTitleBar();
    
    //==================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseDown(const MouseEvent& e)override;
    void mouseMove(const MouseEvent& e)override;
    void mouseDoubleClick(const MouseEvent& e) override;
    //==================================================

    std::function<void()> titleDoubleClickedCallback;
    
    //==================================================
    void checkResizableFromMouseDownPosition(Point<int> pos);
    
    void bringToFrontCompleted() override
    {
        this->comp.bringAllButtonsToFront();
    }
    
    void componentCreated() override
    {
        std::cout << "componentCreated\n";
        //this->comp.bringAllButtonsToFront();

    }
    //==================================================
    
    IRWindowBarActionStatus getStatus() const { return this->status; }
    
    Point<int> pos;
    Point<int> currentPos;
    Point<int> prevPos;
    
    //==================================================
    // acceccible from outside of this class
    IRTitleBarComponent comp;
    IRTitleBarProjectButtonArea* getProjectButtonComponent() { return comp.getProjectButtonComponent(); }

    //==================================================

private:
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    //==================================================
    
    //==================================================

    IRWindowBarActionStatus status;
    bool isResizable = true;
public:
    

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRTitleBar)
};
#endif /* IRTitleBar_hpp */
