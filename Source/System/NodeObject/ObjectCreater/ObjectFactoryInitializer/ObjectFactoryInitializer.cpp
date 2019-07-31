
#include "ObjectFactoryInitializer.hpp"





ObjectFactoryInitializer::ObjectFactoryInitializer()
{
    Image img; // dummy
    objectFactory.registerObject<IRAutomationObject>("IRAutomation", "Automation Graph", img);
    
    //objectFactory.registerObject<IRAudioAnalysisObject>("IRAudioAnalysis", "AudioAnalysis", img);
    
    objectFactory.registerObject<IRWaveformObject>("IRWaveform", "Waveform", img);           // leak is audio

    objectFactory.registerObject<IRLabelObject>("IRLabel", "Label", img);
    
    objectFactory.registerObject<IRISVObject>("IRISV", "Interactive Sound Visualizer", img);


   
    //objectFactory.registerObject<IRSoundPlayerObject>("IRSoundPlayer", "Sound Player", img); // leak is audio


    objectFactory.registerObject<IRTextEditorObject>("IRTextEditor", "Text Editor", img);
    objectFactory.registerObject<IRImageViewerObject>("IRImageViewer", "Image Viewer", img);
    //objectFactory.registerObject<IRSoundPlayerObject>("IRSoundPlayer", "Sound Player", img); // leak is audio
    objectFactory.registerObject<IRSpectrogramObject>("IRSpectrogram", "Spectrogram", img);  // leak is audio

 
    objectFactory.registerObject<IRVideoPlayerObject>("IRVideoPlayer", "Video Player", img); // leak videocomp
    //objectFactory.registerObject<IRTestObject>("IRTestObject", "test", img);

    objectFactory.registerObject<IRShapeObject>("IRShape", "Shape", img); // leak videocomp

    //objectFactory.registerObject<IRVideoThumbnailObject>("IRVideoThumbnail", "Video Thumbnail", img);

}




