//
//  IRResizeSquare.h
//  IRiMaS
//
//  Created by Keitaro on 27/11/2019.
//

#ifndef IRResizeSquare_h
#define IRResizeSquare_h

#include "JuceHeader.h"

class IRResizeSquare : public Component
{
public:
    
    IRResizeSquare(Component* resizedComponent) :
    resizedComponent(resizedComponent)
    {
        createItems();
    }
    
    ~IRResizeSquare()
    {
        for(auto item : this->items)
        {
            delete item;
        }
        this->items.clear();
    }
    
    // ==================================================
    
    void resized() override
    {
        auto t = this->items[0];
        t->setCentrePosition(t->getPosX(), t->getPosY());
        t = this->items[1];
        t->setCentrePosition(t->getPosX(), t->getPosY());
        t = this->items[2];
        t->setCentrePosition(t->getPosX(), t->getPosY());
        t = this->items[3];
        t->setCentrePosition(t->getPosX(), t->getPosY());
    }
    
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::transparentBlack);
    }
    
    // ==================================================
    void createItems()
    {
        
        this->items.push_back(new ResizingItem(Rectangle<int>(0, 0, 20, 20), this));
        this->items.push_back(new ResizingItem(Rectangle<int>(getWidth(), 0, 20, 20), this));
        this->items.push_back(new ResizingItem(Rectangle<int>(getWidth(), getHeight(), 20, 20), this));
        this->items.push_back(new ResizingItem(Rectangle<int>(0, getHeight(), 20, 20), this));

    }
    // ==================================================
    // ==================================================

private:
    // ==================================================

    class ResizingItem : public Component
    {
    public:
        ResizingItem(Rectangle<int> frameRect, IRResizeSquare* owner) :
        owner(owner),
        frameRect(frameRect)
        {
            setSize(frameRect.getWidth(), frameRect.getHeight());
        }
        
        ~ResizingItem() {}
        // ==================================================

        void paint(Graphics& g) override
        {
            g.fillAll(Colours::transparentBlack);
            g.setColour(Colours::black);
            g.drawRect(getLocalBounds());
        }
        // ==================================================

        void mouseDown(const MouseEvent& e) override
        {
            this->owner->itemClicked(this);
        }
        
        // ==================================================

        int getPosX() const { return this->frameRect.getX(); }
        int getPosY() const { return this->frameRect.getY(); }

    private:
        IRResizeSquare* owner;
        Rectangle<int> frameRect;
    };
    // ==================================================

    // ==================================================

    void itemClicked(ResizingItem* item)
    {
           
    }
    // ==================================================

    std::vector<ResizingItem* > items;
    
    Component* resizedComponent = nullptr;
};
#endif /* IRResizeSquare_h */
