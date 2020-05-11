//
//  IRVAWaveformObject.h
//  IRiMaS
//
//  Created by Keitaro on 24/04/2020.
//

#ifndef IRVAWaveformObject_h
#define IRVAWaveformObject_h

#include "IRWaveformObject.hpp"

class IRVAWaveformObject : public IRWaveformObject
{
public:
    IRVAWaveformObject(Component* parent, IRStr* str) :
    IRWaveformObject(parent, str)
    {
        
      
        
        setResizingSquareColour(Colours::grey);

    }
    
    ~IRVAWaveformObject()
    {
        
    }
    
private:
    
};

#endif /* IRVAWaveformObject_h */
