
#ifndef ObjectListMenu_hpp
#define ObjectListMenu_hpp

#include "JuceHeader.h"

#include "ObjectListMenuObject.hpp"

/*
#include "ColourLib.h"
#include "MenusListBox.h"
#include "IRObjectCreater.hpp"
*/

class ObjectListMenu : public MenuObject
{
public:
    ObjectListMenu(Component* parent, Rectangle<int>frameRect) : MenuObject(parent)
    {
        this->frameRect = frameRect;
        setSize(frameRect.getWidth(), frameRect.getHeight());
        this->menu = new ObjectListMenuObject("Object List", 13);
        setListBoxComponent(this->menu);
    }
    
    ~ObjectListMenu()
    {
        
    }
    
    // ========================================
    void paint (Graphics& g) override
    {
        MenuObject::paint(g);
        //g.fillAll(SYSTEMCOLOUR.contents);
        
        auto area = getLocalBounds().reduced (2);
        
        g.setColour (SYSTEMCOLOUR.contents.brighter());
        g.fillRoundedRectangle (area.toFloat(), 5.0f);
        
    }
    void resized() override
    {
        MenuObject::resized();
    }
    
    void selectedItemChanged(int itemIndex) override
    {
        std::cout << itemIndex << std::endl;
        this->selectedIndex = itemIndex;
        this->selectedID = this->menu->getIdBank()[itemIndex];
        callItemSelectionAction();
    }
    
    void selectedItemDoubleClicked(int itemIndex) override
    {
        std::cout << "double clicked = " <<itemIndex << std::endl;
        this->selectedIndex = itemIndex;
        this->selectedID = this->menu->getIdBank()[itemIndex];
        callItemHasSelectedAction();
        
    }
    void menuScrolled() override
    {
        //repaint();
    }
    int getSelectedIndex() const { return this->selectedIndex; }
    std::string getSelectedId() const { return this->selectedID; }
    // ========================================
    
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        virtual void itemSelectionAction(ObjectListMenu* ) = 0;
        virtual void itemHasSelectedAction(ObjectListMenu* ) = 0;
    };
    
    virtual void addListener(Listener* newListener) { this->listeners.add(newListener); }
    virtual void removeListener(Listener* listener) { this->listeners.remove(listener);}
    
    void callItemSelectionAction()
    {
        Component::BailOutChecker checker(this);
        //==========
        // check if the objects are not deleted, if deleted, return
        if(checker.shouldBailOut()) return;
        this->listeners.callChecked(checker, [this](Listener& l){ l.itemSelectionAction(this); });
        //check again
        if(checker.shouldBailOut()) return;
    }
    
    void callItemHasSelectedAction()
    {
        Component::BailOutChecker checker(this);
        //==========
        // check if the objects are not deleted, if deleted, return
        if(checker.shouldBailOut()) return;
        this->listeners.callChecked(checker, [this](Listener& l){ l.itemHasSelectedAction(this); });
        //check again
        if(checker.shouldBailOut()) return;
    }
    
    // ========================================
    
private:
    ListenerList<Listener> listeners;
    
    int selectedIndex = 0;
    std::string selectedID;
    Rectangle<int> frameRect;
    
    ObjectListMenuObject* menu;
    
    // system appearance
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
};

#endif /* ObjectListMenu_hpp */
