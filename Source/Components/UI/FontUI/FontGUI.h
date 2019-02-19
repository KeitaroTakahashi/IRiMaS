//
//  FontGUI.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 22/10/2018.
//

#ifndef FontGUI_h
#define FontGUI_h

enum FontGUIStatus
{
    FontChanged,
    FontStyleChanged,
    FontSizeChanged,
    FontAlignChanged,
    FontColourChanged,
    BackgroundColourChanged
};

class FontGUI : public Component,
public ChangeBroadcaster,
public ChangeListener,
public ComboBox::Listener
{
  
public:
    FontGUI(String title)
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
    ~FontGUI()
    {
        
    }
    
    //===============================================================
    
    void resized() override
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
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::white);
        g.setColour(Colours::black);
        g.drawLine(0,42.5,getWidth(),42.5);
    }
    
    // --------------------------------------------------------------
    void getAllFonts()
    {
        Font f;
        this->fontFamilyList = f.findAllTypefaceNames();
        
        /*
        for(auto family : this->fontFamilyList){
            std::cout << family << std::endl;
        }*/
    }
    void getSelectedFontStyles(String fontName)
    {
        Font f;
        f.setTypefaceName(fontName);
        this->fontStyleList = f.getAvailableStyles();
        
    }
    //===============================================================
    // --------------------------------------------------------------
    void makeFontMenu()
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
    // --------------------------------------------------------------
    void makeFontStyleMenu()
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
    // --------------------------------------------------------------
    void makeAlignMenu()
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
    // --------------------------------------------------------------
    //===============================================================

    void FontMenuChanged()
    {
        std::cout << "font changed!" << std::endl;
        
        this->status = FontChanged;
        sendChangeMessage();

    }
    // --------------------------------------------------------------
    void FontStyleMenuChanged()
    {
        this->status = FontStyleChanged;
        sendChangeMessage();
    }
    // --------------------------------------------------------------
    void FontSizeInputChanged()
    {
        std::string val = this->FontSizeInput.getText().toStdString();
        float valFloat = atof(val.c_str());
        //std::cout << "fontSize value = " << valFloat << std::endl;
        
        if(valFloat == 0){
            valFloat = 1.0;
            this->FontSizeInput.setText("1.0",dontSendNotification);
        }
        
        this->fontSize = valFloat;
        
        this->status = FontSizeChanged;
        sendChangeMessage();
    }
    // --------------------------------------------------------------
    void FontAlignMenuChanged()
    {
        this->status = FontAlignChanged;
        sendChangeMessage();
    }
    // --------------------------------------------------------------
    void FontColourMenuChanged()
    {
        this->status = FontColourChanged;
        sendChangeMessage();
    }
    
    void BackgroundColourMenuChanged()
    {
        this->status = BackgroundColourChanged;
        sendChangeMessage();
    }
    // --------------------------------------------------------------
    FontGUIStatus getChangeStatus() const { return this->status; }
    
    //===============================================================
    void setTypefaceNameIndex(int index){ this->FontMenu.setSelectedId(index); }
    //===============================================================
    String getTypefaceName() const {return this->fontFamilyList[this->FontMenu.getSelectedId()-1];}
    void setTypefaceName(const String newStyle) { this->FontMenu.setSelectedId(this->fontFamilyList.indexOf(newStyle)+1);}

    // --------------------------------------------------------------
    String getTypefaceStyle() const {return this->fontStyleList[this->StyleMenu.getSelectedId()-1];}
    void setTypefaceStyle(const String newStyle) { this->StyleMenu.setSelectedId(this->fontStyleList.indexOf(newStyle)+1);}
    // --------------------------------------------------------------
    int getAlign() const { return this->AlignMenu.getSelectedId(); }
    void setAlign(int newId) { this->AlignMenu.setSelectedId(newId); }
    // --------------------------------------------------------------
    float getHeight() const { return this->fontSize; }
    void setHeight(float newHeight) { this->fontSize = newHeight; }
    // --------------------------------------------------------------
    Colour getTextColour() const { return this->textColour.getCurrentColour(); }
    void setTextColour(Colour newColour) { this->textColour.setCurrentColour(newColour); }
    Colour getBackgroundColour() const { return this->backgroundColour.getCurrentColour(); }
    void setBackgroundColour(Colour newColour) { this->backgroundColour.setCurrentColour(newColour); }

    //===============================================================
    //Listener
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        if(source == &this->textColour)
        {
            // colour changed
            FontColourMenuChanged();
        }
        else if(source == &this->backgroundColour)
        {
            BackgroundColourMenuChanged();
        }
    }
    
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override
    {
        std::cout << "comboBox selection changed\n";
    }
    
    //===============================================================
    
private:
    
    Label LabelTitle;
    
    Label LabelFont;
    ComboBox FontMenu;
    Label LabelFontInput;
    
    Label LabelStyle;
    ComboBox StyleMenu;
    
    Label LabelFontSize;
    Label FontSizeInput;
    float fontSize;
    
    Label LabelTextColour;
    Label LabelBackgroundColour;
    
    Label LabelAlign;
    ComboBox AlignMenu;
    
    StringArray fontStyleList;
    StringArray fontFamilyList;
    
    ColourSelector textColour  {
          ColourSelector::showSliders
        | ColourSelector::showColourspace
        | ColourSelector::showAlphaChannel
    };
    
    ColourSelector backgroundColour {
          ColourSelector::showSliders
        | ColourSelector::showColourspace
        | ColourSelector::showAlphaChannel
    };
    
    FontGUIStatus status;
    
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FontGUI)
};

#endif /* FontGUI_h */
