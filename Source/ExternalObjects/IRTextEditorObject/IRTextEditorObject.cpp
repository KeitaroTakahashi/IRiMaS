
#include "IRTextEditorObject.hpp"




IRTextEditorObject::IRTextEditorObject(Component* parent, IRStr* str) :
IRNodeObject(parent, "IRTextEditor", str, NodeObjectType(ordinaryIRComponent))
{
    
    this->textColour = Colours::black;
    this->backgroundColour = getStr()->SYSTEMCOLOUR.background.withAlpha(0.0f);
    this->controller.reset(new IRTextEditorController(str));
    this->controller->getFontController()->addChangeListener(this);
    this->controller->getFontController()->setBackgroundColour(this->backgroundColour);
    this->controller->getArrangeController()->addChangeListener(this);
    setObjController(this->controller.get());
    
    this->font.setTypefaceName("Arial");
    this->font.setTypefaceStyle("Regular");
    this->font.setHeight(16.0);
    
    addAndMakeVisible(&this->textEditor);
    this->textEditor.setFont(this->font);
    

    this->textEditor.setText("text...", dontSendNotification);
    this->textEditor.setColour(TextEditor::backgroundColourId, this->backgroundColour);
    this->textEditor.setColour(TextEditor::outlineColourId,
                               Colours::transparentBlack);
    this->textEditor.setColour(TextEditor::focusedOutlineColourId,
                               Colours::transparentBlack);

    this->textEditor.applyColourToAllText(Colours::black, true);
    
    this->textEditor.onReturnKey = [this] { onReturnKeyAction(); };
    this->textEditor.onTextChange = [this] { onTextChangeAction(); };
    
    // set editable condition
    
    this->textEditor.setMultiLine(true);
    this->textEditor.setReturnKeyStartsNewLine(true);
    
    this->textEditor.setJustification(Justification::left);
    
    childComponentManager(&this->textEditor);
    
    // default size
    setObjectBounds(0, 0, 150, 150);
    
}


IRTextEditorObject::~IRTextEditorObject()
{
    this->controller.reset();
}


IRNodeObject* IRTextEditorObject::copyThis()
{
    IRTextEditorObject* newObj = new IRTextEditorObject(this->parent, getStr());
    newObj->setFont(this->font);
    newObj->setTextColour(this->textColour);
    newObj->setBackgroundColour(this->backgroundColour);
    newObj->setAlign(this->alignId);
    newObj->textEditor.setText(this->textEditor.getText() ,dontSendNotification);
    newObj->applyColourToAllText(this->textColour);
    return newObj;
}

void IRTextEditorObject::copyData(IRTextEditorObject* data)
{
    this->setFont(data->font);
    this->setTextColour(data->textColour);
    this->setBackgroundColour(data->backgroundColour);
    this->setAlign(data->alignId);
    this->textEditor.setText(data->textEditor.getText() ,dontSendNotification);
    setEncloseMode(data->getEncloseMode());
    
    this->textEditor.applyFontToAllText(this->font);
    this->applyColourToAllText(data->textColour);
}


