//
//  IRVAShapeObject.h
//  IRiMaS
//
//  Created by Keitaro on 03/03/2020.
//

#ifndef IRVAShapeObject_h
#define IRVAShapeObject_h

#include "IRShapeObject.hpp"


class IRVAShapeObject : public IRShapeObject
{
public:
    IRVAShapeObject(Component* parent, IRStr* str) :
    IRShapeObject(parent, str)
    {
        setResizingSquareColour(Colours::grey);
    }
    
    ~IRVAShapeObject()
    {
        
    }
    
private:
    
};

#endif /* IRVAShapeObject_h */
