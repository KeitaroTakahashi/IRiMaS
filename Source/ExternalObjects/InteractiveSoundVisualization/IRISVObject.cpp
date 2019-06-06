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


t_json IRISVObject::saveThisToSaveData()
{
    
    std::cout << "saveThisToSaveData \n";
    std::vector<t_json> presetsData;

    int index = 0;
    for(auto p : this->UI->getPresetData())
    {

        t_json preset = t_json::object({
            {"presetTitle", p.presetTitle.toStdString()},
            
            {"materialColour", t_json::array({(int)p.materialColour.getRed(), (int)p.materialColour.getGreen(), (int)p.materialColour.getBlue()})},
            
            {"lightingColour", t_json::array({(int)p.lightingColour.getRed(), (int)p.lightingColour.getGreen(), (int)p.lightingColour.getBlue()})},
            
            {"lightPosition", t_json::array({p.lightPosition.getX(),
                                             p.lightPosition.getY(),
                                             p.lightPosition.getWidth()})},

            
            {"transform", t_json::array({
                p.transform.getX(),
                p.transform.getY(),
                p.transform.getWidth()})},
            
            {"quaterStretchX", t_json::array({
                p.quaterStretchX.getX(),
                p.quaterStretchX.getY(),
                p.quaterStretchX.getWidth(),
                p.quaterStretchX.getHeight()})},

            {"quaterStretchY", t_json::array({
                p.quaterStretchY.getX(),
                p.quaterStretchY.getY(),
                p.quaterStretchY.getWidth(),
                p.quaterStretchY.getHeight()})},
            
            {"halfStretchX", t_json::array({
                p.halfStretchX.getX(),
                p.halfStretchX.getY()})},
            
            {"halfStretchY", t_json::array({
                p.halfStretchY.getX(),
                p.halfStretchY.getY()})},

            {"halfStretchZ", t_json::array({
                p.halfStretchZ.getX(),
                p.halfStretchZ.getY()})},

            {"intensity", p.intensity},

            {"fineness", p.fineness},
            
            {"stretch_amount", p.amount}

        });

        t_json presetMaster = t_json::object({
           {"preset_" + std::to_string(index), preset}
        });
      
        presetsData.push_back(presetMaster);
        
        index++;
    }
    
    t_json saveData = t_json::object({
        {"presets", presetsData}
    });
    
    t_json save = t_json::object({
        {"ISV", saveData}
    });
    return save;
}

void IRISVObject::loadThisFromSaveData(t_json data)
{
    
    std::cout << "loadThisFromSaveData ISV\n";
    t_json w = data["ISV"];
    
    t_json::array presetsData = w["presets"].array_items();
    int index = 0;
    
    
    for(auto item : presetsData)
    {
        ISVPresetDataStr loadedPresets;
        
        t_json s = item["preset_" + std::to_string(index)];
        
        // presetTitle
        std::string presetTitle = s["presetTitle"].string_value();
        loadedPresets.presetTitle = presetTitle;
        
        std::cout << "presetTitle = " << presetTitle << std::endl;
        
        // lightPosition
        t_json lightPosition = s["lightPosition"].array_items();
        Rectangle<float> lp = Rectangle<float> (lightPosition[0].number_value(),
                                                lightPosition[1].number_value(),
                                                lightPosition[2].number_value(),
                                                0.0);
        loadedPresets.lightPosition = lp;
        
        // materialColour
        t_json materialColour = s["materialColour"].array_items();
        Colour mc = Colour(materialColour[0].int_value(),
                           materialColour[1].int_value(),
                           materialColour[2].int_value());
        loadedPresets.materialColour = mc;
        
        // lightingColour
        t_json lightingColour = s["lightingColour"].array_items();
        Colour lc = Colour(lightingColour[0].int_value(),
                           lightingColour[1].int_value(),
                           lightingColour[2].int_value());
        loadedPresets.lightingColour = lc;
        
        // quaterStretchX
        t_json quaterStretchX = s["quaterStretchX"].array_items();
        Rectangle<float> qX = Rectangle<float> (quaterStretchX[0].number_value(),
                                                quaterStretchX[1].number_value(),
                                                quaterStretchX[2].number_value(),
                                                quaterStretchX[3].number_value());
        loadedPresets.quaterStretchX = qX;
        std::cout << "quaterStretchX " << loadedPresets.quaterStretchX.getX() << ", " << loadedPresets.quaterStretchX.getY() << ", " << loadedPresets.quaterStretchX.getWidth() << std::endl;

        
        // quaterStretchX
        t_json quaterStretchY = s["quaterStretchY"].array_items();
        Rectangle<float> qY = Rectangle<float> (quaterStretchY[0].number_value(),
                                                quaterStretchY[1].number_value(),
                                                quaterStretchY[2].number_value(),
                                                quaterStretchY[3].number_value());
        loadedPresets.quaterStretchY = qY;
        
        std::cout << "quaterStretchY " << loadedPresets.quaterStretchY.getX() << ", " << loadedPresets.quaterStretchY.getY() << ", " << loadedPresets.quaterStretchY.getWidth() << std::endl;
        
        // halfStretchX
        t_json halfStretchX = s["halfStretchX"].array_items();
        Point<float> hX = Point<float> (halfStretchX[0].number_value(),
                                        halfStretchX[1].number_value());
        loadedPresets.halfStretchX = hX;
        
        // halfStretchX
        t_json halfStretchY = s["halfStretchY"].array_items();
        Point<float> hY = Point<float> (halfStretchY[0].number_value(),
                                        halfStretchY[1].number_value());
        loadedPresets.halfStretchY = hY;
        
        // transform
        t_json transform = s["transform"].array_items();
        Rectangle<float> trm = Rectangle<float> (transform[0].number_value(),
                                                transform[1].number_value(),
                                                transform[2].number_value(),
                                                0.0);
        
        loadedPresets.transform = trm;
        
        loadedPresets.intensity = s["intensity"].number_value();
        loadedPresets.fineness = s["fineness"].number_value();
        loadedPresets.amount = s["stretch_amount"].number_value();
        
        std::cout << "intensity = " << loadedPresets.intensity << ", fineness = " << loadedPresets.fineness << std::endl;
        // quater Stretch X
        
        
        
        
        
        this->UI->addPreset(loadedPresets);
        
        index ++;
    }
    
    
}
