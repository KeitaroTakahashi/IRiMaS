//
//  TimeCodeInput.h
//  IRiMaS
//
//  Created by Keitaro on 12/12/2019.
//

#ifndef TimeCodeInput_h
#define TimeCodeInput_h
#include <string>
#include "JuceHeader.h"
#include "IRStrComponent.hpp"

class TimeCodeInput : public Component,
public IRStrComponent,
Label::Listener
{
public:
    TimeCodeInput(IRStr *str) : IRStrComponent(str)
    {
        makeDigitLabel(this->hourLabelBegin);
        makeDigitLabel(this->minLabelBegin);
        makeDigitLabel(this->frameLabelBegin);
        
        makeDigitLabel(this->hourLabelEnd);
        makeDigitLabel(this->minLabelEnd);
        makeDigitLabel(this->frameLabelEnd);
    }
    
    ~TimeCodeInput()
    {
        
    }
    // ==================================================

    void paint(Graphics& g) override
    {
        g.setColour(getStr()->SYSTEMCOLOUR.text);
        g.drawText(":", this->hourLabelBegin.getX() + this->hourLabelBegin.getWidth() - 2,
                   4, 10, 30, Justification::centred);
        g.drawText(":", this->minLabelBegin.getX() + this->minLabelBegin.getWidth() - 2,
                   4, 10, 30, Justification::centred);
        g.drawText("-", this->frameLabelBegin.getX() + this->frameLabelBegin.getWidth(),
                   5, 10, 30, Justification::centred);
        
        g.drawText(":", this->hourLabelEnd.getX() + this->hourLabelEnd.getWidth() - 2,
                   4, 10, 30, Justification::centred);
        g.drawText(":", this->minLabelEnd.getX() + this->minLabelEnd.getWidth() - 2,
                   4, 10, 30, Justification::centred);

    }
    
    void resized() override
    {
        int margin = 2;
        int w = 30;
        int h = 30;
        int x = 2;
        this->hourLabelBegin.    setBounds(x, 5, 26, h);
        x += margin + w;
        this->minLabelBegin.     setBounds(x, 5, 26, h);
        x += margin + w;
        this->frameLabelBegin.   setBounds(x, 5, 26, h);
        
        x += 35;
        
        this->hourLabelEnd.    setBounds(x, 5, 26, h);
        x += margin + w;
        this->minLabelEnd.     setBounds(x, 5, 26, h);
        x += margin + w;
        this->frameLabelEnd.   setBounds(x, 5, 26, h);
        
    }
    // ==================================================
    
    void s2hms(int sec, int& h, int& m, int& s)
    {
        h = sec / 3600;
        int sec2 = sec - (h * 3600);
        m = sec2 / 60;
        int sec3 = sec2 - (m * 60);
        s = sec3;
    }
    void setMaxTimeCode(int sec)
    {
        int h,m,s;
        s2hms(sec, h, m, s);
        
        this->maxH = h;
        this->maxM = m;
        this->maxS = s;
        
        this->maxSec = sec;
        
    }
  
    // ==================================================
    // ==================================================

private:
    
    // ==================================================
    void labelTextChanged (Label* labelThatHasChanged) override
    {
        std::string val = labelThatHasChanged->getText().toStdString();
        
        // check if the input value is digit integer number and otherw
        try
        {
           //int digi = std::stoi(val);
            
            // fix if BeginLabels are greater than EndLabels
            //fixBeginAndEnd();
            // fix if Labels are greater than max values
            fixLabelWithMaxVal();
        }
        catch (std::invalid_argument const &e)
        {
           std::cout << "Bad input " << val << std::endl;
            labelThatHasChanged->setText("00", dontSendNotification);
        }
        

    }
    // ==================================================
    
    void makeDigitLabel(Label& label)
    {
        addAndMakeVisible(label);
        label.setText("00", dontSendNotification);
        label.setEditable(true);
        label.addListener(this);
    }
    
    void setLabelVal(int beginSec, int endSec)
    {
        int h,m,s;
        
        s2hms(beginSec, h, m, s);
        
        if(h < 10) this->hourLabelBegin.    setText("0" + String(h), dontSendNotification);
        if(m < 10) this->minLabelBegin.     setText("0" + String(m), dontSendNotification);
        if(s < 10) this->frameLabelBegin.   setText("0" + String(s), dontSendNotification);
        
        s2hms(endSec, h, m, s);

        if(h < 10) this->hourLabelEnd.    setText("0" + String(h), dontSendNotification);
        if(m < 10) this->minLabelEnd.     setText("0" + String(m), dontSendNotification);
        if(s < 10) this->frameLabelEnd.   setText("0" + String(s), dontSendNotification);

    }
    // ==================================================
    
    void fixDigi(Label& label, int maxVal)
    {
        std::string s = label.getText().toStdString();
        int digi = std::stoi(s);
        if(digi)
        {
            if(digi > maxVal)
            {
                digi = maxVal;
            }
        }
        
        String text = String(digi);
        if(digi < 10) label.setText("0" + text, dontSendNotification);
        else label.setText(text, dontSendNotification);

    }
    
    void fixBeginAndEnd()
    {
        int beginSec, endSec;
        if(isBeginLargerThanEnd(beginSec, endSec))
        {
            beginSec = endSec;
            setLabelVal(beginSec, endSec);
        }
    }
    
    bool isBeginLargerThanEnd(int& beginSec, int& endSec)
    {
        beginSec = std::stoi(this->hourLabelBegin.getText().toStdString()) * 3600 +
                   std::stoi(this->minLabelBegin.getText().toStdString()) * 60 +
                   std::stoi(this->frameLabelBegin.getText().toStdString());
        
        endSec = std::stoi(this->hourLabelEnd.getText().toStdString()) * 3600 +
                 std::stoi(this->minLabelEnd.getText().toStdString()) * 60 +
                 std::stoi(this->frameLabelEnd.getText().toStdString());
        
        if(beginSec > endSec) return true;
        else return false;
    }
    
    void fixLabelWithMaxVal()
    {
        fixDigi(this->hourLabelBegin, maxH);
        fixDigi(this->minLabelBegin, maxM);
        fixDigi(this->frameLabelBegin, maxS);
        
        fixDigi(this->hourLabelEnd, maxH);
        fixDigi(this->minLabelEnd, maxM);
        fixDigi(this->frameLabelEnd, maxS);
    }
    // ==================================================

    Label hourLabelBegin;
    Label minLabelBegin;
    Label frameLabelBegin;
    
    Label hourLabelEnd;
    Label minLabelEnd;
    Label frameLabelEnd;
    
    int maxH = 59;
    int maxM = 59;
    int maxS = 59;
    
    int maxSec = 0;
};
#endif /* TimeCodeInput_h */
