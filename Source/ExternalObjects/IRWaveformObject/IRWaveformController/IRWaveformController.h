//
//  IRWaveformController.h
//  IRiMaS
//
//  Created by Keitaro on 23/07/2019.
//

#ifndef IRWaveformController_h
#define IRWaveformController_h

#include "IRSimpleController.hpp"

class IRWaveformController : public IRSimpleController
{
public:
    IRWaveformController() : IRSimpleController()
    {
        /*
        setup(true,
              true,
              false,
              true,
              false);
         */
    }
    ~IRWaveformController()
    {
        
    }
    
  
private:
    
};
#endif /* IRWaveformController_h */
