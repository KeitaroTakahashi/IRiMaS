//
//  IRObjectController.hpp
//  IRiMaS
//
//  Created by Keitaro on 02/10/2019.
//

#ifndef IRObjectController_hpp
#define IRObjectController_hpp

#include "JuceHeader.h"
#include "IRStrComponent.hpp"
#include "IRSwitchButton.h"


class IRObjectController : public Component,
public IRStrComponent,
private IRSquareButton::Listener
{
public:
    IRObjectController(IRStr* str);
    ~IRObjectController();
    
    virtual void resized() override;
    
    virtual void mainControllerSelected() {}
    virtual void arrangeControllerSelected() {}
private:
    
    IRSwitchButton switchButton;
    void buttonClicked(IRSquareButton* clickedButton) override;

   
};

#endif /* IRObjectController_hpp */
