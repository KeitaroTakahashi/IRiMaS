//
//  IRShapeUI.hpp
//  IRiMaS
//
//  Created by Keitaro on 26/07/2019.
//

#ifndef IRShapeUI_hpp
#define IRShapeUI_hpp

#include "IRFoundation.h"
#include "IRUIFoundation.hpp"
#include "IRShapePreference.h"

class IRShapeUI : public IRUIFoundation,
public ChangeBroadcaster,
public ChangeListener
{
public:
    
    enum IRShapeStatus
    {
        SQUARE,
        CIRCLE,
        TRIANGLE,
        RIGHTTRIANGLE
        
    };
    IRShapeUI(IRNodeObject* parent);
    ~IRShapeUI();
    //========================================================
    
    void paint (Graphics&) override;
    void resized() override;
    
    //========================================================
    void setStatus(IRShapeStatus status)
    {
        this->status = status;
        preference->getUI()->setShape((int)status + 1);
    }
    IRShapeStatus getStatus() const { return this->status; }
    
    void setColour(Colour colour){ this->colour = colour; }
    Colour getColour() const { return this->colour; }
    
    void setLineWidth(float w) { this->lineWidth = w; }
    float getLineWidth() const { return this->lineWidth; }
    
    void setFill(bool flag)
    {
        this->isFill = flag;
        preference->getUI()->setFillShape(flag);
    }
    bool getFill()const { return this->isFill; }
    
    IRShapePreference* getPreference() { return this->preference.get(); }
private:
    
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
    std::shared_ptr<IRShapePreference> preference;

    void drawSquare(Graphics& g);
    void drawBorderSquare(Graphics& g);
    void drawCircle(Graphics& g);
    void drawTriangle(Graphics& g);

    bool isFill = false;
    //========================================================

    Colour colour;
    IRShapeStatus status = SQUARE;
    
    float lineWidth = 4.0;
    
};
#endif /* IRShapeUI_hpp */
