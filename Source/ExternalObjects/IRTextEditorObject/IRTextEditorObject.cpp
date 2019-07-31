
#include "IRTextEditorObject.hpp"




IRTextEditorObject::IRTextEditorObject(Component* parent) : IRNodeObject(parent, "IRTextEditor")
{
    this->preference = new IRTextEditorObjectPreference("TextEditor Preference", Rectangle<int>(400,720));
    this->preference->getFontGUI()->addChangeListener(this);
    
    this->font.setTypefaceName("Arial");
    this->font.setTypefaceStyle("Regular");
    this->font.setHeight(16.0);
    
    addAndMakeVisible(&this->textEditor);
    this->textEditor.setFont(this->font);
    //this->label.setFont(
    
    this->textEditor.setText("text...", dontSendNotification);
    //this->label.setEditable(false); // not editable initially...
    // this->textEditor.setColour(TextEditor::textColourId, SYSTEMCOLOUR.titleText);
    this->textEditor.setColour(TextEditor::backgroundColourId, SYSTEMCOLOUR.background);
    this->textEditor.applyColourToAllText(SYSTEMCOLOUR.titleText, true);
    
    // set editable condition
    
    this->textEditor.setMultiLine(true);
    this->textEditor.setReturnKeyStartsNewLine(true);
    
    this->textEditor.setJustification(Justification::left);
    
    childComponentManager(&this->textEditor);
    
    setSize(150, 150);
}


IRTextEditorObject::~IRTextEditorObject()
{
    delete this->preference;
}


IRNodeObject* IRTextEditorObject::copyThis()
{
    IRTextEditorObject* newObj = new IRTextEditorObject(this->parent);
    newObj->setFont(this->font);
    newObj->setTextColour(this->textColour);
    newObj->setAlign(this->alignId);
    newObj->textEditor.setText(this->textEditor.getText() ,dontSendNotification);
    return newObj;
}


t_json IRTextEditorObject::saveThisToSaveData()
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
        {"textColour", t_json::array({c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha()})},
        {"textContents", this->textEditor.getText().toStdString()}
    });
    
    
    return save;
}


void IRTextEditorObject::loadThisFromSaveData(t_json data)
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
    t_json::array colour = data["textColour"].array_items();
    Colour textColour = Colour((uint8)colour[0].int_value(),
                               (uint8)colour[1].int_value(),
                               (uint8)colour[2].int_value(),
                               (uint8)colour[3].int_value());
    
    this->textEditor.setColour(TextEditor::textColourId, textColour);
    this->textEditor.setColour(TextEditor::outlineColourId, Colours::transparentWhite);

    
    
    // set text contents
    this->textEditor.setText(String(data["textContents"].string_value()), dontSendNotification);
    
    // gui
    FontGUI* gui = this->preference->getFontGUI();
    gui->setTypefaceName(String(data["fontTypefaceName"].string_value()));
    gui->setTypefaceStyle(String(data["fontTypefaceStyle"].string_value()));
    gui->setHeight(data["textHeight"].int_value());
    gui->setAlign(data["textAlign"].int_value());
    gui->setTextColour(textColour);
    
}


void IRTextEditorObject::paint(Graphics &g)
{
    if (isEditMode())
    {
        auto area = getLocalBounds();
        
        g.setColour (SYSTEMCOLOUR.contents);
       // g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
        g.drawRect(area.toFloat(), 2.0);
    }
}


void IRTextEditorObject::resized()
{
    this->textEditor.setBounds(5,5, getWidth()-10, getHeight()-10);
    
}


void IRTextEditorObject::mouseDownEvent(const MouseEvent& e)
{
    //change preference Window if not yet
    
    IRPreferenceSpace* space = getPreferenceWindow()->getPreferenceSpace();
    
    IRPreferenceObject* current = space->getPreferenceObj();
    
    std::cout << "PreferenceOBject = " << preference << std::endl;
    
    if(current != preference){
        space->setPreferenceObj(preference);
    }
    
}


void IRTextEditorObject::changeListenerCallback (ChangeBroadcaster* source)
{
    FontGUI* fontGUI = this->preference->getFontGUI();
    
    if(source == fontGUI)
    {
        std::cout << fontGUI->getChangeStatus() << std::endl;
        switch (fontGUI->getChangeStatus()) {
            case FontChanged:
                this->font.setTypefaceName(fontGUI->getTypefaceName());
                std::cout << fontGUI->getTypefaceName() << std::endl;
                this->textEditor.applyFontToAllText(this->font);
                break;
            case FontStyleChanged:
                this->font.setTypefaceStyle(fontGUI->getTypefaceStyle());
                this->textEditor.applyFontToAllText(this->font);
                break;
            case FontSizeChanged:
                this->font.setHeight(fontGUI->getHeight());
                this->textEditor.applyFontToAllText(this->font);
                break;
            case FontAlignChanged:
                setAlign(fontGUI->getAlign());
                break;
            case FontColourChanged:
                this->textColour = fontGUI->getTextColour();
                this->textEditor.applyColourToAllText(this->textColour, true);
                repaint();
                break;
            default:
                break;
        }
    }
}


void IRTextEditorObject::setFont(Font font)
{
    this->font = font;
    this->textEditor.setFont(this->font);
}


void IRTextEditorObject::setAlign(int id)
{
    
    this->alignId = id;
    switch(id){
        case 1: // left
            this->textEditor.setJustification(Justification::left);
            break;
        case 2: // center
            this->textEditor.setJustification(Justification::centred);
            break;
        case 3: // right
            this->textEditor.setJustification(Justification::right);
            break;
        default:
            break;
    }
}


void IRTextEditorObject::setTextColour(Colour colour)
{
    this->textColour = colour;
    this->textEditor.setColour(Label::textColourId, colour);
}


Font IRTextEditorObject::getFont() const
{
    return this->font;
}


Colour IRTextEditorObject::getTextColour() const
{
    return this->textColour;
}


int IRTextEditorObject::getAlignId() const
{
    return this->alignId;
}




// **** **** PRIVATE METHODS **** **** //

void IRTextEditorObject::statusChangedCallback(IRNodeComponentStatus status)
{
    switch (status)
    {
        case EditModeStatus:
            if(isEditMode()){
                this->textEditor.setCaretVisible(false);
            }
            else{
                this->textEditor.setCaretVisible(true);
            }
            break;
        case SelectableStatus:
            break;
        case HasResizedStatus:
            break;
        default:
            break;
    }
}



