//
//  IRStrComponent.hpp
//  IRiMaS
//
//  Created by Keitaro on 29/08/2019.
//

#ifndef IRStrComponent_hpp
#define IRStrComponent_hpp

#include "IRStr.h"
#include "LayoutStuff.h"

class IRStrComponent : public LayoutStuff
{
 
public:
    IRStrComponent(IRStr* str);
    ~IRStrComponent();
    // ==================================================

    IRStr* getStr() const { return this->ir_str; }
    void setStr(IRStr* str) { this->ir_str = str; }
    KeyListener* getMasterKey() const { return this->masterKey; }
    
    
    // ==================================================

    void setUIColour(IR::colorPattern pattern)
    {
        getStr()->SYSTEMCOLOUR = pattern;
        getStr()->projectOwner->resized();
        getStr()->projectOwner->repaint();
        getStr()->projectOwner->updateAppearance();

        getStr()->projectOwner->rebindOpenGLContents();
    }
    
    // ==================================================
    
    

 // ==================================================

private:
    IRStr* ir_str;
    KeyListener* masterKey;
    
};

#endif /* IRStrComponent_hpp */
