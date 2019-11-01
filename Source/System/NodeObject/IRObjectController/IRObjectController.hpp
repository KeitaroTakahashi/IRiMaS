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

class IRObjectController : public Component,
public IRStrComponent
{
public:
    IRObjectController(IRStr* str);
    ~IRObjectController();
    
    
private:
    
};

#endif /* IRObjectController_hpp */
