//
//  ObjectArranger.cpp
//  IRiMaS
//
//  Created by Keitaro on 04/02/2020.
//

#include "ObjectArranger.hpp"


ArrangeController::ArrangeController(IRStr* str) : IRStrComponent(str)
{
    createLabel(this->labelSize, "Size : ");
    createSmallLabel(this->labelWidth, "Width");
    createSmallLabel(this->labelHeight, "Height");
    
    createLabel(this->InputWidth, "0");
    this->InputWidth.setEditable(true);
    createLabel(this->InputHeight, "0");
    this->InputHeight.setEditable(true);

    createLabel(this->labelPosition, "Position : ");
    createSmallLabel(this->labelX, "X");
    createSmallLabel(this->labelY, "Y");

    createLabel(this->InputX, "0");
    this->InputX.setEditable(true);
    createLabel(this->InputY, "0");
    this->InputY.setEditable(true);
    
    createButton(this->frontButton, "Front");
    createButton(this->backButton, "Back");

    
    setSize (250, 400);

}

ArrangeController::~ArrangeController()
{

}
// =======================================================

void ArrangeController::resized()
{
    int margin = 5;
    int ySmall = 30;
    int yIncrement = 40;
    int y = margin;
    int x = margin;
    this->labelSize.setBounds(x, y, 60, 30);
    this->InputWidth.setBounds(x + 80, y, 60, 30);
    this->InputHeight.setBounds(x + 150, y, 60, 30);
    y += ySmall;
    this->labelWidth.setBounds(x + 80, y, 60, 20);
    this->labelHeight.setBounds(x + 150, y, 60, 20);

    y+= ySmall;
    this->labelPosition.setBounds(margin, y,100, 30);
    this->InputX.setBounds(x + 80, y, 60, 30);
    this->InputY.setBounds(x + 150, y, 60, 30);
    y += ySmall;
    this->labelX.setBounds(x + 80, y, 60, 20);
    this->labelY.setBounds(x + 150, y, 60, 20);
    y += ySmall;
    this->frontButton.setBounds(x + 20, y, 100, 20);
    this->backButton.setBounds(x + 130, y, 100, 20);
}

void ArrangeController::paint(Graphics& g)
{
    g.setColour(Colours::black);
    g.drawRect(this->InputWidth.getBounds());
    g.drawRect(this->InputHeight.getBounds());
    g.drawRect(this->InputX.getBounds());
    g.drawRect(this->InputY.getBounds());

}
// =======================================================

void ArrangeController::createLabel(Label& label, String text)
{
    Font f("Avenir Next",20, Font::plain);
    label.setFont(f);
    addAndMakeVisible(label);
    label.setText(text, dontSendNotification);
    label.setJustificationType(Justification::left);
}
// =======================================================

void ArrangeController::createSmallLabel(Label& label, String text)
{
    Font f("Avenir Next",16, Font::plain);
    label.setFont(f);
    addAndMakeVisible(label);
    label.setText(text, dontSendNotification);
    label.setJustificationType(Justification::right);
}
// =======================================================


void ArrangeController::labelTextChanged (Label* labelThatHasChanged)
{
    std::string val = labelThatHasChanged->getText().toStdString();
    
    std::cout << "label changed = " << val << std::endl;
    
    // check if the input value is digit integer number and otherw
    try
    {
        LabelChangeListener(labelThatHasChanged);
    }
    catch (const std::invalid_argument &e)
    {
       std::cout << "Bad input " << val << std::endl;
        labelThatHasChanged->setText("0", dontSendNotification);
    }
}


void ArrangeController::LabelChangeListener(Label* label)
{
    if(label == &this->InputWidth)
    {
        this->status = INPUTWIDTH;
        sendChangeMessage();
    }else if(label == &this->InputHeight)
    {
        this->status = INPUTHEIGHT;
        sendChangeMessage();
    }else if(label == &this->InputX)
    {
        this->status = INPUTX;
        sendChangeMessage();
        
    }else if(label == &this->InputY)
    {
        this->status = INPUTY;
        sendChangeMessage();
    }
}


// =======================================================
void ArrangeController::createButton(TextButton& button, String text)
{
    button.setButtonText(text);
    addAndMakeVisible(button);
    button.addListener(this);
}

void ArrangeController::buttonClicked(Button* button)
{
    if(button == &this->frontButton)
    {
        this->status = FRONTBUTTON;
        sendChangeMessage();
    }else if(button == &this->backButton)
    {
        this->status = BACKBUTTON;
        sendChangeMessage();
    }
}

// =======================================================
int ArrangeController::getLabelWidth()
{
    return std::stoi(this->labelWidth.getText().toStdString());
}
int ArrangeController::getLabelHeight()
{
    return std::stoi(this->labelHeight.getText().toStdString());
}
int ArrangeController::getLabelX()
{
    return std::stoi(this->labelX.getText().toStdString());
}
int ArrangeController::getLabelY()
{
    return std::stoi(this->labelY.getText().toStdString());
}
// =======================================================
// =======================================================
