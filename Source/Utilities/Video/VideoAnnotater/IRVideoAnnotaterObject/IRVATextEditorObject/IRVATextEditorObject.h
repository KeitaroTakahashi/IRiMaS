//
//  IRVATextEditorObject.h
//  IRiMaS
//
//  Created by Keitaro on 01/03/2020.
//

#ifndef IRVATextEditorObject_h
#define IRVATextEditorObject_h

#include "IRTextEditorObject.hpp"

class IRVATextEditorObject : public IRTextEditorObject
{
public:
    IRVATextEditorObject(Component* parent, IRStr* str) :
    IRTextEditorObject(parent, str)
    {
        
        Colour backColour = getStr()->SYSTEMCOLOUR.fundamental.withAlpha(0.7f);
        setBackgroundColour(backColour);
        Colour textColour = Colours::white.withAlpha(1.0f);
        applyColourToAllText(textColour);
        
        setResizingSquareColour(Colours::grey);

    }
    
    ~IRVATextEditorObject()
    {
        
    }
    
private:
    
};

#endif /* IRVATextEditorObject_h */
