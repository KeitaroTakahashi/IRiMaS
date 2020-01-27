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
    StopWatch bench; bench.start();
    setOpaque(false);
    this->UI.reset( new IRShapeUI(this, str) );
    addAndMakeVisible(this->UI.get());
    childComponentManager(this->UI.get());
    setObjController(this->UI->getController());
    bench.result("x x x x x x x x x x IRShapeObject initialized");
    setSize(200,200);
}

IRShapeObject::~IRShapeObject()
{
    this->UI.reset();
}
IRNodeObject* IRShapeObject::copyThis()
{
    IRShapeObject* newObj = new IRShapeObject(this->parent, getStr());
    
    newObj->UI->setColour(this->UI->getColour());
    newObj->UI->setStatus(this->UI->getStatus());
    newObj->UI->setLineWidth( this->UI->getLineWidth() );
    newObj->UI->setFill(this->UI->getFill() );
    newObj->UI->repaint();
    return newObj;
}

t_json IRShapeObject::saveThisToSaveData()
{
    
    Colour c = this->UI->getColour();
    
    t_json saveData = t_json::object({
        {"textColour", json11::Json::array({c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha()})},
        {"status", (int)this->UI->getStatus()},
        {"lineWidth", this->UI->getLineWidth()},
        {"setFill", this->UI->getFill()}
    });
    
    t_json save = t_json::object({
        {"shape", saveData}
    });
    
    return save;
}
void IRShapeObject::loadThisFromSaveData(t_json data)
{
    // example : array
    t_json s = data["shape"];
    json11::Json::array c = s["textColour"].array_items();
    int r = c[0].int_value();
    int g = c[1].int_value();
    int b = c[2].int_value();
    int a = c[3].int_value();
    
    Colour textColour = Colour((uint8)r,
                               (uint8)g,
                               (uint8)b,
                               (uint8)a);
    this->UI->setColour(textColour);
    
    this->UI->setStatus((IRShapeUI::IRShapeStatus)s["status"].int_value());
    this->UI->setLineWidth(s["lineWidth"].number_value());
    this->UI->setFill(s["setFill"].bool_value());
    
}

// ------------------------------------------------------------
void IRShapeObject::paint(Graphics &g)
{
    //guide
    if(isEditMode())
    {
        g.setColour(Colours::grey);
        g.drawRect(getLocalBounds(), 1.0);
    }

    g.fillAll(Colours::transparentBlack);
}

void IRShapeObject::resized()
{
    if(this->UI.get() != nullptr)
    {
        this->UI->setBounds(getLocalBounds());
        this->UI->repaint();
    }
}
// ------------------------------------------------------------

void IRShapeObject::mouseDownEvent(const MouseEvent& e)
{
    
    if(getPreferenceWindow() != nullptr)
    {
        //IRPreferenceSpace *space = getPreferenceWindow()->getPreferenceSpace();
        
        //IRPreferenceObject* current = space->getPreferenceObj();
        /*
        if(current != this->UI->getPreference())
        {
            space->setPreferenceObj(this->UI->getPreference());
        }*/
    }
}
