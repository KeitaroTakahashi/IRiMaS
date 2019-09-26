//
//  ObjectMenuComponent.hpp
//  IRiMaS
//
//  Created by Keitaro on 11/08/2019.
//

#ifndef ObjectMenuComponent_hpp
#define ObjectMenuComponent_hpp

#include "JuceHeader.h"
#include "ColourLib.h"
#include "KeAnimationComponent.h"
#include "ExternalObjectHeader.h"
#include "IRStrComponent.hpp"
#include "IRWorkspace2.hpp"

class ObjectMenuComponent : public Component,
public IRStrComponent,
private KeAnimationComponent
{
public:
    ObjectMenuComponent(IRStr* str, Rectangle<int> frameRect);
    ~ObjectMenuComponent();
    // ==================================================
    
    void paint(Graphics& g) override;
    void resized() override;
    // ==================================================
    
    void setTitleText(String text);
    // ==================================================
    
    bool isMenuOpened() const { return this->isOpened; }
    void setMenuOpened(bool flag) { this->isOpened = flag; }
   
    // ==================================================

    class ObjectMenuItem : public Component
    {
    public:
        ObjectMenuItem(ObjectMenuComponent* parent, IRObjectFactory2::t_object* object, int buttonSize) :
        parent(parent), object(object), buttonSize(buttonSize)
        {
            
        }
        ~ObjectMenuItem() {}
        
        void paint(Graphics& g) override
        {
            if(this->isSelectedFlag) g.fillAll(SYSTEMCOLOUR.contents.brighter());
            
            g.setColour(SYSTEMCOLOUR.text);
            g.drawEllipse(5, 5, this->buttonSize, this->buttonSize, 1);
            int s = this->buttonSize + 15;
            g.drawText(this->object->name, s, 0, getWidth() - s, getHeight(), Justification::left);
        }
        void resized() override
        {
            repaint();
        }
        
        void mouseDown(const MouseEvent &e) override
        {
            this->parent->ObjectMenuItemClicked(this);
            setSelected(true);
        }
        void mouseUp(const MouseEvent &e) override
        {
            this->parent->ObjectMenuItemMouseUp(this);
            setSelected(false);
        }
        
        IRObjectFactory2::t_object* getObject() const { return this->object; }
        
        bool isSelected() const { return this->isSelectedFlag; }
        void setSelected(bool flag) { this->isSelectedFlag = flag; repaint(); }
        
    private:
        ObjectMenuComponent* parent;
        IRObjectFactory2::t_object* object;
        String title;
        Image img;
        int buttonSize;
        
        bool isSelectedFlag = false;

        IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    };
    
    // ==================================================

    void createItem(IRObjectFactory2::t_object* obj);
    
    virtual void itemSelected(IRObjectFactory2::t_object* obj) {}
    virtual void itemReleased(IRObjectFactory2::t_object* obj) {}
    
private:
    void updateAnimationFrame() override;
    // ==================================================

    // called when mouse down on an item
    void ObjectMenuItemClicked(ObjectMenuItem* item);
    // called when mouse up on an item
    void ObjectMenuItemMouseUp(ObjectMenuItem* item);
    
    bool isOpened = false; // if menu area is visible or not
    

    // ==================================================

    Rectangle<int> frameRect;

    // ==================================================
    Label title;
    int buttonSize = 40;
    int xMarge = 5;
    int yMarge = 5;

    // ==================================================
    std::vector<IRObjectFactory2::t_object* > objects;
    std::vector<ObjectMenuItem* > items;
    // ==================================================
    
};
#endif /* ObjectMenuComponent_hpp */
