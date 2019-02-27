//
//  AutomationPreferenceUI.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 23/10/2018.
//

#ifndef AutomationPreferenceUI_h
#define AutomationPreferenceUI_h

class AutomationPreferenceUI : public Component
{
public:
    AutomationPreferenceUI(String title)
    {
        addAndMakeVisible(this->LabelTitle);
        this->LabelTitle.setText(title, dontSendNotification);
        this->LabelTitle.setColour(Label::textColourId, Colours::black);
        this->LabelTitle.setJustificationType(Justification::left);

    }
    
    ~AutomationPreferenceUI()
    {
        
    }
    
    //===============================================================
    
    void resized() override
    {
        // margin 10
        this->LabelTitle.setBounds          (20, 10, 300, 30);
        
    }
    
    // --------------------------------------------------------------
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::white);
        g.setColour(Colours::black);
        g.drawLine(0,42.5,getWidth(),42.5);
    }
    // --------------------------------------------------------------
    
private:
    
    Label LabelTitle;
};



#endif /* AutomationPreferenceUI_h */
