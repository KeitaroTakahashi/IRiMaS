//
//  IRIVSRObject.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 31/05/2019.
//

#include "IRISVObject.hpp"

IRISVObject::IRISVObject(Component* parent) :
IRNodeObject(parent, "IRISV", NodeObjectType(heavyWeightComponent))
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
    std::cout << "IRISVObject deconstructed!\n";
}
// ------------------------------------------------------------

void IRISVObject::paint(Graphics &g)
{
    
    //auto area = getLocalBounds().reduced (2);
    g.setColour (SYSTEMCOLOUR.contents);
    //g.drawRoundedRectangle (area.toFloat(), 5.0f, 4.0f);
    g.drawRect(getLocalBounds().toFloat(), 1.0);
    
    g.drawText("3D Graphics", getLocalBounds(), dontSendNotification);
    
    
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
            
            {"materialColour", t_json::array({
                p.materialColour.getVal1(),
                p.materialColour.getVal2(),
                p.materialColour.getVal3()
            })},
            
            {"lightingColour", t_json::array({
                p.lightingColour.getVal1(),
                p.lightingColour.getVal2(),
                p.lightingColour.getVal3()
            })},
            
            {"lightPosition", t_json::array({
                p.lightPosition.getVal1(),
                p.lightPosition.getVal2(),
                p.lightPosition.getVal3()
            })},

            
            {"transform", t_json::array({
                p.transform.getVal1(),
                p.transform.getVal2(),
                p.transform.getVal3()
            })},
            
            {"quaterStretchX", t_json::array({
                p.quaterStretchX.getVal1(),
                p.quaterStretchX.getVal2(),
                p.quaterStretchX.getVal3(),
                p.quaterStretchX.getVal4()
            })},

            {"quaterStretchY", t_json::array({
                p.quaterStretchY.getVal1(),
                p.quaterStretchY.getVal2(),
                p.quaterStretchY.getVal3(),
                p.quaterStretchY.getVal4()
            })},
            
            {"halfStretchX", t_json::array({
                p.halfStretchX.getVal1(),
                p.halfStretchX.getVal2()
            })},
            
            {"halfStretchY", t_json::array({
                p.halfStretchY.getVal1(),
                p.halfStretchY.getVal2()
            })},

            {"halfStretchZ", t_json::array({
                p.halfStretchZ.getVal1(),
                p.halfStretchZ.getVal2()
            })},

            {"intensity", p.intensity},

            {"fineness", p.fineness},
            
            {"vertexNoiseSpeed", p.vertexNoiseSpeed},
            
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
        KVector<float> lp = KVector<float> (lightPosition[0].number_value(),
                                            lightPosition[1].number_value(),
                                            lightPosition[2].number_value(),
                                            0.0);
        loadedPresets.lightPosition = lp;
        
        // materialColour
        t_json materialColour = s["materialColour"].array_items();
        KVector<float> mc = KVector<float>(materialColour[0].number_value(),
                                           materialColour[1].number_value(),
                                           materialColour[2].number_value());
        loadedPresets.materialColour = mc;
        
        // lightingColour
        t_json lightingColour = s["lightingColour"].array_items();
        KVector<float> lc = KVector<float>(lightingColour[0].number_value(),
                                           lightingColour[1].number_value(),
                                           lightingColour[2].number_value());
        loadedPresets.lightingColour = lc;
        
        // quaterStretchX
        t_json quaterStretchX = s["quaterStretchX"].array_items();
        KVector<float> qX = KVector<float> (quaterStretchX[0].number_value(),
                                            quaterStretchX[1].number_value(),
                                            quaterStretchX[2].number_value(),
                                            quaterStretchX[3].number_value());
        loadedPresets.quaterStretchX = qX;

        
        // quaterStretchX
        t_json quaterStretchY = s["quaterStretchY"].array_items();
        KVector<float> qY = KVector<float> (quaterStretchY[0].number_value(),
                                            quaterStretchY[1].number_value(),
                                            quaterStretchY[2].number_value(),
                                            quaterStretchY[3].number_value());
        loadedPresets.quaterStretchY = qY;
        
        
        // halfStretchX
        t_json halfStretchX = s["halfStretchX"].array_items();
        KVector<float> hX = KVector<float> (halfStretchX[0].number_value(),
                                            halfStretchX[1].number_value());
        loadedPresets.halfStretchX = hX;
        
        // halfStretchX
        t_json halfStretchY = s["halfStretchY"].array_items();
        KVector<float> hY = KVector<float> (halfStretchY[0].number_value(),
                                            halfStretchY[1].number_value());
        loadedPresets.halfStretchY = hY;
        
        // transform
        t_json transform = s["transform"].array_items();
        KVector<float> trm = KVector<float> (transform[0].number_value(),
                                            transform[1].number_value(),
                                            transform[2].number_value(),
                                            0.0);
        
        loadedPresets.transform = trm;
        
        loadedPresets.intensity = s["intensity"].number_value();
        loadedPresets.fineness = s["fineness"].number_value();
        loadedPresets.vertexNoiseSpeed = s["vertexNoiseSpeed"].number_value();
        loadedPresets.amount = s["stretch_amount"].number_value();
        
        std::cout << "intensity = " << loadedPresets.intensity << ", fineness = " << loadedPresets.fineness << std::endl;
        // quater Stretch X

        this->UI->addPreset(loadedPresets, false);
        
        index ++;
    }
    
    
}
