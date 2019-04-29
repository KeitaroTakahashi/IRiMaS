//
//  ObjectFactory.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 21/01/2019.
//

#ifndef ObjectFactory_h
#define ObjectFactory_h

#include "ExternalObjectHeader.h"
#include "IRNodeObject.hpp"

/*
 
 Initialize object factory.
 
 Register all IRObjects here with an indentity image file which is shown in the object menu.
 
 registerObject<object type>(object registration name, object name, menu image file)
 
 The order of the registration appears to the order of the menu.
 */



class init_factory
{
public:
 
    init_factory()
    {
        Image img; // dummy
        IRFactory.registerObject<IRLabelObject>("IRLabel", "Label", img);
        
     
        IRFactory.registerObject<IRTextEditorObject>("IRTextEditor", "Text Editor", img);
        IRFactory.registerObject<IRImageViewerObject>("IRImageViewer", "Image Viewer", img);
        IRFactory.registerObject<IRSoundPlayerObject>("IRSoundPlayer", "Sound Player", img);
        IRFactory.registerObject<IRWaveformObject>("IRWaveform", "Waveform", img);
        IRFactory.registerObject<IRSpectrogramObject>("IRSpectrogram", "Spectrogram", img);
        
        IRFactory.registerObject<IRAudioAnalysisObject>("IRAudioAnalysis", "AudioAnalysis", img);

        IRFactory.registerObject<IRAutomationObject>("IRAutomation", "Automation Graph", img);
        IRFactory.registerObject<IRVideoPlayerObject>("IRVideoPlayer", "Video Player", img);
        
        IRFactory.registerObject<IRVideoThumbnailObject>("IRVideoThumbnail", "Video Thumbnail", img);
        
        IRFactory.registerObject<IRTestObject>("IRTestObject", "test", img);
         
       
    }
    
    
    
private:
    
    // IRObjectFactory
    IRObjectFactory& IRFactory = singleton<IRObjectFactory>::get_instance();
};


#endif /* ObjectFactory_h */
