//
//  ObjectArranger.hpp
//  IRiMaS
//
//  Created by Keitaro on 04/02/2020.
//

#ifndef ObjectArranger_hpp
#define ObjectArranger_hpp

#include "JuceHeader.h"
#include "IRStrComponent.hpp"

class ArrangeController : public Component,
public IRStrComponent,
public ChangeBroadcaster,
private Label::Listener,
private Button::Listener
{
public:
    
    enum ArrangeControllerStatus{
        NONE,
        INPUTWIDTH,
        INPUTHEIGHT,
        INPUTX,
        INPUTY,
        
        FRONTBUTTON,
        BACKBUTTON
    };
    
    
    ArrangeController(IRStr* str);
    ~ArrangeController();
    // =======================================================
    
    void resized() override;
    
    void paint(Graphics& g) override;
    // =======================================================
    // =======================================================
    ArrangeControllerStatus getStatus() const { return this->status; }
    // =======================================================
    
    int getLabelWidth();
    int getLabelHeight();
    int getLabelX();
    int getLabelY();
    
    // =======================================================

private:
    // =======================================================
    Label labelSize;
    Label labelWidth;
    Label labelHeight;
    
    Label InputWidth;
    Label InputHeight;
    
    Label labelPosition;
    Label labelX;
    Label labelY;
    
    Label InputX;
    Label InputY;
    
    void createLabel(Label& label, String text);
    void createSmallLabel(Label& label, String text);
    // =======================================================

    TextButton frontButton;
    TextButton backButton;

    void createButton(TextButton& button, String text);
    void buttonClicked(Button* button) override;
    // =======================================================

    void labelTextChanged (Label* labelThatHasChanged) override;
    void LabelChangeListener(Label* label);
    // =======================================================
    ArrangeControllerStatus status = NONE;
    // =======================================================

};

#endif /* ObjectArranger_hpp */
