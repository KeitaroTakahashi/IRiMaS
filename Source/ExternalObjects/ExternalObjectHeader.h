//
//  ExternalObjectHeader.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 23/08/2018.
//

#ifndef ExternalObjectHeader_h
#define ExternalObjectHeader_h

// ------------------------------------------------------------
// test related object
#include "IRTestObject.h"

// ------------------------------------------------------------
// audio related object
#include "IRAudioPlayerObject.hpp"

#include "IRWaveformObject.hpp"

// ------------------------------------------------------------
// FFT related object
#include "IRSpectrogramObject.hpp"

// ------------------------------------------------------------
// image related object
#include "IRImageViewerObject.h"

// ------------------------------------------------------------
// video related object
#include "IRVideoPlayerObject.hpp"
#include "IRVideoThumbnailObject.hpp"
#include "NSVideoPlayerObject.h"
#include "IRFfomegVideoPlayer.hpp"
#include "IRCVVideoPlayer.hpp"
// ------------------------------------------------------------
// Text related object
#include "IRLabelObject.h"
#include "IRTextEditorObject.h"

// ------------------------------------------------------------
// UIs
#include "IRAutomationObject.hpp"

// other object

#include "ObjectGlobalTestObject.h"

// ------------------------------------------------------------
#include "IRObjectCreater.hpp"



namespace IR{
    
 
    
    
    
    
    
enum ObjectList
{
  
    IRLabel,
    IRTextEditor,
    IRImageViwer,
    IRSoundPlayer,
    IRWaveform,
    IRSpectrogram,
    IRAutomation
    
};
    
    
}


#endif /* ExternalObjectHeader_h */