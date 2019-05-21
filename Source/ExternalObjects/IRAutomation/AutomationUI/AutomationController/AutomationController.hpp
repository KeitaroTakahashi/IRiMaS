//
//  AutomationController.hpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 02/05/2019.
//

#ifndef AutomationController_hpp
#define AutomationController_hpp

#include "JuceHeader.h"

#include "IRIconBank.hpp"
#include "IRImageButton.hpp"
#include "IRImageMovableButton.hpp"
#include "AutomationVertex.hpp"

class AutomationController : public Component
{
public:
    
   
    AutomationController();
    ~AutomationController();
    
    void resized() override;
    void paint(Graphics& g) override;
    
    void setMovableStatus(IRAutomation::movableStatus status);
    
    
    void setZoomInEvent(std::function<void()> callBack);
    void setZoomOutEvent(std::function<void()> callBack);
    
    
    void setMovableEvent(std::function<void(IRAutomation::movableStatus)> callBack);
    
    // ==================================================

    // ==================================================

private:
    
    // ==================================================

    IRIconBank& ImageBank = singleton<IRIconBank>::get_instance();

    IRImageButton zoomInButton;
    IRImageButton zoomOutButton;
    
    IRImageMovableButton movableButton;

    // ==================================================

    
    void zoomInClicked();
    void zoomOutClicked();
    
    
    int buttonSize = 40;
    int buttonInterval = 5;
};

#endif /* AutomationController_hpp */
