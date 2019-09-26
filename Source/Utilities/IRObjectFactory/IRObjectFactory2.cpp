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
    registerObject<IRAutomationObject>("IRAutomation",
                                       "Automation Graph",
                                       objectCategory::CHARTMENU,
                                       img);
    
    //registerObject<IRAudioAnalysisObject>("IRAudioAnalysis", "AudioAnalysis", img);
    
    registerObject<IRWaveformObject>("IRWaveform",
                                      "Waveform",
                                       objectCategory::AUDIOMENU,
                                        img);           // leak is audio
    
    registerObject<IRLabelObject>("IRLabel",
                                                "Label",
                                                objectCategory::TEXTMENU,
                                                img);
    
    registerObject<IRISVObject>("IRISV",
                                              "Interactive Sound Visualizer",
                                              objectCategory::OBJECTMENU,
                                              img);
    
    
    
    //registerObject<IRSoundPlayerObject>("IRSoundPlayer", "Sound Player", img); // leak is audio
    
    
    registerObject<IRTextEditorObject>("IRTextEditor",
                                                     "Text Editor",
                                                     objectCategory::TEXTMENU,
                                                     img);
    registerObject<IRImageViewerObject>("IRImageViewer",
                                                      "Image Viewer",
                                                      objectCategory::IMAGEMENU,
                                                      img);
    //registerObject<IRSoundPlayerObject>("IRSoundPlayer", "Sound Player", img); // leak is audio
    registerObject<IRSpectrogramObject>("IRSpectrogram",
                                                      "Spectrogram",
                                                      objectCategory::AUDIOMENU,
                                                      img);  // leak is audio
    
    
    registerObject<IRVideoPlayerObject>("IRVideoPlayer",
                                                      "Video Player",
                                                      objectCategory::PLAYERMENU,
                                                      img); // leak videocomp
    //registerObject<IRTestObject>("IRTestObject", "test", img);
    
    registerObject<IRShapeObject>("IRShape",
                                                "Shape",
                                                objectCategory::IMAGEMENU,
                                                img); // leak videocomp
    
    //registerObject<IRVideoThumbnailObject>("IRVideoThumbnail", "Video Thumbnail", img);
}


IRNodeObject* IRObjectFactory2::createObject(std::string id, Component* parent)
{
    return this->list[id].obj->create(parent);
}


void IRObjectFactory2::showRegisteredObjectList()
{
    std::cout << "showRegisteredObjectList" << std::endl;
    for (auto item : this->list)
    {
        std::cout << item.first << std::endl;
    }
}


