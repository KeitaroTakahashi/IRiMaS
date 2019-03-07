//
//  IRTestObject.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 23/08/2018.
//

#ifndef IRTestObject_h
#define IRTestObject_h

#include "IRNodeObject.hpp"
#include "IRTestUI.h"

class IRTestObject : public IRNodeObject
{
public:
    // ------------------------------------------------------------

    IRTestObject(Component* parent) : IRNodeObject(parent, "IRTest"),
    ui(this)
    {
        setSize(100, 20);
        
        /*
        addAndMakeVisible(&this->playButton);
        this->playButton.setButtonText("button");
        this->playButton.setColour(TextButton::buttonColourId, SYSTEMCOLOUR.contents);
        this->playButton.onClick = [this] {buttonClicked();};
        
        if(isEditMode()) this->playButton.setEnabled(false);
        else this->playButton.setEnabled(true);
        
        childComponentManager(&this->playButton);
         
         */
        
        addAndMakeVisible(&this->ui);
        childComponentManager(&this->ui);
        this->ui.setEditMode(isEditMode());

        
       // this->playButton.addMouseListener(this, true);
        //allowClicks
        //allowClicksOnChildComponents
        //this->playButton.setInterceptsMouseClicks(false, false);
    }
    // ------------------------------------------------------------

    ~IRTestObject() {}
    // ------------------------------------------------------------
    // copy constructor
    
    IRNodeObject* copyThis() override
    {
        return new IRTestObject(this->parent);
    }
    // ------------------------------------------------------------
    void loadThisFromSaveData(t_json saveData) override
    {
        
    }
    // ------------------------------------------------------------

    void buttonClicked()
    {
        std::cout << "bottun clicked\n";
    }
    
    // ------------------------------------------------------------

    void resized() override
    {
       // this->playButton.setBounds(5,5, getWidth()-10, getHeight()-10);
        this->ui.setBounds(getLocalBounds());
    }
    // ------------------------------------------------------------

    void paint(Graphics& g) override
    {
        if(isEditMode())
        {
            auto area = getLocalBounds().reduced (2);
            
            g.setColour (SYSTEMCOLOUR.contents);
            g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
        }
    }
    
    // ------------------------------------------------------------
    // call back function automatically called when the status of this object changed by others.
    // write some tasks here
    void statusChangedCallback(IRNodeComponentStatus status) override
    {
        
        
        switch (status)
        {
            case EditModeStatus:
                if(isEditMode()) this->playButton.setEnabled(false);
                else this->playButton.setEnabled(true);
                
               //this->ui.setEditMode(isEditMode());

                break;
            case SelectableStatus:
                
                break;
            case HasResizedStatus:
                
                break;
            default:
                break;
        }
    }
    // ------------------------------------------------------------

private:
    TextButton playButton;
    
    IRTestUI ui;

};


#endif /* IRTestObject_h */
