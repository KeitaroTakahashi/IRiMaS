
#include "FontGUI.hpp"





FontGUI::FontGUI(String title)
{
    
    addAndMakeVisible(this->LabelTitle);
    this->LabelTitle.setText(title, dontSendNotification);
    this->LabelTitle.setColour(Label::textColourId, Colours::black);
    this->LabelTitle.setJustificationType(Justification::left);
    
    addAndMakeVisible(this->LabelFont);
    this->LabelFont.setText("Font : ", dontSendNotification);
    this->LabelFont.setColour(Label::textColourId, Colours::black);
    this->LabelFont.setJustificationType(Justification::left);
    
    // get info
    getAllFonts();
    
    makeFontMenu();
    makeFontStyleMenu();
    
    addAndMakeVisible(this->LabelStyle);
    this->LabelStyle.setText("Style : ", dontSendNotification);
    this->LabelStyle.setColour(Label::textColourId, Colours::black);
    this->LabelStyle.setJustificationType(Justification::left);
    
    addAndMakeVisible(this->LabelFontSize);
    this->LabelFontSize.setText("Font size : ", dontSendNotification);
    this->LabelFontSize.setColour(Label::textColourId, Colours::black);
    this->LabelFontSize.setJustificationType(Justification::left);
    
    addAndMakeVisible(this->FontSizeInput);
    this->FontSizeInput.setEditable(true);
    this->FontSizeInput.setText("16", dontSendNotification);
    this->FontSizeInput.setColour(Label::textColourId, Colours::black);
    this->FontSizeInput.setColour(Label::textWhenEditingColourId, Colours::black);
    
    this->FontSizeInput.setJustificationType(Justification::left);
    this->FontSizeInput.onTextChange = [this] { FontSizeInputChanged(); };
    
    addAndMakeVisible(this->LabelAlign);
    this->LabelAlign.setText("Align : ", dontSendNotification);
    this->LabelAlign.setColour(Label::textColourId, Colours::black);
    this->LabelAlign.setJustificationType(Justification::left);
    
    makeAlignMenu();
    
    addAndMakeVisible(this->LabelTextColour);
    this->LabelTextColour.setText("Text Colour : ", dontSendNotification);
    this->LabelTextColour.setColour(Label::textColourId, Colours::black);
    this->LabelTextColour.setJustificationType(Justification::left);
    
    addAndMakeVisible(this->textColour);
    this->textColour.setCurrentColour(Colours::black);
    this->textColour.addChangeListener(this);
    
    
    addAndMakeVisible(this->LabelBackgroundColour);
    this->LabelBackgroundColour.setText("BackGround Colour : ", dontSendNotification);
    this->LabelBackgroundColour.setColour(Label::textColourId, Colours::black);
    this->LabelBackgroundColour.setJustificationType(Justification::left);
    
    addAndMakeVisible(this->backgroundColour);
    this->backgroundColour.setCurrentColour(SYSTEMCOLOUR.background);
    this->backgroundColour.addChangeListener(this);
    
}


FontGUI::~FontGUI()
{
    
}


void FontGUI::resized()
{
    
    // margin 10
    this->LabelTitle.setBounds      (20, 10, 300, 30);
    
    this->LabelFont.setBounds       (10,50,100,30);
    this->FontMenu.setBounds        (110,50,250,30);
    
    this->LabelStyle.setBounds      (10,90,100,30);
    this->StyleMenu.setBounds       (110,90,250,30);
    
    this->LabelFontSize.setBounds   (10,130,100,30);
    this->FontSizeInput.setBounds   (110,130,100,30);
    
    this->LabelAlign.setBounds      (10,170,100,30);
    this->AlignMenu.setBounds       (110,170,250,30);
    
    this->LabelTextColour.setBounds (10,210,100,30);
    this->textColour.setBounds      (110,210,250,250);
    
    this->LabelBackgroundColour.setBounds (10,470, 100, 30);
    this->backgroundColour.setBounds      (110, 470, 250, 250);
    
}


void FontGUI::paint(Graphics& g)
{
    g.fillAll(Colours::white);
    g.setColour(Colours::black);
    g.drawLine(0,42.5,getWidth(),42.5);
}


void FontGUI::getAllFonts()
{
    Font f;
    this->fontFamilyList = f.findAllTypefaceNames();
    
    /*
     for(auto family : this->fontFamilyList){
     std::cout << family << std::endl;
     }*/
}


void FontGUI::getSelectedFontStyles(String fontName)
{
    Font f;
    f.setTypefaceName(fontName);
    this->fontStyleList = f.getAvailableStyles();
    
}


void FontGUI::makeFontMenu()
{
    addAndMakeVisible(this->FontMenu);
    
    this->FontMenu.setColour(ComboBox::backgroundColourId, SYSTEMCOLOUR.fundamental);
    this->FontMenu.setColour(ComboBox::outlineColourId, SYSTEMCOLOUR.contents);
    this->FontMenu.setColour(ComboBox::focusedOutlineColourId, SYSTEMCOLOUR.contents);
    int index = 1;
    for (auto family : this->fontFamilyList){
        this->FontMenu.addItem(family, index);
        index ++;
    }
    this->FontMenu.onChange = [this]{ FontMenuChanged(); };
    this->FontMenu.addListener(this);
    int defaultFontIndex = 1;
    this->FontMenu.setSelectedId(defaultFontIndex);
    
    getSelectedFontStyles(this->fontFamilyList[defaultFontIndex-1]);
}


