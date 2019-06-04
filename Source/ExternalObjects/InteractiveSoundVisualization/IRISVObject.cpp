//
//  IRIVSRObject.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 31/05/2019.
//

#include "IRISVObject.hpp"

IRISVObject::IRISVObject(Component* parent) :
IRNodeObject(parent, "IRISV")
{

    this->UI = std::make_shared<IR3DGraphicUIWithPreference>(this);
    
    this->UI->setBounds(0, 0, 600, 400);
    addAndMakeVisible(this->UI.get());
    this->UI->addChangeListener(this);
    childComponentManager(this->UI.get());
    setSize(600, 400);
    
}


IRISVObject::~IRISVObject()
{
    
}
// ------------------------------------------------------------

void IRISVObject::paint(Graphics &g)
{
    
    //auto area = getLocalBounds().reduced (2);
    g.setColour (SYSTEMCOLOUR.contents);
    //g.drawRoundedRectangle (area.toFloat(), 5.0f, 4.0f);
    g.drawRect(getLocalBounds().toFloat(), 1.0);
    
    
}
// ------------------------------------------------------------
void IRISVObject::resized()
{
    this->UI->setBounds(0,0, getWidth(), getHeight());
}

void IRISVObject::mouseDownEvent(const MouseEvent& e)
{
    // stop OpenGL rendering when this is clicked in EditMode
    // restart OpenGL rendering if in EditMode
    IRPreferenceSpace* space = getPreferenceWindow()->getPreferenceSpace();
    
    IRPreferenceObject* current = space->getPreferenceObj();
    
    std::cout << this->name << " : PreferenceObject = " << this->UI->getPreference() << std::endl;
    
    if(current != this->UI->getPreference()){
        space->setPreferenceObj(this->UI->getPreference());
    }
    
}

void IRISVObject::mouseUpEvent(const MouseEvent& e)
{
    
    
}


