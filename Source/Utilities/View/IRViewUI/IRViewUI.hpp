//
//  IRViewUI.hpp
//  IRiMaS
//
//  Created by Keitaro on 12/07/2019.
//

#ifndef IRViewUI_hpp
#define IRViewUI_hpp

#include "IRiMaSHeader.h"
#include "IRUIFoundation.hpp"
#include "IRMeasureGrid.hpp"

class IRViewUI : public Component
{
public:
    IRViewUI(Component* main,
             float vmin, float vmax,
             float hmin, float hmax);
    ~IRViewUI();
    
    // ==================================================
    virtual void zoomInClicked() {};
    virtual void zoomOutClicked() {};
    // ==================================================

private:
    // ==================================================

    int gridSize = 30;
    IRMeasureGrid verticalGrid;
    IRMeasureGrid horizontalGrid;
    // ==================================================

    class Component4ViewPort : public Component
    {
    public:
          Component4ViewPort(Component* main,
                             IRMeasureGrid* vertical,
                             IRMeasureGrid* horizontal,
                             int gridSize) :
        main(main),
        vertical(vertical),
        horizontal(horizontal),
        gridSize(gridSize)
        {
            addAndMakeVisible(main);
            addAndMakeVisible(vertical);
            addAndMakeVisible(horizontal);
        }
        
        ~Component4ViewPort() {}
        
        void resized() override
        {
            this->main->setBounds(this->gridSize,
                                  this->gridSize,
                                  getWidth() - this->gridSize,
                                  getHeight() - this->gridSize);
            this->vertical->setBounds(0,
                                      this->gridSize,
                                      this->gridSize,
                                      getHeight() - this->gridSize);
            this->horizontal->setBounds(this->gridSize,
                                        0,
                                        getWidth() - this->gridSize,
                                        this->gridSize);
        }
        
        void setVisibleArea(Rectangle<int> area)
        {
            this->vertical->setBounds(area.getX(),
                                      this->gridSize,
                                      this->gridSize,
                                      getHeight() - this->gridSize);
        }
        
    private:
        Component* main;
        IRMeasureGrid* vertical;
        IRMeasureGrid* horizontal;
        
        int gridSize;
        
    };
    
    std::shared_ptr<Component4ViewPort> viewPort;
    
    // ==================================================
    // ==================================================
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRViewUI)

};


#endif /* IRViewUI_hpp */
