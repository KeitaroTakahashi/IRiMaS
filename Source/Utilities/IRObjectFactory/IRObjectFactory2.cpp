//
//  IRObjectFactory2.cpp
//  IRiMaS
//
//  Created by Keitaro on 20/09/2019.
//

#include "IRObjectFactory2.hpp"



IRObjectFactory2::IRObjectFactory2()
{
    Image img; // dummy
    
    //registerObject<IRAudioAnalysisObject>("IRAudioAnalysis", "AudioAnalysis", img);
    //registerObject<IRSoundPlayerObject>("IRSoundPlayer", "Sound Player", img); // leak is audio

    registerObject<IRAutomationObject>("IRAutomation",
                                       "Graph",
                                       objectCategory::CHARTMENU,
                                       img);
    
    
    registerObject<IRWaveformObject>("IRWaveform",
                                      "Waveform",
                                       objectCategory::AUDIOMENU,
                                        img);           // leak is audio
    
    registerObject<IRLabelObject>("IRLabel",
                                                "Label",
                                                objectCategory::TEXTMENU,
                                                img);
    
    registerObject<IRISVObject>("IR3DVisualizer",
                                              "3D Visualizer",
                                              objectCategory::OBJECTMENU,
                                              img);
    
    
    
    
    

    
    //registerObject<IRSoundPlayerObject>("IRSoundPlayer", "Sound Player", img); // leak is audio
     
    registerObject<IRSpectrogramObject>("IRSpectrogram",
                                                      "Spectrogram",
                                                      objectCategory::AUDIOMENU,
                                                      img);
    
    

    
    
    //registerObject<IRTestObject>("IRTestObject", "test", img);
    
    
    //registerObject<IRVideoThumbnailObject>("IRVideoThumbnail", "Video Thumbnail", img);
    
    registerObject<IRImageViewerObject>("IRImageViewer",
                                        "Image Viewer",
                                        objectCategory::IMAGEMENU,
                                        img);
    
    
    registerObject<IRVideoAnnotaterInterfaceObject>("IRVideoPlayer",
                                                      "Video Player",
                                                      objectCategory::PLAYERMENU,
                                                      img);
    
    /*
    registerObject<IRVideoAnnotaterObject2>("IRVideoAnnotater2",
                                            "Video Annotater",
                                            objectCategory::PLAYERMENU,
                                            img);*/
    
    
    registerObject<IRTextEditorObject>("IRTextEditor",
                                       "Text Editor",
                                       objectCategory::TEXTMENU,
                                       img);
    
    registerObject<IRQuillTextEditorObject>("IRQuillTextEditor",
                                       "Quill Text Editor",
                                       objectCategory::TEXTMENU,
                                       img);
    
    registerObject<IRShapeObject>("IRShape",
                                  "Shape",
                                  objectCategory::IMAGEMENU,
                                  img); // leak videocomp
    std::cout << "IRFactory2 initialized\n";
}


IRNodeObject* IRObjectFactory2::createObject(std::string id, Component* parent, IRStr* str)
{
    std::cout << "IRObjectFactory2::createObject : " << id << std::endl;
    return this->list[id].obj->create(parent, str);
}


void IRObjectFactory2::showRegisteredObjectList()
{
    std::cout << "showRegisteredObjectList" << std::endl;
    for (auto item : this->list)
    {
        std::cout << item.first << std::endl;
    }
}


