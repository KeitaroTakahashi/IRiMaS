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
    IRTestUI(IRNodeObject* parent) : IRUIFoundation(parent)
    {
        
    }
    
    ~IRTestUI()
    {
        
    }
    
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::blue);
    }
    // --------------------------------------------------
    void setEditMode(bool flag) override
    {
        this->editModeFlag = flag;

        
        // if not editMode, get keyboardFocus
        if(! flag){
            setWantsKeyboardFocus(true);
            addKeyListener(this);
            
        }
        else
        {
            setWantsKeyboardFocus(false);
            removeKeyListener(this);
        }
    }
    // --------------------------------------------------

private:
    bool editModeFlag = true;

};

#endif /* IRTestUI_h */
