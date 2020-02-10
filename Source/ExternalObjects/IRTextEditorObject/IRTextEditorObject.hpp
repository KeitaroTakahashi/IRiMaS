
/*
 check belows forum where discussing how to implement different colours for each text fragments.
 https://forum.juce.com/t/need-to-change-text-in-order-for-text-editor-to-change-text-color/18931/2
 
 */



#ifndef IRTextEditorObject_h
#define IRTextEditorObject_h

#include "IRNodeObject.hpp"
#include "IRTextEditorController.hpp"




class IRTextEditorObject : public IRNodeObject,
public ChangeListener
{
    
public:
    
    IRTextEditorObject(Component* parent, IRStr* str);
    ~IRTextEditorObject();
    // ------------------------------------------------------------

    IRNodeObject* copyThis() override;
    
    t_json saveThisToSaveData() override;
    void loadThisFromSaveData(t_json data) override;
    // ------------------------------------------------------------

    void paint(Graphics &g) override;
    void resized() override;
    // ------------------------------------------------------------

    void mouseDownEvent(const MouseEvent& e) override;
    // ------------------------------------------------------------

    void changeListenerCallback(ChangeBroadcaster* source) override;
    // ------------------------------------------------------------

    void setFont(Font font);
    void setAlign(int id);
    void setTextColour(Colour colour);
    void setBackgroundColour(Colour colour);
    // ------------------------------------------------------------

    Font getFont() const;
    Colour getTextColour() const;
    Colour getBackgroundColour() const;
    int getAlignId() const;
    
    int getTextWidth() const;
    int getTextHeight() const;
    
    void showTextContents();
    // ------------------------------------------------------------

    void onReturnKeyAction();
    void onTextChangeAction();
    
    std::function<void()> onReturnKey;
    std::function<void()> onTextChange;
    // ------------------------------------------------------------

    TextEditor textEditor;
    // ------------------------------------------------------------

    // preference
    std::unique_ptr<IRTextEditorController> controller;
    
    // ------------------------------------------------------------

    void statusInEditMode();
    void statusInControlMode();
private:
    
    // ------------------------------------------------------------
    // call back function automatically called when the status of this object changed by others.
    // write some tasks here
    void statusChangedCallback(IRNodeComponentStatus status) override;
    // ------------------------------------------------------------

    
    Font font;
    Colour textColour;
    Colour backgroundColour;
    int alignId;
    
    // ------------------------------------------------------------

    // ------------------------------------------------------------
    // ------------------------------------------------------------

    
};




#endif /* IRTextEditorObject_h */




