//
//  IRShapeObject.cpp
//  IRiMaS
//
//  Created by Keitaro on 26/07/2019.
//

#include "IRShapeObject.hpp"

IRShapeObject::IRShapeObject(Component* parent, IRStr* str) :
IRNodeObject(parent, "IRShape", str, NodeObjectType(orginaryIRComponent))
{
    std::cout << "IRShapeObject\n";
    setOpaque(false);
    
    this->UI = std::make_shared<IRShapeUI>(this, str);
    addAndMakeVisible(this->UI.get());
    childComponentManager(this->UI.get());
    
    setObjController(this->UI->getController());
    
   

    setSize(200,200);
}

IRShapeObject::~IRShapeObject()
{
    
}
IRNodeObject* IRShapeObject::copyThis()
{
    IRShapeObject* newObj = new IRShapeObject(this->parent, getStr());
    
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
    
    std::cout << "IRShapeObject paint\n";

    //guide
    if(isEditMode())
    {
        g.setColour(Colours::grey);
        g.drawRect(getLocalBounds(), 1.0);
    }

    g.fillAll(Colours::transparentBlack);

    std::cout << "IRShapeObject done\n";
}

void IRShapeObject::resized()
{
    this->UI->setBounds(getLocalBounds());
    this->UI->repaint();

}
// ------------------------------------------------------------

void IRShapeObject::mouseDownEvent(const MouseEvent& e)
{
    
    if(getPreferenceWindow() != nullptr)
    {
        IRPreferenceSpace *space = getPreferenceWindow()->getPreferenceSpace();
        
        IRPreferenceObject* current = space->getPreferenceObj();
        /*
        if(current != this->UI->getPreference())
        {
            space->setPreferenceObj(this->UI->getPreference());
        }*/
    }
}
