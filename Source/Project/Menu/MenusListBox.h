//
//  MenusListBox.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 16/01/2019.
//

#ifndef MenusListBox_h
#define MenusListBox_h



#include "../JuceLibraryCode/JuceHeader.h"
#include "../Utilities/Colour/ColourLib.h"
#include "../ListBoxObject/IRListBoxObject.hpp"

enum MenuObjectStatus
{
    MenuOpened,
    MenuClosed,
    MenuComplete
};

class MenuObject : public Component,
public ChangeListener,
public ChangeBroadcaster
{
public:
    MenuObject(Component* parent)
    {
        
        int w = getWidth();
        int h = getHeight();
        
        
        this->parent = parent;
        /*
        this->closeButton = new BackForwardButton(closeTypeButton, "close Button");
        this->closeButton->setBounds(10,10,40,40);
        addAndMakeVisible(this->closeButton);
        this->closeButton->onClick = [this]{ closeMenu(); };
        
        //menu ok button
        this->menuOKButton.setButtonText("DONE");
        this->menuOKButton.setSize(80,40);
        this->menuOKButton.setCentrePosition(w - 60, h - 30);
        this->menuOKButton.addListener(this);
        this->menuOKButton.setColour(TextButton::buttonColourId, Colours::transparentWhite);
        this->menuOKButton.setColour(TextButton::buttonOnColourId, Colours::transparentWhite);
        this->menuOKButton.setColour(TextButton::textColourOffId, Colours::black);
        this->menuOKButton.setColour(TextButton::textColourOnId, Colours::black);
        this->menuOKButton.onClick = [this]{ completeMenu(); };
        */
        
        
    }
    
    ~MenuObject()
    {
        
       // if(this->closeButton != nullptr)
        //    delete this->closeButton;
    }
    
    // ========================================
    void paint (Graphics& g) override
    {
       
        
    }
    void resized() override
    {
        int w = getWidth();
        int h = getHeight();
        //this->menuOKButton.setCentrePosition(w - 60, h - 30);
        
    }
    // ========================================
    
    virtual void selectedItemChanged(int itemIndex) = 0;
    virtual void selectedItemDoubleClicked(int itemIndex) = 0;
    virtual void menuScrolled() {}
    
  
    // ========================================
    
    void showWindow()
    {
        this->parent->addAndMakeVisible(this);
        //this->parent->addAndMakeVisible(this->menuOKButton);
        
        setBounds(0,0,this->parent->getWidth(),this->parent->getHeight());
    }
    
    // ========================================
    // when X button pressed and menu selection interrupted.
    void closeWindow()
    {
        this->parent->removeChildComponent(this);
        //this->parent->removeChildComponent(&this->menuOKButton);
        this->status = MenuObjectStatus::MenuClosed;
        sendChangeMessage();
    }
    // when Done button pressed and menu selection completed.
    void completeSelection()
    {
        this->parent->removeChildComponent(this);
        //this->parent->removeChildComponent(&this->menuOKButton);
        this->status = MenuObjectStatus::MenuComplete;
        sendChangeMessage();
    }
    
    // ========================================
    
    void setListBoxComponent(IRTableListBox* menu)
    {
        this->ListMenu = menu;
        this->ListMenu->addChangeListener(this);
        
        this->ListMenu->setBounds(0,30,getWidth(),getHeight()-60);
        this->ListMenu->makeTableContents();
        addAndMakeVisible(this->ListMenu);
    }
    // ========================================
    
    IRTableListBox* getListMenu() { return this->ListMenu; }
    
    MenuObjectStatus getStatus() const { return this->status; }
    
    
private:
    
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        if(source == this->ListMenu)
        {
            switch (this->ListMenu->getStatus())
            {
                case IRTableListBoxStatus::selectedRowChanged:
                    std::cout << "IRTableListBoxStatus :: selectedRowChanged to " << this->ListMenu->getSelectedIndex() << std::endl;
                    
                    selectedItemChanged(this->ListMenu->getSelectedIndex());
                    break;
                case IRTableListBoxStatus::selectedColumnChanged:
                    break;
                case IRTableListBoxStatus::scrolled:
                    break;
                case IRTableListBoxStatus::selectedRowDoubleClicked:
                    selectedItemDoubleClicked(this->ListMenu->getSelectedIndex());
                    break;
                default:
                    break;
                    
                    
            }
        }
    }
    
    
    Component* parent;
    
    IRTableListBox* ListMenu = nullptr;
    
    MenuObjectStatus status;
    
    /*
    BackForwardButton *closeButton = nullptr;
    
    TextButton menuOKButton;

    Image backgroundImage;
    bool isBackgroundImage = false;
    */
public:
    
    //std::vector<ListTypeMenuCellComponent* > cells;
    
    
    
};


#endif /* MenusListBox_h */
