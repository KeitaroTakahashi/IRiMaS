//
//  IRMultiPurposeObject.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 27/08/2018.
//

#ifndef IRMultiPurposeObject_hpp
#define IRMultiPurposeObject_hpp

#include "JuceHeader.h"
#include "IRNodeObject.hpp"

class IRMultiPurposeObject : public IRNodeObject
{
    // ============================================================
public:
    IRMultiPurposeObject(Component* parent)
    : IRNodeObject(parent, "IRMultiPurposeObject")
    {
        setSize(100, 20);

    }
    // ------------------------------------------------------------
    ~IRMultiPurposeObject(){}
    // ------------------------------------------------------------
    IRNodeObject* copyThisWithContents() override
    {
        IRMultiPurposeObject* obj = new IRMultiPurposeObject(this->parent);
        obj->setBoundsInRatio(this->xRatio, this->yRatio, this->wRatio, this->hRatio);
        obj->setSelected(this->isSelected());
        obj->setBoundsRatio(this->isBoundsRatio());
        
        return obj;
    }
    // ------------------------------------------------------------
    void resized() override
    {
        if(isBoundsRatio()) renewBounds();
     }
    // ------------------------------------------------------------
    void paint(Graphics& g) override
    {
        if(isSelected()) g.setColour(Colours::lightgreen);
        else g.setColour(Colours::lightgrey);
        g.setOpacity(0.3);
        g.fillAll();
        
    }
    // ------------------------------------------------------------
    
    void setBoundsInRatio(float x, float y, float w, float h)
    {
        this->xRatio = x;
        this->yRatio = y;
        this->wRatio = w;
        this->hRatio = h;
    }
    
    Rectangle<float> getBoundsInRatio() const
    {
        return Rectangle<float> (this->xRatio, this->yRatio, this->wRatio, this->hRatio);
    }
    
    void renewBounds()
    {
        setBoundsRelative(this->xRatio,
                          this->yRatio,
                          this->wRatio,
                          this->hRatio);
    }
    // ------------------------------------------------------------
    bool isBoundsRatio() const { return this->boundsRatioFlag; }
    void setBoundsRatio(bool flag) { this->boundsRatioFlag = flag; }
    // ------------------------------------------------------------
    void mouseDownEvent(const MouseEvent& e) override
    {
        
    }

    void mouseUpEvent(const MouseEvent& e) override
    {

    }
    // ============================================================
    // ------------------------------------------------------------
    // call back function automatically called when the status of this object changed by others.
    // write some tasks here
    
    //# MultiPurpose Object needs the opposite mouseListener structure to the IRNodeComponent
    // # because MultiPurpose Object is added to the IRNodeObject.
    void statusChangedWrapper(IRNodeComponentStatus status) override
    {
        switch (status)
        {
            case EditModeStatus:
                setMovable(false, false, false);
                
                //  ---- here we do not deselect objects ---- setSelected(false);
                
                if(isEditMode())
                {
                    if(isMouseListenerFlag())
                    {
                        removeMouseListener(this->parent);
                        setMouseListenerFlag(false);
                    }
                    for(auto comp : getChildren())
                    {
                        comp->setInterceptsMouseClicks(true, true);
                    }
                }else{
                    if(! isMouseListenerFlag())
                    {
                        addMouseListener(this->parent, true);
                        setMouseListenerFlag(true);
                    }
                    for(auto comp : getChildren())
                    {
                        comp->setInterceptsMouseClicks(false, false);
                    }
                }
                break;
            case SelectableStatus:
                break;
            case HasResizedStatus:
                break;
            default:
                break;
        }
    }
    // ------------------------------------------------------------
private:
    
    // for audio parametter
    double start = 0.0;
    double duration = 0.0;
    
    int startSample = 0;
    int durationSample = 0;
    
    bool boundsRatioFlag = false;
    
    // ratio to its parent
    float xRatio = 0.0;
    float yRatio = 0.0;
    float wRatio = 0.0;
    float hRatio = 0.0;

    
};
#endif /* IRMultiPurposeObject_hpp */
