//
//  NodeComponent.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 20/08/2018.
//

#include "IRNodeComponent.hpp"

IRNodeComponent::IRNodeComponent(Component* parent, String name, NodeObjectType objectType) :
resizingArea(25,25)
{
    this->objectType = objectType;
    //default size
    setSize(100, 100);
    this->name = name;
    this->mixer = new MixerAudioSource();
    
    this->parent = parent;
    
    this->menu.addItem(999, name); // do nothing
    this->menu.addSeparator();
    this->menu.addItem(1, "Bring to Front");
    this->menu.addItem(2, "Send to Back");
    this->menu.addSeparator();
    this->menu.addItem(3, "Cut");
    this->menu.addItem(4, "Copy");
    this->menu.addItem(5, "Paste");
    this->menu.addItem(6, "Duplicate");
    
    
    

}

// --------------------------------------------------
IRNodeComponent::~IRNodeComponent()
{
    
}

// --------------------------------------------------


// ===========================================================================
// basics
void IRNodeComponent::resized()
{
    //this->constrainer.setMinimumOnscreenAmounts(getHeight(), getWidth(),
      //                                          getHeight(), getWidth());
}

void IRNodeComponent::setSize(float width, float height)
{
    statusChangedWrapper(IRNodeComponentStatus::WillResizeStatus);

    
    float w = width > this->maxWidth ? this->maxWidth : width;
    float h = height > this->maxHeight ? this->maxHeight : height;
    
    
    w = w >= this->minWidth ? w : this->minWidth;
    h = h >= this->minHeight ? h : this->minHeight;
    
    float x = getX();
    float y = getY();
    
    setBounds(x, y, w, h);
    
    statusChangedWrapper(IRNodeComponentStatus::HasResizedStatus);
}
// --------------------------------------------------
// ===========================================================================
// paint
void IRNodeComponent::paint(Graphics& g)
{
    auto area = getLocalBounds();//.reduced (0);

    if(isEditMode())
    {
        g.setColour (SYSTEMCOLOUR.contents);
        g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
    }
    
    //g.setColour (this->isSelected()? Colours::white : SYSTEMCOLOUR.titleText);
    //g.drawFittedText ("NodeObject", area, Justification::centred, 1);
    
}
// ===========================================================================
// Child Component Management
void IRNodeComponent::childComponentManager(Component* comp)
{
    comp->addMouseListener(this, true);
    if(isEditMode()) comp->setInterceptsMouseClicks(false, false);
    else comp->setInterceptsMouseClicks(true, false);
}
// ===========================================================================
// Audio source
void IRNodeComponent::addAudioComponent(AudioSource *source)
{
    this->mixer->addInputSource(source, true);
    this->containAudioSourceFlag = true;
}

// ===========================================================================
// interaction
void IRNodeComponent::mouseDown(const MouseEvent& e)
{
    std::cout << "IRNodeComponent mouseDown\n";
    mouseDownNodeEvent(e);
    mouseDownEvent(e);
}
// ---------------------------------------------------------------------------
void IRNodeComponent::mouseMove(const MouseEvent& e)
{
    mouseUpNodeEvent(e);
    mouseMoveEvent(e);
}
// ---------------------------------------------------------------------------
void IRNodeComponent::mouseUp(const MouseEvent& e)
{
    mouseUpNodeEvent(e);
    mouseUpEvent(e);
}
// ---------------------------------------------------------------------------
void IRNodeComponent::mouseDoubleClick(const MouseEvent& e)
{
    mouseDoubleClickNodeEvent(e);
    mouseDoubleClickEvent(e);
}
// ---------------------------------------------------------------------------
void IRNodeComponent::mouseDrag(const MouseEvent& e)
{
    mouseDragNodeEvent(e); // defines fundamental behavior
    mouseDragEvent(e); // defined by an unique Node object
}
// ---------------------------------------------------------------------------

void IRNodeComponent::statusChangedWrapper(IRNodeComponentStatus status)
{
    switch (status)
    {
        case EditModeStatus:
            // cancel selection
            setSelected(false);
            if(isEditMode())
            {
                if(! this->mouseListenerFlag)
                {
                    addMouseListener(this->parent, true);
                    this->mouseListenerFlag = true;
                }
                
                for(auto comp : getChildren())
                {
                    comp->setInterceptsMouseClicks(false, false);
                }
            }else{
                
                if(this->mouseListenerFlag)
                {
                    removeMouseListener(this->parent);
                    this->mouseListenerFlag = false;
                }
                
                
                for(auto comp : getChildren())
                {
                    comp->setInterceptsMouseClicks(true, true);
                }
            }
            
            break;
        default:
            break;
    }
    
    // call statusChangeCallback
    statusChangedCallback(status);
}

// ---------------------------------------------------------------------------
void param_register(std::string id, int data)
{
    t_json save = t_json::object({
        {id, data}
    });
}
