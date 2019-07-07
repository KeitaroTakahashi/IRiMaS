//
//  IRSpectrogramObject.cpp
//  IRiMaS
//
//  Created by Keitaro on 26/06/2019.
//

#include "IRSpectrogramObject.hpp"

IRSpectrogramObject::IRSpectrogramObject(Component* parent) :
IRNodeObject(parent, "IRSpectrogram")
{
    this->spectrogram = std::make_shared<IRSpectrogram>(this);
    addAndMakeVisible(this->spectrogram.get());
    
    childComponentManager(this->spectrogram.get());

    setSize(500,500);

}

IRSpectrogramObject::~IRSpectrogramObject()
{
    
}


// copy constructor
IRNodeObject* IRSpectrogramObject::copyThis()
{
    std::cout << "IRSpectrogramObject copyThis " << this << std::endl;
    return new IRSpectrogramObject(this->parent);
}


IRNodeObject* IRSpectrogramObject::copyContents(IRNodeObject* object)
{
    IRSpectrogramObject* obj = static_cast<IRSpectrogramObject*>(object);
    
 
    return obj;
}




t_json IRSpectrogramObject::saveThisToSaveData()
{

    
    t_json save = t_json::object({
        {"waveform", "ok"}
    });
    
    return save;
}


void IRSpectrogramObject::loadThisFromSaveData(t_json data)
{
    
    
}


void IRSpectrogramObject::resized()
{
    this->spectrogram->setSize(getWidth(), getHeight());
}


void IRSpectrogramObject::paint(Graphics& g)
{
    if(isEditMode())
    {
        auto area = getLocalBounds();//.reduced (2);
        
        g.setColour (SYSTEMCOLOUR.contents);
        //g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
        g.drawRect(area.toFloat(), 1.0);
    }
}


// call back function automatically called when the status of this object changed by others.
// write some tasks here
void IRSpectrogramObject::statusChangedCallback(IRNodeComponentStatus status)
{
    switch (status)
    {
        case EditModeStatus:
            if(isEditMode())
            {
            }else{
            }
            break;
        case SelectableStatus:
            break;
        case HasResizedStatus:
            this->isBeingResized = true;
            break;
        default:
            break;
    }
}


// events
void IRSpectrogramObject::mouseDownEvent(const MouseEvent& e)
{
    std::cout << "IRSpectrogramObject Object mouse down\n";
    
    IRNodeObject::mouseDownEvent(e);
    
}


void IRSpectrogramObject::changeListenerCallback(ChangeBroadcaster* source)
{
   
}


void IRSpectrogramObject::mouseUpEvent(const MouseEvent& e)
{
    if(this->isBeingResized)
    {
        //this->spectrogram->recreateSpectrogram();
        this->isBeingResized = false;
    }
}

