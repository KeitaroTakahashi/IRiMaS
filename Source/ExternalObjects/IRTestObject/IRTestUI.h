//
//  IRTestUI.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 06/03/2019.
//

#ifndef IRTestUI_h
#define IRTestUI_h

#include "IRUIFoundation.hpp"

class IRTestUI : public IRUIFoundation
{
public:
    IRTestUI()
    {
        
    }
    
    ~IRTestUI()
    {
        
    }
    
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::blue);
    }
    
private:
    
};

#endif /* IRTestUI_h */
