//
//  IRShapeObject.cpp
//  IRiMaS
//
//  Created by Keitaro on 26/07/2019.
//

#include "IRShapeObject.hpp"

IRShapeObject::IRShapeObject(Component* parent) :
IRNodeObject(parent, "IRShape")
{
    
    
    this->UI = std::make_shared<IRShapeUI>(this);
    addAndMakeVisible(this->UI.get());
    
    setSize(200,200);
}

IRShapeObject::~IRShapeObject()
{
    
}
IRNodeObject* IRShapeObject::copyThis()
{
    IRShapeObject* newObj = new IRShapeObject(this->parent);
    
    newObj->UI->setColour(this->UI->getColour());
    newObj->UI->setStatus(this->UI->getStatus());
    newObj->UI->setLineWidth( this->UI->getLineWidth() );
    newObj->UI->setFill(this->UI->getFill() );
    std::cout << "isFill = " << this->UI->getFill() << std::endl;
    
    newObj->UI->repaint();
    return newObj;
}

t_json IRShapeObject::saveThisToSaveData()
{
    t_json saveData = t_json::object({
        {"test", "hello"}
    });
    
    t_json save = t_json::object({
        {"shape", saveData}
    });
    
    return save;
}
void IRShapeObject::loadThisFromSaveData(t_json data)
{
    
}

// ------------------------------------------------------------
void IRShapeObject::paint(Graphics &g)
{
    
}

void IRShapeObject::resized()
{
    this->UI->setBounds(getLocalBounds());
    this->UI->repaint();

}
// ------------------------------------------------------------

void IRShapeObject::mouseDownEvent(const MouseEvent& e)
{
    IRPreferenceSpace *space = getPreferenceWindow()->getPreferenceSpace();
    
    IRPreferenceObject* current = space->getPreferenceObj();
    
    if(current != this->UI->getPreference())
    {
        space->setPreferenceObj(this->UI->getPreference());
    }
}
