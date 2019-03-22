
#ifndef FontGUI_h
#define FontGUI_h

#include "JuceHeader.h"

#include "ColourLib.h"
#include "singletonClass.hpp"

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
    
    FontGUI(String title);
    ~FontGUI();
    
    void resized() override;
    void paint(Graphics& g) override;
    
    void getAllFonts();
    void getSelectedFontStyles(String fontName);
    
    void makeFontMenu();
    void makeFontStyleMenu();
    void makeAlignMenu();

    void FontMenuChanged();
    void FontStyleMenuChanged();
    void FontSizeInputChanged();
    void FontAlignMenuChanged();
    void FontColourMenuChanged();
    void BackgroundColourMenuChanged();
    
    FontGUIStatus getChangeStatus() const;
    
    void setTypefaceNameIndex(int index);
    String getTypefaceName() const;
    void setTypefaceName(const String newStyle);

    String getTypefaceStyle() const;
    void setTypefaceStyle(const String newStyle);

    int getAlign() const;
    void setAlign(int newId);
    
    float getHeight() const;
    void setHeight(float newHeight);
   
    Colour getTextColour() const;
    void setTextColour(Colour newColour);
    Colour getBackgroundColour() const;
    void setBackgroundColour(Colour newColour);

    void changeListenerCallback (ChangeBroadcaster* source) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    
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
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FontGUI)
    
};




#endif /* FontGUI_h */



