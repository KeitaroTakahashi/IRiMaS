//
//  IRVideoAnnotationTextComponent.h
//  IRiMaS
//
//  Created by Keitaro on 19/01/2020.
//

#ifndef IRVideoAnnotationTextComponent_h
#define IRVideoAnnotationTextComponent_h

#include "IRVideoAnnotationComponent.h"
#include "AnnotationTextEventComponent.h"

class IRVideoAnnotationTextComponent : public IRVideoAnnotationComponent,
private IRNodeObject::Listener
{
public:
    IRVideoAnnotationTextComponent(IRStr* str, AnnotationTextEventComponent* event) :
    IRVideoAnnotationComponent(str, event),
    thisEvent(event)
    {
        // only Height is initially set as it is variable according to the text height
        setSize(100, 60);
    }
    
    ~IRVideoAnnotationTextComponent()
    {
        this->textEditor.reset();
    }
    // --------------------------------------------------

    void paint(Graphics& g) override
    {
        Colour backColour = getStr()->SYSTEMCOLOUR.fundamental.withAlpha(0.7f);
        g.fillAll(Colours::yellow);
    }
    
    void resized() override
    {
        
    }
    // --------------------------------------------------
    void setIRTextEditorObject(IRTextEditorObject* t)
    {
        if(this->textEditor.get() == nullptr)
        {
            this->textEditor.reset(new IRTextEditorObject(this, getStr()));
            addAndMakeVisible(this->textEditor.get());
            this->textEditor->addMouseListener(this,true);
            this->textEditor->addListener(this);
            // to enable editing text, it needs to be set editMode false
            this->textEditor->setEditMode(false);
        }
        
        //this->textEditor->setObjectBounds(t->getBounds());
        
        this->textEditor->copyData(t);
        
        std::cout << "setIRTextEditorObject : " << t->getWidth() << ", " << t->getHeight() << std::endl;
        
        resized();
        
        if(isSizeChanged())
        {
            std::cout << "change! : \n";
            if(this->sizeChanged != nullptr) this->sizeChanged();
        }
        
        // set and show the caret position
        this->textEditor->setCaretPosition(t->getCaretPosition());
        
        
        
        //setPreviousBounds(t->getWidth(), t->getHeight());
    }
    
    // --------------------------------------------------
    IRTextEditorObject* getIRTextEditorObject() { return this->textEditor.get(); }
    
    
private:
    
    AnnotationTextEventComponent* thisEvent = nullptr;
    
    std::shared_ptr<IRTextEditorObject> textEditor;
    // --------------------------------------------------
    
    
    void nodeObjectPositionChanged(IRNodeObject* obj) override
    {
        std::cout << "textEditor arrangeController size changed = " << obj->getWidth() << ",  " << obj->getHeight() <<std::endl;
        
        setBounds(obj->getLocalBounds());
        /*
        if(this->textEditor != nullptr){
            this->textEditor->setObjectBounds(getLocalBounds());
            std::cout <<"textEditor resized!\n";
        }*/
    }
    // --------------------------------------------------
    // --------------------------------------------------
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoAnnotationTextComponent)

    
};
#endif /* IRVideoAnnotationTextComponent_h */
