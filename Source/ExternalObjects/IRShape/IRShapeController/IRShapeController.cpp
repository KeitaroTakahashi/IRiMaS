//
//  IRShapeController.cpp
//  IRiMaS
//
//  Created by Keitaro on 02/10/2019.
//

#include "IRShapeController.hpp"

IRShapeController::IRShapeController(IRStr* str) : IRObjectController("Shape", str),
comboBoxFont(str)
{

    addAndMakeVisible(&this->LabelTitle);
    this->LabelTitle.setText("Shape", dontSendNotification);
    this->LabelTitle.setFont(getStr()->SYSTEMCOLOUR.h3);
    this->LabelTitle.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    this->LabelTitle.setJustificationType(Justification::left);

    
    addAndMakeVisible(&this->shapeMenuLabel);
    this->shapeMenuLabel.setText("Shape", dontSendNotification);
    this->shapeMenuLabel.setFont(getStr()->SYSTEMCOLOUR.h4);
    this->shapeMenuLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    this->shapeMenuLabel.setJustificationType(Justification::left);
    
    addAndMakeVisible(&this->shapeMenu);
    this->shapeMenu.setColour(ComboBox::focusedOutlineColourId, getStr()->SYSTEMCOLOUR.contents);
    this->shapeMenu.addItem("Square", 1);
    this->shapeMenu.addItem("Circle", 2);
    this->shapeMenu.addItem("Triangle", 3);
    this->shapeMenu.setSelectedId(1, dontSendNotification);
    this->shapeMenu.addListener(this);
    
    addAndMakeVisible(&this->lineWidthLabel);
    this->lineWidthLabel.setText("Line Width", dontSendNotification);
    this->lineWidthLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    this->lineWidthLabel.setJustificationType(Justification::left);
    
    addAndMakeVisible(&this->lineWidthInput);
    this->lineWidthInput.onTextChange = [this] { lineWidthChanged(); };

    this->lineWidthInput.setColour(Label::textColourId, Colours::black);
    this->lineWidthInput.setColour(Label::textWhenEditingColourId, Colours::black);
    this->lineWidthInput.setText("4", dontSendNotification);
    this->lineWidthInput.setJustificationType(Justification::left);
    this->lineWidthInput.setEditable(true);
    
    addAndMakeVisible(&this->fillLabel);
    this->fillLabel.setText("Fill shape", dontSendNotification);
    this->fillLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    this->fillLabel.setJustificationType(Justification::left);
    
    addAndMakeVisible(&this->fillMenu);
    this->fillMenu.setColour(ComboBox::backgroundColourId, getStr()->SYSTEMCOLOUR.fundamental);
    this->fillMenu.setColour(ComboBox::outlineColourId, getStr()->SYSTEMCOLOUR.contents);
    this->fillMenu.setColour(ComboBox::focusedOutlineColourId, getStr()->SYSTEMCOLOUR.contents);
    this->fillMenu.addItem("Fill", 1);
    this->fillMenu.addItem("Border", 2);
    this->fillMenu.setSelectedId(2, dontSendNotification);
    this->fillMenu.addListener(this);
    
    addAndMakeVisible(&this->shapeColourLabel);
    this->shapeColourLabel.setText("Shape colour", dontSendNotification);
    this->shapeColourLabel.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
    this->shapeColourLabel.setJustificationType(Justification::left);
    addAndMakeVisible(&this->shapeColour);
    
    this->shapeColour.setCurrentColour(Colours::red);
    this->shapeColour.addChangeListener(this);
    
}

IRShapeController::~IRShapeController()
{
    
}


void IRShapeController::ControllerResized()
{
    int y = 10;
    int yIncrement = 30;
    int yBigIncrement = 40;
    
    this->LabelTitle.       setBounds(10, y, 300, 30);
    
    y += yBigIncrement;
    
    this->shapeMenuLabel.   setBounds(10, y, 100, 30);
    y += yIncrement;
    this->shapeMenu.        setBounds(10, y, getWidth() - 20, 30);
    
    y += yBigIncrement;

    this->lineWidthLabel.   setBounds(10, y, 100, 30);
    this->lineWidthInput.   setBounds(110, y, 100, 30);
    
    y += yBigIncrement;

    this->fillLabel.        setBounds(10, y, 100, 30);
    this->fillMenu.         setBounds(110, y, 100, 30);
    
    y += yBigIncrement;

    this->shapeColourLabel. setBounds(10, y, 100, 30);
    y += yIncrement;
    this->shapeColour.      setBounds(10, y, getWidth() - 20, 200);
    
}

void IRShapeController::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.contents);
    g.setColour(Colours::black);
    g.drawLine(0,42.5,getWidth(),42.5);
    //g.drawLine(0,250, getWidth(), 250);
    //g.drawLine(0,400,getWidth(),400);
}

void IRShapeController::shapeColourChanged()
{
    
}

void IRShapeController::changeListenerCallback(ChangeBroadcaster* source)
{
    if (source == &this->shapeColour)
    {
        shapeColourChanged();
        this->status = ColourChanged;
        sendChangeMessage();
    }
}

void IRShapeController::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if(comboBoxThatHasChanged == &this->fillMenu)
    {
        
        std::cout << "comboBoxChanged\n";
        this->status = FillMenuSelected;
        sendChangeMessage();
    }else if(comboBoxThatHasChanged == &this->shapeMenu)
    {
        this->status = ShapeMenuSelected;
        sendChangeMessage();
    }
}

void IRShapeController::lineWidthChanged()
{
    std::string val = this->lineWidthInput.getText().toStdString();
    float valFloat = atof(val.c_str());
    //std::cout << "fontSize value = " << valFloat << std::endl;
    
    if (valFloat == 0.0f)
    {
        valFloat = 4;
        this->lineWidthInput.setText("4", dontSendNotification);
    }
    
    this->lineWidth = valFloat;
    
    this->status = LineWidthChanged;
    sendChangeMessage();
}

void IRShapeController::setFillShape(bool flag)
{
    if(flag)
    {
        this->fillMenu.setSelectedId(1, dontSendNotification);
    }else{
        this->fillMenu.setSelectedId(2, dontSendNotification);
    }
    this->isFill = flag;
}

void IRShapeController::setShape(int index)
{
    this->shapeMenu.setSelectedId(index, dontSendNotification);
}


