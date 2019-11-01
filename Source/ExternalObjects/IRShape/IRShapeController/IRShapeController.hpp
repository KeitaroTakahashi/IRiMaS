//
//  IRShapeController.hpp
//  IRiMaS
//
//  Created by Keitaro on 02/10/2019.
//

#ifndef IRShapeController_hpp
#define IRShapeController_hpp

#include "IRObjectController.hpp"
#include "IRColourSelector.h"

class IRShapeController : public IRObjectController,
public ChangeListener,
public ChangeBroadcaster,
private ComboBox::Listener

{
public:
    
    enum IRShapeControllerStatus
    {
        ColourChanged,
        FillMenuSelected,
        LineWidthChanged,
        ShapeMenuSelected,
        BorderWidthChanged
    };
    
    
    IRShapeController(IRStr* str);
    ~IRShapeController();

    // ==================================================
    
    void resized() override;
    void paint(Graphics& g) override;
    
    // ==================================================
    
    void setColour(Colour colour) { this->shapeColour.setCurrentColour(colour); }
    Colour getColour() const { return this->shapeColour.getCurrentColour(); }
    void setLineWidth(float width) { this->lineWidth = width; }
    float getLineWidth() const { return this->lineWidth; }
    void setFillShape(bool flag);
    bool isFillShape() const { return this->isFill; }
    int getSelectedShapeIndex() const { return this->shapeMenu.getSelectedId(); }
    
    void setShape(int index);

    
    int getFillMenuIndex() const { return this->fillMenu.getSelectedId(); }
    void lineWidthChanged();
    
    IRShapeControllerStatus getStatus() const { return this->status; }
    
private:
    
    
    // ==================================================
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    void changeListenerCallback(ChangeBroadcaster* source) override;

    
    // ==================================================

    IRShapeControllerStatus status;
    
    
    Label LabelTitle;
    Label shapeMenuLabel;
    ComboBox shapeMenu;
    
    Label fillLabel;
    ComboBox fillMenu;
    bool isFill = false;
    
    Label lineWidthLabel;
    Label lineWidthInput;
    float lineWidth = 4;
    
    Label shapeColourLabel;
    void shapeColourChanged();
    ColourSelector shapeColour
    {
        ColourSelector::showSliders
        | ColourSelector::showColourspace
        | ColourSelector::showAlphaChannel
    };
    // ==================================================
    
    //for combo box
    class BoxLookAndFeel : public LookAndFeel_V3
    {
    public:
        BoxLookAndFeel(IRStr* str) : str(str) {}
        
        Font getComboBoxFont(ComboBox& /* box */) override
        {
            return getNewFont();
        }
        Font getPopupMenuFont() override
        {
            return getNewFont();
        }
        
    private:
        IRStr* str;
        Font getNewFont()
        {
            return this->str->SYSTEMCOLOUR.h5;
        }
    };
    
    BoxLookAndFeel comboBoxFont;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRShapeController)

};

#endif /* IRShapeController_hpp */
