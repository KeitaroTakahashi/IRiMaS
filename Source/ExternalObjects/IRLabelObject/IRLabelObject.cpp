
#include "IRLabelObject.hpp"

IRLabelObject::IRLabelObject(Component* parent) : IRNodeObject(parent, "IRLabel")
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
    
    // Link Menu
    clearLinkParam();
    addLinkParam(TextLinkFlag);
    addLinkParam(ConsoleLinkFlag);
    addLinkParam(AudioLinkFlag);
    addLinkParam(ImageLinkFlag);
    addLinkParam(DataLinkFlag);

}


IRLabelObject::~IRLabelObject()
{
    // std::cout << "~IRLABELOBJECT DESTRUCTOR" << std::endl;
    delete this->preference;
}


IRNodeObject* IRLabelObject::copyThis()
{
    IRLabelObject* newObj = new IRLabelObject(this->parent);
    newObj->setFont(this->font);
    newObj->label.setText(label.getText(true) ,dontSendNotification);
    return newObj;
}


t_json IRLabelObject::saveThisToSaveData()
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


void IRLabelObject::loadThisFromSaveData(t_json data)
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


void IRLabelObject::paint(Graphics &g)
{
    if (isEditMode())
    {
        auto area = getLocalBounds().reduced (2);
        
        g.setColour (SYSTEMCOLOUR.contents);
        g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
    }

}

void IRLabelObject::paintOnWorkspace(Graphics& g, Component* workspace)
{
    
}


void IRLabelObject::resized()
{
    this->label.setBounds(5,5, getWidth()-10, getHeight()-10);

}


void IRLabelObject::mouseDownEvent(const MouseEvent& e)
{
    //change preference Window if not yet
    
    IRPreferenceSpace* space = getPreferenceWindow()->getPreferenceSpace();
    
    IRPreferenceObject* current = space->getPreferenceObj();
    
    std::cout << "PreferenceOBject = " << preference << std::endl;
    
    if (current != preference){
        space->setPreferenceObj(preference);
    }
}


void IRLabelObject::changeListenerCallback (ChangeBroadcaster* source)
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


void IRLabelObject::setFont(Font font)
{
    this->font = font;
    this->label.setFont(this->font);
}


Font IRLabelObject::getFont() const
{
    return this->font;
}


void IRLabelObject::setAlign(int newAlign)
{
    switch(newAlign)
    {
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




// **** **** PRIVATE METHODS **** **** //




void IRLabelObject::statusChangedCallback(IRNodeComponentStatus status)
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



