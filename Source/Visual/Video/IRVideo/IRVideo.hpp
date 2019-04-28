//
//  IRVideo.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 01/03/2019.
//

#ifndef IRVideo_hpp
#define IRVideo_hpp

#include "JuceHeader.h"

class IRVideo
{
public:
    IRVideo(bool useNativeControlsIfAvailable);
    ~IRVideo();
    
private:
    VideoComponent video;
};

#endif /* IRVideo_hpp */
