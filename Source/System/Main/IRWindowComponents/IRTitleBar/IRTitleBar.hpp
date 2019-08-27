//
//  IRTitleBar.hpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#ifndef IRTitleBar_hpp
#define IRTitleBar_hpp


#include "../JuceLibraryCode/JuceHeader.h"
#include "IRwindowHeader.h"

class IRTitleBar : public Component,
public ChangeBroadcaster
{
public:
    //==================================================
    IRTitleBar();
    ~IRTitleBar();
    
    //==================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseDown(const MouseEvent& e)override;
    void mouseMove(const MouseEvent& e)override;
    
    //==================================================
    void checkResizableFromMouseDownPosition(Point<int> pos);
    
    //==================================================
    
    IRWindowBarActionStatus getStatus() const { return this->status; }
    
    Point<int> pos;
    Point<int> currentPos;
    Point<int> prevPos;
    
private:
    //==================================================
    // Your private member variables go here...
    
    IRWindowBarActionStatus status;
    bool isResizable = true;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRTitleBar)
};
#endif /* IRTitleBar_hpp */
