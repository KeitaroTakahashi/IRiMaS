
#include "ObjectFactoryInitializer.hpp"





ObjectFactoryInitializer::ObjectFactoryInitializer()
{
    Image img; // dummy
    
    objectFactory.registerObject<IRLabelObject>("IRLabel", "Label", img);
    objectFactory.registerObject<IRTextEditorObject>("IRTextEditor", "Text Editor", img);
    
    // DISABLING THE REST NOW TO FOCUS MY ATTENTION ON IRLABELOBJECT
    
    /*
    
    objectFactory.registerObject<IRImageViewerObject>("IRImageViewer", "Image Viewer", img);
    objectFactory.registerObject<IRSoundPlayerObject>("IRSoundPlayer", "Sound Player", img);
    objectFactory.registerObject<IRWaveformObject>("IRWaveform", "Waveform", img);
    objectFactory.registerObject<IRSpectrogramObject>("IRSpectrogram", "Spectrogram", img);
    objectFactory.registerObject<IRAutomationObject>("IRAutomation", "Automation Graph", img);
    objectFactory.registerObject<IRVideoPlayerObject>("IRVideoPlayer", "Video Player", img);
    
    objectFactory.registerObject<IRVideoThumbnailObject>("IRVideoThumbnail", "Video Thumbnail", img);
    
    objectFactory.registerObject<IRTestObject>("IRTestObject", "test", img);
     */
    
    // objectFactory.registerObject<IRCVVideoPlayer>("IRCVVideoPlayer", "CV Video Player", img);
}