void FontGUI::makeFontStyleMenu()
{
    addAndMakeVisible(this->StyleMenu);
    
    this->StyleMenu.setColour(ComboBox::backgroundColourId, SYSTEMCOLOUR.fundamental);
    this->StyleMenu.setColour(ComboBox::outlineColourId, SYSTEMCOLOUR.contents);
    this->StyleMenu.setColour(ComboBox::focusedOutlineColourId, SYSTEMCOLOUR.contents);
    int index = 1;
    for (auto style : this->fontStyleList){
        this->StyleMenu.addItem(style, index);
        index ++;
    }
    int defaultFontIndex = 1;
    this->StyleMenu.setSelectedId(defaultFontIndex);
    this->StyleMenu.onChange = [this] { FontStyleMenuChanged(); };
}


void FontGUI::makeAlignMenu()
{
    addAndMakeVisible(this->AlignMenu);
    this->AlignMenu.setColour(ComboBox::backgroundColourId, SYSTEMCOLOUR.fundamental);
    this->AlignMenu.setColour(ComboBox::outlineColourId, SYSTEMCOLOUR.contents);
    this->AlignMenu.setColour(ComboBox::focusedOutlineColourId, SYSTEMCOLOUR.contents);
    
    this->AlignMenu.addItem("Left",1);
    this->AlignMenu.addItem("Centre",2);
    this->AlignMenu.addItem("Right",3);
    this->AlignMenu.setSelectedId(1);
    this->AlignMenu.onChange = [this] { FontAlignMenuChanged(); };
}


void FontGUI::FontMenuChanged()
{
    std::cout << "Font changed" << std::endl;
    
    this->status = FontChanged;
    sendChangeMessage();
    
}


void FontGUI::FontStyleMenuChanged()
{
    this->status = FontStyleChanged;
    sendChangeMessage();
}


void FontGUI::FontSizeInputChanged()
{
    std::string val = this->FontSizeInput.getText().toStdString();
    float valFloat = atof(val.c_str());
    //std::cout << "fontSize value = " << valFloat << std::endl;
    
    if(valFloat == 0){
        valFloat = 1.0;
        this->FontSizeInput.setText("1.0", dontSendNotification);
    }
    
    this->fontSize = valFloat;
    
    this->status = FontSizeChanged;
    sendChangeMessage();
}


void FontGUI::FontAlignMenuChanged()
{
    this->status = FontAlignChanged;
    sendChangeMessage();
}


void FontGUI::FontColourMenuChanged()
{
    this->status = FontColourChanged;
    sendChangeMessage();
}


void FontGUI::BackgroundColourMenuChanged()
{
    this->status = BackgroundColourChanged;
    sendChangeMessage();
}


FontGUIStatus FontGUI::getChangeStatus() const
{
    return this->status;
}


void FontGUI::setTypefaceNameIndex(int index)
{
    this->FontMenu.setSelectedId(index);
}


String FontGUI::getTypefaceName() const
{
    return this->fontFamilyList[this->FontMenu.getSelectedId() - 1];
}


void FontGUI::setTypefaceName(const String newStyle)
{
    this->FontMenu.setSelectedId(this->fontFamilyList.indexOf(newStyle) + 1);
}


String FontGUI::getTypefaceStyle() const
{
    return this->fontStyleList[this->StyleMenu.getSelectedId()-1];
}


void FontGUI::setTypefaceStyle(const String newStyle)
{
    this->StyleMenu.setSelectedId(this->fontStyleList.indexOf(newStyle)+1);
}


int FontGUI::getAlign() const
{
    return this->AlignMenu.getSelectedId();
}


void FontGUI::setAlign(int newId)
{
    this->AlignMenu.setSelectedId(newId);
}


float FontGUI::getHeight() const
{
    return this->fontSize;
}


void FontGUI::setHeight(float newHeight)
{
    this->fontSize = newHeight;
}


Colour FontGUI::getTextColour() const
{
    return this->textColour.getCurrentColour();
}


void FontGUI::setTextColour(Colour newColour)
{
    this->textColour.setCurrentColour(newColour);
}


Colour FontGUI::getBackgroundColour() const
{
    return this->backgroundColour.getCurrentColour();
}


void FontGUI::setBackgroundColour(Colour newColour)
{
    this->backgroundColour.setCurrentColour(newColour);
}


void FontGUI::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &this->textColour)
    {
        // colour changed
        FontColourMenuChanged();
    }
    else if (source == &this->backgroundColour)
    {
        BackgroundColourMenuChanged();
    }
}


void FontGUI::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{
    std::cout << "comboBox selection changed" << std::endl;
}




