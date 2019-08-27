//
//  IRWindowComponent.hpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#ifndef IRWindowComponent_hpp
#define IRWindowComponent_hpp


#include "JuceHeader.h"
#include "IRTitleBar.hpp"
#include "IRLeftBar.hpp"

class IRWindowComponent : public Component
{
public:
    
    IRWindowComponent()
    {
        
    }
    ~IRWindowComponent()
    {
        
    }
    // ==================================================
    
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::white);
        
        
    }
    
    void resized() override
    {
        int w = getWidth();
        int h = getHeight();
        
        this->mainComponentHeight = h - this->barHeight;
        
        if(this->hasBar)
            this->bar->setBounds(0, 0, w, this->barHeight);
        if(this->hasMainComp)
            this->mainComp->setBounds(0, this->barHeight,
                                      w, this->mainComponentHeight);
        
        if(this->hasLeftBar)
            this->leftBar->setBounds(0, this->barHeight,
                                     this->leftBarWidth, h - this->barHeight);
    }
    // ==================================================
    
    void mouseDrag(const MouseEvent& e) override
    {
        auto pos = e.getPosition();
        
        if(this->isResizable)
        {
            Point<int> delta = pos - this->prevPos;
            
            int newW = getWidth() + delta.getX();
            int newH = getHeight() + delta.getY();
            if(newW < this->minWidth) newW = this->minWidth;
            if(newH < this->minHeight) newH = this->minHeight;
            
            setSize(newW, newH);
            
            this->prevPos = pos;
        }
        
    }
    void mouseUp(const MouseEvent& e) override
    {
        
    }
    void mouseDown(const MouseEvent& e)override
    {
        auto pos = e.getPosition();
        this->prevPos = pos;
        
        std::cout << pos.getX() << ", " << pos.getY() << std::endl;
        
        if(pos.getX() > getWidth() - this->resizableMargin &&
           pos.getY() > getHeight() - this->resizableMargin)
        {
            this->isResizable = true;
        }else this->isResizable = false;
    }
    void mouseMove(const MouseEvent& e)override
    {
        
    }
    // ==================================================
    
    void setComponentsHeight(int barHeight, int mainHeight)
    {
        this->barHeight = barHeight;
        this->mainComponentHeight = mainHeight;
    }
    
    
    // ==================================================
    void setWindowBar(IRTitleBar* bar)
    {
        this->bar = bar;
        this->hasBar = true;
        
        addAndMakeVisible(this->bar);
    }
    
    void removeWinowBar()
    {
        removeChildComponent(this->bar);
        this->hasBar = false;
    }
    
    void setMainComponent(Component* mainComp)
    {
        this->mainComp = mainComp;
        this->hasMainComp = true;
        
        addAndMakeVisible(this->mainComp);
    }
    
    void removeMainComponent()
    {
        removeChildComponent(this->mainComp);
        this->hasMainComp = false;
    }
    
    void setLeftBarComponent(IRLeftBar* leftBar)
    {
        this->leftBar = leftBar;
        this->hasLeftBar = true;
        
        addAndMakeVisible(this->leftBar);
        
    }
    
    void removeLeftBar()
    {
        removeChildComponent(this->leftBar);
        this->hasLeftBar = false;
    }
    
    // ==================================================
    Point<int> pos;
    Point<int> currentPos;
    Point<int> prevPos;
    
    bool isResizable = false;
    int resizableMargin = 20;
private:
    // ==================================================
    
    int barHeight = 50;
    int leftBarWidth = 70;
    int mainComponentHeight = 400;
    
    bool hasBar = false;
    IRTitleBar* bar;
    
    IRLeftBar* leftBar = nullptr;
    bool hasLeftBar = false;
    bool hasMainComp = false;
    Component* mainComp;
    
    // ==================================================
    
    // define the minimum size of the window
    int minWidth = 400;
    int minHeight = 700;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRWindowComponent)
    
};
#endif /* IRWindowComponent_hpp */
