//
//  IRVAImageObject.h
//  IRiMaS
//
//  Created by Keitaro on 24/04/2020.
//

#ifndef IRVAImageObject_h
#define IRVAImageObject_h
#include "IRImageViewerObject.hpp"

class IRVAImageViewerObject : public IRImageViewerObject
{
public:
    IRVAImageViewerObject(Component* parent, IRStr* str) :
    IRImageViewerObject(parent, str)
    {
        
        setResizingSquareColour(Colours::grey);

    }
    
    ~IRVAImageViewerObject()
    {
        
    }
    
private:
    
};

#endif /* IRVAImageObject_h */
