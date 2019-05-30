//
//  AutomationVertex.hpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 01/05/2019.
//

#ifndef AutomationVertex_hpp
#define AutomationVertex_hpp

#include "JuceHeader.h"


namespace IRAutomation {
    
    enum uiCondition
    {
        uiOrdinary,
        uiSelected
    };
    
    
    enum movableStatus
    {
        NOTMOVABLE,
        HORIZONTALMOVABLE,
        VERTICALMOVABLE,
        CROSSMOVABLE
    };
    
    enum lineStatus
    {
        LINEAR,
        BEZIER
    };
    
    
    class vertex : public Component
    {
        
    public:
        // --------------------------------------------------
        
        vertex(Component* parent);
        ~vertex();
        
        // --------------------------------------------------
        
        void paint(Graphics& g) override;
        
        void resized() override;
        // must be implemented in resized() method in the parent
        void parentResized();
        
        void mouseDownNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
        void mouseMoveNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
        void mouseDoubleClickNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
        void mouseUpNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
        void mouseDragNodeEvent(const MouseEvent& e); // make another method to avoid spaghetti codes
        void recoverEventStatus();
        
        void mouseDown(const MouseEvent& e) override; // JUCE oriented
        void mouseMove(const MouseEvent& e) override; // JUCE oriented
        void mouseUp(const MouseEvent& e)override; // JUCE oriented
        void mouseDoubleClick(const MouseEvent& e) override; // JUCE oriented
        void mouseDrag(const MouseEvent& e) override; // JUCE oriented
        
        // --------------------------------------------------

        bool hitTest(Rectangle<float> area) const;
        
        // --------------------------------------------------
        
        void setSelected(bool flag);
        bool isSelected();
        void setMouseDownPos(Point<int> pos);
        Point<int> getMouseDownPos() const;
        void moveThisComponentEvent(const MouseEvent& e);
        
        // --------------------------------------------------
        
        uiCondition getCondition() const { return this->condition; }
        
        // --------------------------------------------------
        // operator for sorting vertex
        bool operator < (const vertex& rhs) const {
            return getX() < rhs.getX();
        }
        // utility
        void showRect()
        {
            std::cout << getX() << ", " << getY() << ", " << getWidth() << ", " << getHeight() << std::endl;
        }
        
        // --------------------------------------------------
        
        // change status
        // return a flag shows whether this Component is movable or not. Default is true.
        bool isMovable() const;
        bool isVerticalMovable() const;
        bool isHorizontalMovable() const;
        // all setter
        void setMovable(bool movable, bool verticalMovable, bool horizontalMovable);
        bool isMoving() const;
        bool isDragging() const;
        
        bool isPoint() const { return this->isPointFlag; }
        void setPoint(bool p) { this->isPointFlag = p; }
        // --------------------------------------------------
        
        
        void setPosition(Point<float>pos);
        void setPosition(Point<int>pos);

        Point<float> getPosition();
        
        void setPositionEx(Point<float>pos);
        
        void setBezierRatio(float ratio) { this->bezierRatio = ratio; }
        float getBezierRatio() const { return this->bezierRatio; }
        // --------------------------------------------------
        
        void calcBezier(vertex* previous);
        void setBezier(vertex* a, vertex* b)
        {
            this->bezier1 = a;
            this->bezier2 = b;
        }
    
    private:
        
        Point<float> position;
        
        ComponentBoundsConstrainer constrainer;
        ComponentDragger dragger;
        bool draggingFlag = false;
        
        // --------------------------------------------------
        
        bool movingFlag = false;
        bool isMovableFlag = true;
        bool isVerticalMovableFlag = true;
        bool isHorizontalMovableFlag = true;
        
        bool isPointFlag = true;

        // --------------------------------------------------

        uiCondition condition = uiOrdinary;
        Point<int> mouseDownPos = {0, 0};
        int selectedSize = 10;
        int ordinarySize = 5;
        
        // --------------------------------------------------
        
        vertex* bezier1 = nullptr;
        vertex* bezier2 = nullptr;
        // in case this object is used for bezier points.
        float bezierRatio = 0.5;
        // --------------------------------------------------

    };
}
#endif /* AutomationVertex_hpp */
