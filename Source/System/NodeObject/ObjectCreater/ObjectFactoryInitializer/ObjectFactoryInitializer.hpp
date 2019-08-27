
#ifndef ObjectFactoryInitializer_hpp
#define ObjectFactoryInitializer_hpp

#include "ExternalObjectHeader.h"
#include "IRNodeObject.hpp"

/*
 
 Initialize object factory.
 
 Register all IRObjects here with an identical image file which is shown on the object menu.
 
 registerObject<object type>(object registration name, object name, menu image file)
 
 The order of the registration appears to the order of the menu.
 */



class ObjectFactoryInitializer
{
    
public:
    
    ObjectFactoryInitializer()
    {
        Image img; // dummy
        objectFactory.registerObject<IRAutomationObject>("IRAutomation",
                                                         "Automation Graph",
                                                         objectCategory::CHARTMENU,
                                                         img);
        
        //objectFactory.registerObject<IRAudioAnalysisObject>("IRAudioAnalysis", "AudioAnalysis", img);
        
        objectFactory.registerObject<IRWaveformObject>("IRWaveform",
                                                       "Waveform",
                                                       objectCategory::AUDIOMENU,
                                                       img);           // leak is audio
        
        objectFactory.registerObject<IRLabelObject>("IRLabel",
                                                    "Label",
                                                    objectCategory::TEXTMENU,
                                                    img);
        
        objectFactory.registerObject<IRISVObject>("IRISV",
                                                  "Interactive Sound Visualizer",
                                                  objectCategory::OBJECTMENU,
                                                  img);
        
        
        
        //objectFactory.registerObject<IRSoundPlayerObject>("IRSoundPlayer", "Sound Player", img); // leak is audio
        
        
        objectFactory.registerObject<IRTextEditorObject>("IRTextEditor",
                                                         "Text Editor",
                                                         objectCategory::TEXTMENU,
                                                         img);
        objectFactory.registerObject<IRImageViewerObject>("IRImageViewer",
                                                          "Image Viewer",
                                                          objectCategory::IMAGEMENU,
                                                          img);
        //objectFactory.registerObject<IRSoundPlayerObject>("IRSoundPlayer", "Sound Player", img); // leak is audio
        objectFactory.registerObject<IRSpectrogramObject>("IRSpectrogram",
                                                          "Spectrogram",
                                                          objectCategory::AUDIOMENU,
                                                          img);  // leak is audio
        
        
        objectFactory.registerObject<IRVideoPlayerObject>("IRVideoPlayer",
                                                          "Video Player",
                                                          objectCategory::PLAYERMENU,
                                                          img); // leak videocomp
        //objectFactory.registerObject<IRTestObject>("IRTestObject", "test", img);
        
        objectFactory.registerObject<IRShapeObject>("IRShape",
                                                    "Shape",
                                                    objectCategory::IMAGEMENU,
                                                    img); // leak videocomp
        
        //objectFactory.registerObject<IRVideoThumbnailObject>("IRVideoThumbnail", "Video Thumbnail", img);
        
    }
    
    
private:
    
    IRObjectFactory& objectFactory { singleton<IRObjectFactory>::get_instance() };
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ObjectFactoryInitializer)
    
};





#endif /* ObjectFactoryInitializer_hpp */





