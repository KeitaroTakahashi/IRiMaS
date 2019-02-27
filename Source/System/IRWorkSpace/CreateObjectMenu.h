//
//  CreateObjectMenu.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 16/01/2019.
//

#ifndef CreateObjectMenu_h
#define CreateObjectMenu_h

#include "JuceHeader.h"
#include "ColourLib.h"
#include "MenusListBox.h"
#include "IRObjectCreater.hpp"

class ObjectListMenuObject : public IRTableListBox
{
public:
    ObjectListMenuObject(String title, int rowNumber):IRTableListBox(title, rowNumber)
    {
        makeCells();
        setNumRows((int)this->cells.size());
    }
    
    ~ObjectListMenuObject()
    {
        for(auto item : this->cells)
        {
            delete item;
        }
    }
    
    Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override
    {
        if(columnId == 1){
            if(existingComponentToUpdate != nullptr){
                delete existingComponentToUpdate;
            }
            auto* cell = this->cells[rowNumber]->copy();
            return cell;
        }
        
        jassert(existingComponentToUpdate == nullptr);
        return nullptr;
    }
    // ==================================================
    
    void makeCells()
    {
        
        this->cells.clear();
        
        std::map<std::string, IRObjectFactory::t_object> list = IRFactory.getRegisteredObjectList();
    
        int index = 1;
        this->idBank.push_back(""); // dummy
        for(auto &obj : list)
        {
            cellComponent* cell = new cellComponent(obj.second.name,
                                                    obj.second.id,
                                                    index,
                                                    *this);
            this->cells.push_back(cell);
            this->idBank.push_back(obj.second.id);
            
            index++;
        }
    }
    
    std::vector<std::string> getIdBank() const { return this->idBank; }

private:
    class cellComponent : public Component
    {
    public:
        cellComponent(String titleText, std::string id, int index, IRTableListBox& td) :
        owner(td)
        {
            this->titleText = titleText;
            this->index = index;
            this->id = id;
        }
        
        ~cellComponent()
        {
        }
        
        cellComponent* copy()
        {
            cellComponent* comp = new cellComponent(this->titleText,
                                                    this->id,
                                                    this->index,
                                                    this->owner);
            return comp;
        }
        
        // ==================================================
        
        void paint(Graphics& g) override
        {
            
            //int p_x = getWidth() / 2 - 50;
            int p_y = (getHeight() - 100) / 2;
            
            g.setColour(SYSTEMCOLOUR.contents);
            
            g.fillEllipse(10, p_y, 100, 100);
            
            Font f("Avenir Next",20, Font::plain);
            g.setColour(SYSTEMCOLOUR.text);
            g.setFont(f);
            g.drawText(this->titleText, 0, p_y + 5, getWidth()-20, 100, Justification::right,true);
            
        }
        // ==================================================
        
        void mouseDown(const MouseEvent& e) override
        {
            this->owner.table.selectRowsBasedOnModifierKeys(this->index-1, e.mods, false);
            Component::mouseDown(e);
        }
        
        void mouseDoubleClick(const MouseEvent& e)override
        {
            printf("Item double clicked!!\n");
            this->owner.selectedRowDoubleClicked(this->index);
        }
        
        int getIndex() const { return this->index; }
        
        
        // ==================================================

    private:

        IRTableListBox& owner;
        int index;
        String titleText;
        std::string id;
        
        
        // system appearance
        IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(cellComponent);
    };
    
private:
    std::vector<cellComponent* > cells;
    std::vector<std::string> idBank;

    // IRObjectFactory
    IRObjectFactory& IRFactory = singleton<IRObjectFactory>::get_instance();

};


// ==================================================
// ==================================================

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
/*
 
    ObjectMenu appears as window
 
 */

class ObjectMenuWindow : public DocumentWindow
{
public:
    ObjectMenuWindow(String name, Rectangle<int> frameRect, ObjectListMenu* menu) :
    DocumentWindow (name,
                    Desktop::getInstance().getDefaultLookAndFeel()
                    .findColour (ResizableWindow::backgroundColourId),
                    DocumentWindow::allButtons)
    {
        // do not show a title bar
        setUsingNativeTitleBar (false);
        setTitleBarHeight(0);
        setBounds(frameRect);
        setVisible(true);
        // do not deallocate menu object here.
        setContentNonOwned(menu,true);
        
    }
    ~ObjectMenuWindow()
    {
        
    }
    
private:
    
    // setup systme colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ObjectMenuWindow)
};

#endif /* CreateObjectMenu_h */
