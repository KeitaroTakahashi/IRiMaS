//
//  IRTextEditor.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 26/08/2018.
//

#ifndef IRTextEditor_h
#define IRTextEditor_h

#include "IRNodeObject.hpp"
#include "IRLabelObjectPreference.hpp"

class IRLabelObject : public IRNodeObject,
public ChangeListener
{
public:
    IRLabelObject(Component* parent) : IRNodeObject(parent, "IRLabel")
    {
        
        // initialize preference
        this->preference = new IRLabelObjectPreference("Label Preference", Rectangle<int>(400,720));
        this->preference->getFontGUI()->addChangeListener(this);
        
        this->font.setTypefaceName("Arial");
        this->font.setTypefaceStyle("Regular");
        this->font.setHeight(16.0);
        
        setSize(100,25);
        addAndMakeVisible(&this->label);
        this->label.setFont(this->font);
        //this->label.setFont(
        
        this->label.setText("text...", dontSendNotification);
        this->label.setJustificationType(Justification::left);
        this->label.setColour(Label::textColourId, SYSTEMCOLOUR.titleText);
        this->label.setColour(Label::textWhenEditingColourId, SYSTEMCOLOUR.titleText);

        this->label.setMinimumHorizontalScale(1.0);
        // set editable condition
        this->label.setEditable(false, // edit on single click
                                true, // edit on double click
                                false // loss of focus discards changes
                                );
        
        childComponentManager(&this->label);
        Image img;
        //IRFactory.registerObject<IRLabelObject*>("IRLabel", "Label", img);
        
        std::cout << "IRLabel initialized\n";

    }
    // ------------------------------------------------------------

    ~IRLabelObject() {}
    // ------------------------------------------------------------
    IRNodeObject* copyThis() override
    {
        IRLabelObject* newObj = new IRLabelObject(this->parent);
        newObj->setFont(this->font);
        newObj->label.setText(label.getText(true) ,dontSendNotification);
        return newObj;
    }
    // ------------------------------------------------------------

    t_json saveThisToSaveData() override
    {
        FontGUI* gui = this->preference->getFontGUI();
        Colour c = gui->getTextColour();
        
        std::string align = std::to_string(gui->getAlign());
        
        //std::string LabelTextContents = this->label
        t_json save = t_json::object({
            {"fontTypefaceName", this->font.getTypefaceName().toStdString()},
            {"fontTypefaceStyle", this->font.getTypefaceStyle().toStdString()},
            {"fontHeight", this->font.getHeight()},
            {"textAlign", gui->getAlign()},
            {"textColour", json11::Json::array({c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha()})},
            {"textContents", this->label.getText().toStdString()}
        });
        
        
        return save;
    }
    
    void loadThisFromSaveData(t_json data) override
    {
        // example : string value
        this->font.setTypefaceName(String(data["fontTypefaceName"].string_value()));
        this->font.setTypefaceStyle(String(data["fontTypefaceStyle"].string_value()));
        // example : int value
        this->font.setHeight(data["fontHeight"].int_value());
        // set align
        setAlign(data["textAlign"].int_value());
        
        // set font
        setFont(this->font);
        
        // example : array
        json11::Json::array colour = data["textColour"].array_items();
        Colour textColour = Colour((uint8)colour[0].int_value(),
                                   (uint8)colour[1].int_value(),
                                   (uint8)colour[2].int_value(),
                                   (uint8)colour[3].int_value());
        
        this->label.setColour(Label::textColourId, textColour);
        
        // set text contents
        this->label.setText(String(data["textContents"].string_value()), dontSendNotification);
        
        // gui
        FontGUI* gui = this->preference->getFontGUI();
        gui->setTypefaceName(String(data["fontTypefaceName"].string_value()));
        gui->setTypefaceStyle(String(data["fontTypefaceStyle"].string_value()));
        gui->setHeight(data["textHeight"].int_value());
        gui->setAlign(data["textAlign"].int_value());
        gui->setTextColour(textColour);

    }
    
    // ------------------------------------------------------------

    void paint(Graphics &g) override
    {
        if(isEditMode())
        {
            auto area = getLocalBounds().reduced (2);
            
            g.setColour (SYSTEMCOLOUR.contents);
            g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
        }
    }
    
    // ------------------------------------------------------------
    void resized() override
    {
        this->label.setBounds(5,5, getWidth()-10, getHeight()-10);
    }
    // ------------------------------------------------------------
    
    void mouseDownEvent(const MouseEvent& e) override
    {
        //change preference Window if not yet
        
        IRPreferenceSpace* space = getPreferenceWindow()->getPreferenceSpace();
        
        IRPreferenceObject* current = space->getPreferenceObj();
                
        std::cout << "PreferenceOBject = " << preference << std::endl;
        
        if(current != preference){
            space->setPreferenceObj(preference);
        }
        
        

        
    }
    
    //===============================================================
    //Listener
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        FontGUI* fontGUI = this->preference->getFontGUI();

   
        if(source == fontGUI)
        {
            //std::cout << fontGUI->getChangeStatus() << std::endl;
            switch (fontGUI->getChangeStatus()) {
                case FontChanged:
                    this->font.setTypefaceName(fontGUI->getTypefaceName());
                    std::cout << fontGUI->getTypefaceName() << std::endl;
                    this->label.setFont(this->font);
                    break;
                case FontStyleChanged:
                    this->font.setTypefaceStyle(fontGUI->getTypefaceStyle());
                    this->label.setFont(this->font);
                    break;
                case FontSizeChanged:
                    this->font.setHeight(fontGUI->getHeight());
                    this->label.setFont(this->font);
                    break;
                case FontAlignChanged:
                    
                    std::cout << "align number = " << fontGUI->getAlign() << std::endl;

                    setAlign(fontGUI->getAlign());
                    break;
                case FontColourChanged:
                    this->label.setColour(Label::textColourId, fontGUI->getTextColour());
                    repaint();
                    break;
                default:
                    break;
            }
            
        }
            
        
    }
    // ------------------------------------------------------------
    void setFont(Font font){
        this->font = font;
        this->label.setFont(this->font);
    }
    
    Font getFont() const { return this->font; }
    
    void setAlign(int newAlign)
    {
        switch(newAlign){
            case 1: // left
                this->label.setJustificationType(Justification::left);
                break;
            case 2: // center
                this->label.setJustificationType(Justification::centred);
                break;
            case 3: // right
                this->label.setJustificationType(Justification::right);
                break;
            default:
                break;
        }
    }
    
    // ------------------------------------------------------------

    // ============================================================
    Label label;
    
    // preference
    IRLabelObjectPreference *preference;

    
private:
    // ------------------------------------------------------------
    // call back function automatically called when the status of this object changed by others.
    // write some tasks here
    void statusChangedCallback(IRNodeComponentStatus status) override
    {
        switch (status)
        {
            case EditModeStatus:
                break;
            case SelectableStatus:
                break;
            case HasResizedStatus:
                break;
            default:
                break;
        }
    }
    
    Font font;
    

};

#endif /* IRTextEditor_h */
