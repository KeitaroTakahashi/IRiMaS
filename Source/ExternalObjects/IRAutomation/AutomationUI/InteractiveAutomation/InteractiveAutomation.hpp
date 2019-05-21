//
//  InteractiveAutomation.hpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 29/04/2019.
//

#ifndef InteractiveAutomation_hpp
#define InteractiveAutomation_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include <algorithm>

#include "IRUIFoundation.hpp"
#include "AutomationVertex.hpp"
#include "VertecesSelector.hpp"
#include "AutomationController.hpp"

using namespace IRAutomation;


class InteractiveAutomation : public IRUIFoundation
{
public:
    
    // ==================================================
    
    InteractiveAutomation(IRNodeObject* nodeObject);
    ~InteractiveAutomation();
    // ==================================================
    
    void paint(Graphics& g) override;
    void resized() override;
    // ==================================================
    
    void mouseDown(const MouseEvent& e) override;
    void mouseDoubleClick(const MouseEvent& e) override;
    
    void mouseUp(const MouseEvent& e) override;
    void mouseMove(const MouseEvent& e) override;
    void mouseDrag(const MouseEvent& e) override;
    
    // ==================================================
    void selectAllVerteces();
    void deselectAllVerteces();
    void deleteSelectedVerteces();
    void deleteAllVerteces();
    void repaintAllverteces();
    // ==================================================
    
    bool isHitAnyVerteces(Rectangle<float> area) const;
    
    void showVerteces()
    {
        std::cout << " ========== \n";
        int index = 0;
        for(auto v : this->verteces)
        {
            std::cout << "verteces[" << index <<"] : " << v->getX() << ", " << v->getY() << std::endl;
            index++;
        }
        std::cout << " ========== \n";
        
    }
    
    // ==================================================
    
    void setMovable(bool movable, bool verticalMovable, bool horizontalMovable);
    
    void reCalcPos();
    
    // --------------------------------------------------
    Array<vertex* > getVerteces() { return this->verteces; }
    
    
    // --------------------------------------------------
    // ==================================================
    
    void zoomInClicked();
    void zoomOutClicked();
    void movableClicked(IRAutomation::movableStatus status);
    
    std::function<void()>zoomInClickedCallback;
    std::function<void()>zoomOutClickedCallback;
    std::function<void(IRAutomation::movableStatus status)>movableClickedCallback;
    // ==================================================
    
    // test
    void demoData(int num);
    
    // ==================================================
    
    
private:
    
    class ascendingSorter
    {
    public:
        static int compareElements (const vertex* first, const vertex* second)
        {
            if (first->getX() < second->getX())
                return -1;
            if (first->getX() > second->getX())
                return 1;
            return 0;
        }
    };
    
    std::shared_ptr<AutomationController> controller;
    
    
    float automation_width_ratio = 1.0;
    float automation_height_ratio = 1.0;
    
    Array<vertex* > dataBuffer;
    Array<vertex* > verteces;
    Array<vertex* > selectedVerteces;
    
    float MaxVertexValue = -999999;
    float MinVertexValue = 999999;
    
    Rectangle<int> previousBounds;
    
    std::shared_ptr<VertecesSelector> selector;
    
    void createVertex(Point<int> pos, bool isSelected);
    
    void drawVerteces(Graphics& g);
    
    void drawLinesBetweenVerteces(Graphics& g);
    
    // ==================================================
    
    Point<int> currentMousePos {0, 0};
    
    
    // ==================================================
    
    
    
    // ==================================================
    
    //bool keyPressed(const KeyPress &key,
      //              Component* originatingComponent) override;
    void IRKeyPressed(int keyCode,String keyText) override;
    // ==================================================
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(InteractiveAutomation)
    
    
};
#endif /* InteractiveAutomation_hpp */