t_json IRTextEditorObject::saveThisToSaveData()
{
    FontController* gui = this->controller->getFontController();
    Colour c = gui->getTextColour();
    Colour b = gui->getBackgroundColour();
    
    std::string align = std::to_string(gui->getAlign());
    
    //std::string LabelTextContents = this->label
    t_json save = t_json::object({
        {"fontTypefaceName", this->font.getTypefaceName().toStdString()},
        {"fontTypefaceStyle", this->font.getTypefaceStyle().toStdString()},
        {"fontHeight", this->font.getHeight()},
        {"textAlign", gui->getAlign()},
        {"textColour", t_json::array({c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha()})},
        {"backgroundColour", t_json::array({b.getRed(), b.getGreen(), b.getBlue(), b.getAlpha()})},
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
    t_json::array colour2 = data["backgroundColour"].array_items();
    Colour backgroundColour = Colour((uint8)colour2[0].int_value(),
                                     (uint8)colour2[1].int_value(),
                                     (uint8)colour2[2].int_value(),
                                     (uint8)colour2[3].int_value());
    
    this->textEditor.setColour(TextEditor::textColourId, textColour);
    this->textEditor.setColour(TextEditor::outlineColourId, Colours::transparentWhite);
    this->textEditor.setColour(TextEditor::backgroundColourId , backgroundColour);

    // set text contents
    this->textEditor.setText(String(data["textContents"].string_value()), dontSendNotification);
    
    // gui
    FontController* gui = this->controller->getFontController();
    gui->setTypefaceName(String(data["fontTypefaceName"].string_value()));
    gui->setTypefaceStyle(String(data["fontTypefaceStyle"].string_value()));
    gui->setHeight(data["textHeight"].int_value());
    gui->setAlign(data["textAlign"].int_value());
    gui->setTextColour(textColour);
    gui->setBackgroundColour(backgroundColour);
    
}


void IRTextEditorObject::paint(Graphics &g)
{
    //g.fillAll(getStr()->SYSTEMCOLOUR.background);
    if (isEditMode())
    {
        //auto area = getLocalBounds();
        
        //g.setColour (getStr()->SYSTEMCOLOUR.contents);
       // g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
       // g.drawRect(area.toFloat(), 2.0);
    }
}


void IRTextEditorObject::resized()
{
    this->textEditor.setBounds(getLocalBounds());
}

void IRTextEditorObject::ObjectPositionChanged(int x, int y)
{
    

}
// ------------------------------------------------------------

// ------------------------------------------------------------

void IRTextEditorObject::mouseDownEvent(const MouseEvent& e)
{
    std::cout << "IRTextEditorObject mouseDown at " << this->textEditor.getCaretPosition() << std::endl;
    
}

// ------------------------------------------------------------

void IRTextEditorObject::IRChangeListenerCallback (ChangeBroadcaster* source)
{
    FontController* fontGUI = this->controller->getFontController();
    ArrangeController* arrangeGUI = this->controller->getArrangeController();

    if(source == fontGUI)
    {
        fontControllerChangedCallback(fontGUI);
    }
}
// ------------------------------------------------------------

void IRTextEditorObject::fontControllerChangedCallback(FontController* source)
{
    switch (source->getChangeStatus()) {
        case FontChanged:
            this->font.setTypefaceName(source->getTypefaceName());
            this->textEditor.applyFontToAllText(this->font);
            textArrangeChanged();
            break;
        case FontStyleChanged:
            this->font.setTypefaceStyle(source->getTypefaceStyle());
            this->textEditor.applyFontToAllText(this->font);
            textArrangeChanged();
            break;
        case FontSizeChanged:
            this->font.setHeight(source->getHeight());
            this->textEditor.applyFontToAllText(this->font);
            textArrangeChanged();
            break;
        case FontAlignChanged:
            setAlign(source->getAlign());
            break;
        case FontColourChanged:
            this->textColour = source->getTextColour();
            this->textEditor.applyColourToAllText(this->textColour, true);
            repaint();
            textArrangeChanged();
            break;
        case BackgroundColourChanged:
            this->backgroundColour = source->getBackgroundColour();
            setBackgroundColour(this->backgroundColour);
            break;
        default:
            break;
    }
}


// ------------------------------------------------------------


void IRTextEditorObject::setFont(Font font)
{
    this->font = font;
    this->textEditor.setFont(this->font);
    
    textArrangeChanged();
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
    
    
    textArrangeChanged();
}

// ==================================================

void IRTextEditorObject::setTextColour(Colour colour)
{
    this->textColour = colour;
    this->textEditor.setColour(TextEditor::textColourId, colour);
    this->controller->getFontController()->setTextColour(colour);

    textArrangeChanged();
}

void IRTextEditorObject::setTextColour(Colour colour, float alpha)
{
    setTextColour(Colour(colour.getRed(), colour.getGreen(), colour.getBlue(), alpha));

}


void IRTextEditorObject::setTextColour(uint8 red, uint8 green, uint8 blue, float alpha)
{
    setTextColour(Colour(red, green, blue, alpha));
}

void IRTextEditorObject::applyColourToAllText(Colour colour)
{
    this->textColour = colour;
    this->textEditor.applyColourToAllText(colour);
    this->controller->getFontController()->setTextColour(colour);

    textArrangeChanged();
}

void IRTextEditorObject::applyColourToAllText(Colour colour, float alpha)
{
    applyColourToAllText(Colour(colour.getRed(), colour.getGreen(), colour.getBlue(), alpha));

}


void IRTextEditorObject::applyColourToAllText(uint8 red, uint8 green, uint8 blue, float alpha)
{
    applyColourToAllText(Colour(red, green, blue, alpha));
}

// ==================================================

void IRTextEditorObject::setBackgroundColour(Colour colour)
{
    this->backgroundColour = colour;
    this->textEditor.setColour(TextEditor::backgroundColourId, colour);
    this->controller->getFontController()->setBackgroundColour(colour);
    repaint();
    
    textArrangeChanged();
}

void IRTextEditorObject::setBackgroundColour(Colour colour, float alpha)
{
    setBackgroundColour(Colour(colour.getRed(), colour.getGreen(), colour.getBlue(), alpha));

}


void IRTextEditorObject::setBackgroundColour(uint8 red, uint8 green, uint8 blue, float alpha)
{
    setBackgroundColour(Colour(red, green, blue, alpha));
}

// ==================================================
Font IRTextEditorObject::getFont() const
{
    return this->font;
}


Colour IRTextEditorObject::getTextColour() const
{
    return this->textColour;
}

Colour IRTextEditorObject::getBackgroundColour() const
{
    return this->backgroundColour;
}

int IRTextEditorObject::getAlignId() const
{
    return this->alignId;
}

int IRTextEditorObject::getTextWidth() const
{
    return this->textEditor.getTextWidth();
}
int IRTextEditorObject::getTextHeight() const
{
    return this->textEditor.getTextHeight();
}

void IRTextEditorObject::encloseStatusChangedListener()
{
    auto arr = this->controller->getArrangeController();
    
    bool id = arr->getEnclosedButtonStatus();
    
}


// ------------------------------------------------------------

void IRTextEditorObject::statusInEditMode()
{
    this->textEditor.setCaretVisible(true);
    this->textEditor.setReadOnly(false);
    
    // redefine MouseLIstenr
    //this->textEditor.addMouseListener(this, true);
    //this->textEditor.setInterceptsMouseClicks(true, false);
    
}

void IRTextEditorObject::statusInControlMode()
{
    this->textEditor.setCaretVisible(true);
    this->textEditor.setReadOnly(false);
    
    
}
// **** **** PRIVATE METHODS **** **** //

// ------------------------------------------------------------

void IRTextEditorObject::statusChangedCallback(IRNodeComponentStatus status)
{
    switch (status)
    {
        case EditModeStatus:
            if(isEditMode()) statusInEditMode();
            else statusInControlMode();
            break;
        case SelectableStatus:
            break;
        case HasResizedStatus:
            break;
        default:
            break;
    }
}

void IRTextEditorObject::selectedChangedAction(bool flag)
{
    if(flag)
    {
        this->textEditor.setColour(TextEditor::outlineColourId,
                                   Colours::black);
        this->textEditor.setColour(TextEditor::focusedOutlineColourId,
                                   Colours::black);
    }else{
        this->textEditor.setColour(TextEditor::outlineColourId,
                                   Colours::transparentBlack);
        this->textEditor.setColour(TextEditor::focusedOutlineColourId,
                                   Colours::transparentBlack);
    }
}


// ------------------------------------------------------------


void IRTextEditorObject::onReturnKeyAction()
{
    //if(this->onReturnKey != nullptr)
     //   this->onReturnKey();
}
void IRTextEditorObject::onTextChangeAction()
{

    if(this->onTextChange != nullptr)
        this->onTextChange();
}

void IRTextEditorObject::textArrangeChanged()
{
    repaint();

    onTextChangeAction();
}

// ------------------------------------------------------------


void IRTextEditorObject::showTextContents()
{
    std::cout << this->textEditor.getText() << std::endl;
}

// ------------------------------------------------------------

int IRTextEditorObject::getCaretPosition()
{
    return this->textEditor.getCaretPosition();
}

void IRTextEditorObject::setCaretPosition(int newIndex)
{
    this->textEditor.setCaretPosition(newIndex);
}
// ------------------------------------------------------------


void IRTextEditorObject::arrangeControllerChangedNotify()
{
    std::cout << "arrangeControllerChangedNotify\n";
    onTextChangeAction();
}
