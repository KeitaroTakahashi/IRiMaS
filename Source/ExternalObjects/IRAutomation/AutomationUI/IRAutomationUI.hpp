//
//  IRAutomation.hpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 02/05/2019.
//

#ifndef IRAutomation_hpp
#define IRAutomation_hpp

#include "JuceHeader.h"

#include "IRUIFoundation.hpp"
#include "InteractiveAutomation.hpp"
#include "AutomationController.hpp"


class IRAutomationUI : public IRUIFoundation
{
public:
    IRAutomationUI(IRNodeObject* nodeObject);
    ~IRAutomationUI();
    // ==================================================
    void resized() override;
    // ==================================================
    
    void demoData(int num);
    
private:
    // ==================================================
    
    Viewport automationView;
    
    // ==================================================
    void zoomInClicked();
    void zoomOutClicked();
    void movableClicked(IRAutomation::movableStatus status);
    
    // setup
    bool isVerticalMovable = true;
    bool isHorizontalMovable = true;
    void verticalMovableClicked();
    void horizontalMovableClicked();
    
    // all setter
    void setMovable(bool movable, bool verticalMovable, bool horizontalMovable);
    
    // ==================================================
    
    
    std::shared_ptr<InteractiveAutomation> automation;
    
    float automation_width_ratio = 1.0;
    
    int xMargin = 10;
    int yMargin = 10;
    
    int controllerWidth = 40;
};

#endif /* IRAutomation_hpp */
