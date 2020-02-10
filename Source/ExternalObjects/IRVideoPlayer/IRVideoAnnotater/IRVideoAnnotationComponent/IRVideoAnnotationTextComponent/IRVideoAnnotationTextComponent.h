//
//  IRVideoAnnotationTextComponent.h
//  IRiMaS
//
//  Created by Keitaro on 19/01/2020.
//

#ifndef IRVideoAnnotationTextComponent_h
#define IRVideoAnnotationTextComponent_h

#include "IRVideoAnnotationComponent.hpp"
#include "AnnotationTextEventComponent.h"

class IRVideoAnnotationTextComponent : public IRVideoAnnotationComponent
{
public:
    IRVideoAnnotationTextComponent(IRStr* str, AnnotationTextEventComponent* event) :
    IRVideoAnnotationComponent(str, event),
    thisEvent(event)
    {

    }
    
    ~IRVideoAnnotationTextComponent()
    {
        if(this->textEditor != nullptr) delete this->textEditor;
        this->textEditor = nullptr;
    }
    // --------------------------------------------------

    void paint(Graphics& g) override
    {
        Colour backColour = Colours::darkgrey.withAlpha(0.7f);
        g.fillAll(backColour);
        
        //g.setColour(Colours::white);
        //Font f("Avenir Next",34, Font::plain);
        //g.setFont(f);
        //g.drawText(thisEvent->getTextContents(), 0,0,getWidth(), getHeight(), Justification::centred);
    }
    
    void resized() override
    {
        if(this->textEditor != nullptr)
            this->textEditor->setBounds(getLocalBounds());
    }
    // --------------------------------------------------
    void setIRTextEditorObject(IRTextEditorObject* t)
    {
        if(this->textEditor != nullptr) delete this->textEditor;
        this->textEditor = nullptr;
        
        
        t->showTextContents();
        
        this->textEditor = static_cast<IRTextEditorObject*>(t->copyThis());
        this->textEditor->setTextColour(Colours::white);
        addAndMakeVisible(this->textEditor);
        
        this->textEditor->showTextContents();
        
        setSize(t->getWidth(), t->getHeight());
        
        std::cout << "setIRTextEditorObject : " << t->getWidth() << ", " << t->getHeight() << std::endl;
        
        resized();
    }
    
    // --------------------------------------------------

    
private:
    
    AnnotationTextEventComponent* thisEvent = nullptr;
    
    IRTextEditorObject* textEditor = nullptr;
    // --------------------------------------------------
    // --------------------------------------------------
    // --------------------------------------------------
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoAnnotationTextComponent)

    
};
#endif /* IRVideoAnnotationTextComponent_h */
