//
//  newFontController.h
//  IRiMaS
//
//  Created by Keitaro on 02/10/2019.
//

#ifndef newFontController_h
#define newFontController_h

#include "IRColourSettingIcon.h"

enum FontControllerStatus
{
    FontChanged,
    FontStyleChanged,
    FontSizeChanged,
    FontAlignChanged,
    FontColourChanged,
    BackgroundColourChanged
};


class FontController : public Component,
public IRStrComponent,
public ChangeBroadcaster,
public ChangeListener,
public ComboBox::Listener
{
    
public:
    
    FontController(IRStr* str) : IRStrComponent(str),
    textColourIcon(Colours::black, str)
    {
        StopWatch bench;
        addAndMakeVisible(this->labelFont);
        this->labelFont.setText("Font : ", dontSendNotification);
        //this->labelFont.setColour(Label::textColourId, Colours::black);
        this->labelFont.setJustificationType(Justification::left);
        
        // get info
        bench.start();
        getAllFonts();
        bench.result("getAllFonts");
        
        bench.start();

        makeFontMenu();
        makeFontStyleMenu();
        
        bench.result("makeFontMenu");

        
        addAndMakeVisible(this->labelStyle);
        this->labelStyle.setText("Style : ", dontSendNotification);
        //this->labelStyle.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
        this->labelStyle.setJustificationType(Justification::left);
        
        
        addAndMakeVisible(this->labelFontSize);
        this->labelFontSize.setText("Font size : ", dontSendNotification);
        //this->labelFontSize.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
        this->labelFontSize.setJustificationType(Justification::left);
        
        addAndMakeVisible(this->fontSizeInput);
        this->fontSizeInput.setEditable(true);
        this->fontSizeInput.setText("16", dontSendNotification);
        //this->fontSizeInput.setColour(Label::textColourId, Colours::black);
        //this->fontSizeInput.setColour(Label::textWhenEditingColourId, Colours::black);
        
        this->fontSizeInput.setJustificationType(Justification::left);
        this->fontSizeInput.onTextChange = [this] { fontSizeInputChanged(); };
        
        addAndMakeVisible(this->labelAlign);
        this->labelAlign.setText("Align : ", dontSendNotification);
        //this->labelAlign.setColour(Label::textColourId, Colours::black);
        this->labelAlign.setJustificationType(Justification::left);
        
        makeAlignMenu();
        
        addAndMakeVisible(this->labelTextColour);
        this->labelTextColour.setText("Text Colour : ", dontSendNotification);
        //this->labelTextColour.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
        this->labelTextColour.setJustificationType(Justification::left);
        

        // colour icon
        addAndMakeVisible(this->textColourIcon);
        this->textColourIcon.addChangeListener(this);
        

        addAndMakeVisible(this->labelBackgroundColour);
        this->labelBackgroundColour.setText("BackGround Colour : ", dontSendNotification);
        //this->labelBackgroundColour.setColour(Label::textColourId, getStr()->SYSTEMCOLOUR.text);
        this->labelBackgroundColour.setJustificationType(Justification::left);
        
       
    }
    ~FontController() {}
    
    void resized() override
    {
        int y = 0;
        int yIncrement = 30;
        int yBigIncrement = 30;
        
        int menuWidth = getWidth() - 20;
        int shortMenuWidth = getWidth() - 100;

        // margin 10
        //this->labelTitle.setBounds              (10, y, 300, 30);
        
        this->labelFont.setBounds               (10, y, 100, 30);
        y += yIncrement;
        this->fontMenu.setBounds                (10, y, menuWidth, 30);
        
        y += yBigIncrement;

        this->labelStyle.setBounds              (10, y, 80, 30);
        this->styleMenu.setBounds               (90, y, shortMenuWidth, 30);
        
        y += yBigIncrement;

        this->labelFontSize.setBounds           (10, y, 80, 30);
        this->fontSizeInput.setBounds           (90, y, shortMenuWidth, 30);
        
        y += yBigIncrement;

        this->labelAlign.setBounds              (10, y, 80, 30);
        this->alignMenu.setBounds               (90, y, shortMenuWidth, 30);
        
        y += yBigIncrement + 5;

        this->labelTextColour.setBounds         (10, y, 100, 30);
        this->textColourIcon.setBounds          (getWidth() - 10 - 80, y + 5, 80, 20);
        /*
        this->labelBackgroundColour.setBounds   (10, y, 100, 30);
        y += yIncrement;
        this->backgroundColour.setBounds        (10, y, menuWidth, 200);
         */
    }
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::transparentWhite);
        g.setColour(Colours::black);
        //g.drawLine(0, 42.5, getWidth(), 42.5);
    }
    
    void getAllFonts()
    {
        Font f;
        this->fontFamilyList = getStr()->fontFamilyList;
    }
    void getSelectedFontStyles(String fontName)
    {
        Font f;
        f.setTypefaceName(fontName);
        this->fontStyleList = f.getAvailableStyles();
    }
    
    void makeFontMenu()
    {
        addAndMakeVisible(this->fontMenu);
        
        this->fontMenu.setColour(ComboBox::backgroundColourId, getStr()->SYSTEMCOLOUR.fundamental);
        this->fontMenu.setColour(ComboBox::outlineColourId, getStr()->SYSTEMCOLOUR.contents);
        this->fontMenu.setColour(ComboBox::focusedOutlineColourId, getStr()->SYSTEMCOLOUR.contents);
        int index = 1;
        for (auto family : this->fontFamilyList)
        {
            this->fontMenu.addItem(family, index);
            index++;
        }
        this->fontMenu.onChange = [this] { fontMenuChanged(); };
        this->fontMenu.addListener(this);
        int defaultFontIndex = 1;
        this->fontMenu.setSelectedId(defaultFontIndex);
        
        getSelectedFontStyles(this->fontFamilyList[defaultFontIndex-1]);
    }
    void makeFontStyleMenu()
    {
        addAndMakeVisible(this->styleMenu);
        
        this->styleMenu.setColour(ComboBox::backgroundColourId, getStr()->SYSTEMCOLOUR.fundamental);
        this->styleMenu.setColour(ComboBox::outlineColourId, getStr()->SYSTEMCOLOUR.contents);
        this->styleMenu.setColour(ComboBox::focusedOutlineColourId, getStr()->SYSTEMCOLOUR.contents);
        int index = 1;
        for (auto style : this->fontStyleList)
        {
            this->styleMenu.addItem(style, index);
            index++;
        }
        int defaultFontIndex = 1;
        this->styleMenu.setSelectedId(defaultFontIndex);
        this->styleMenu.onChange = [this] { fontStyleMenuChanged(); };
    }
    
    void makeAlignMenu()
    {
        addAndMakeVisible(this->alignMenu);
        this->alignMenu.setColour(ComboBox::backgroundColourId, getStr()->SYSTEMCOLOUR.fundamental);
        this->alignMenu.setColour(ComboBox::outlineColourId, getStr()->SYSTEMCOLOUR.contents);
        this->alignMenu.setColour(ComboBox::focusedOutlineColourId, getStr()->SYSTEMCOLOUR.contents);
        
        this->alignMenu.addItem("Left",1);
        this->alignMenu.addItem("Centre",2);
        this->alignMenu.addItem("Right",3);
        this->alignMenu.setSelectedId(1);
        this->alignMenu.onChange = [this] { fontAlignMenuChanged(); };
    }
    
    void fontMenuChanged()
    {
        
        this->status = FontChanged;
        sendChangeMessage();
    }
    void fontStyleMenuChanged()
    {
        this->status = FontStyleChanged;
        sendChangeMessage();
    }
    void fontSizeInputChanged()
    {
        std::string val = this->fontSizeInput.getText().toStdString();
        float valFloat = atof(val.c_str());
        //std::cout << "fontSize value = " << valFloat << std::endl;
        
        if (valFloat == 0.0f)
        {
            valFloat = 1.0f;
            this->fontSizeInput.setText("16", dontSendNotification);
        }
        
        this->fontSize = valFloat;
        
        this->status = FontSizeChanged;
        sendChangeMessage();
    }
    void fontAlignMenuChanged()
    {
        this->status = FontAlignChanged;
        sendChangeMessage();
    }
    void fontColourMenuChanged()
    {
        this->status = FontColourChanged;
        sendChangeMessage();
    }
    void backgroundColourMenuChanged()
    {
        this->status = BackgroundColourChanged;
        sendChangeMessage();
    }
    
    FontControllerStatus getChangeStatus() const { return this->status; }
    
    void setTypefaceNameIndex(int index) { this->fontMenu.setSelectedId(index); }
    String getTypefaceName() const { return this->fontFamilyList[this->fontMenu.getSelectedId() - 1]; }
    void setTypefaceName(const String newStyle) { this->fontMenu.setSelectedId(this->fontFamilyList.indexOf(newStyle) + 1); }
    
    String getTypefaceStyle() const { return this->fontStyleList[this->styleMenu.getSelectedId() - 1]; }
    void setTypefaceStyle(const String newStyle) { this->styleMenu.setSelectedId(this->fontStyleList.indexOf(newStyle) + 1); }
    
    int getAlign() const { return this->alignMenu.getSelectedId(); }
    void setAlign(int newId) { this->alignMenu.setSelectedId(newId); }
    
    float getHeight() const { return this->fontSize; }
    void setHeight(float newHeight) { this->fontSize = newHeight; }
    
    Colour getTextColour() const { return this->textColourIcon.getCurrentColour(); }
    void setTextColour(Colour newColour) { this->textColourIcon.setCurrentColour(newColour); }
    
    void changeListenerCallback(ChangeBroadcaster* source) override
    {
       if (source == &this->textColourIcon)
        {
            fontColourMenuChanged();
        }
    }
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override
    {

    }
    
    
private:
    
    //Label labelTitle;
    
    Label labelFont;
    ComboBox fontMenu;
    Label labelFontInput;
    
    Label labelStyle;
    ComboBox styleMenu;
    
    Label labelFontSize;
    Label fontSizeInput;
    float fontSize;
    
    Label labelTextColour;
    Label labelBackgroundColour;
    
    Label labelAlign;
    ComboBox alignMenu;
    
    StringArray fontStyleList;
    StringArray fontFamilyList;
    
    IRColourSettingIcon textColourIcon;
    
    FontControllerStatus status;
    
    // system colour
    //IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FontController)
    
};


#endif /* newFontController_h */
