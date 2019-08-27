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



class ObjectMenuComponent : public Component,
private KeAnimationComponent
{
public:
    ObjectMenuComponent(Rectangle<int> frameRect);
    ~ObjectMenuComponent();
    // ==================================================
    
    void paint(Graphics& g) override;
    void resized() override;
    // ==================================================
    
    void setTitleText(String text);
    // ==================================================
   
    // ==================================================

    class ObjectMenuItem : public Component
    {
    public:
        ObjectMenuItem(ObjectMenuComponent* parent, IRObjectFactory::t_object* object, int buttonSize) :
        parent(parent), object(object), buttonSize(buttonSize)
        {
            
        }
        ~ObjectMenuItem() {}
        
        void paint(Graphics& g) override
        {
            if(this->isSelectedFlag) g.fillAll(SYSTEMCOLOUR.contents.brighter());
            
            g.setColour(SYSTEMCOLOUR.text);
            g.drawEllipse(5, 5, this->buttonSize, this->buttonSize, 2);
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
            setSelected(false);
        }
        
        IRObjectFactory::t_object* getObject() const { return this->object; }
        
        bool isSelected() const { return this->isSelectedFlag; }
        void setSelected(bool flag) { this->isSelectedFlag = flag; repaint(); }
        
    private:
        ObjectMenuComponent* parent;
        IRObjectFactory::t_object* object;
        String title;
        Image img;
        int buttonSize;
        
        bool isSelectedFlag = false;

        IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    };
    
    // ==================================================

    void createItem(IRObjectFactory::t_object* obj);
    
    virtual void itemSelected(IRObjectFactory::t_object* obj) {}
    
private:
    void updateAnimationFrame() override;
    // ==================================================

    
    void ObjectMenuItemClicked(ObjectMenuItem* item);
    
    

    // ==================================================

    Rectangle<int> frameRect;

    // ==================================================
    Label title;
    int buttonSize = 40;
    int xMarge = 5;
    int yMarge = 5;

    // ==================================================
    std::vector<IRObjectFactory::t_object* > objects;
    std::vector<ObjectMenuItem* > items;
    // ==================================================

    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    
public:
    // IRObjectFactory
    IRObjectFactory& IRFactory = singleton<IRObjectFactory>::get_instance();
    
};
#endif /* ObjectMenuComponent_hpp */
