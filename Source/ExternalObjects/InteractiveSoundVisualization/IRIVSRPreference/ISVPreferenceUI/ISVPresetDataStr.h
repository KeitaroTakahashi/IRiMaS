//
//  ISVPresetDataStr.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 05/06/2019.
//

#ifndef ISVPresetDataStr_h
#define ISVPresetDataStr_h
#include "JuceHeader.h"


struct ISVPresetDataStr
{
    
    Rectangle<float> lightPosition;
    
    Colour materialColour;
    Colour lightingColour;
    
    // Top Left, Buttom Left, Buttom Right, Top Right
    Rectangle<float> quaterStretchX;
    Rectangle<float> quaterStretchY;
    
    // Left, Right
    Point<float> halfStretchX;
    Point<float> halfStretchY;
    Point<float> halfStretchZ;
    
    // 0.0f, 0.0f, -18.0f
    Rectangle<float> transform;
    
    // vertex noise
    float intensity = 0.0;
    float fineness = 0.0;
    
    ISVPresetDataStr()
    {
        this->materialColour = Colour(0.5f, 0.84f, 0.1f, 1.0f);
        this->lightingColour = Colour(0.71f, 1.0f, 0.77f, 1.0f);
        
        this->lightPosition = Rectangle<float> (5.0f, 15.0f, 0.0f, 0.0f);

        this->quaterStretchX = Rectangle<float> (1.0f, 1.0f, 1.0f, 1.0f);
        this->quaterStretchY = Rectangle<float> (1.0f, 1.0f, 1.0f, 1.0f);
        
        this->halfStretchX = Point<float> (1.0f, 1.0f);
        this->halfStretchY = Point<float> (1.0f, 1.0f);
        this->halfStretchZ = Point<float> (1.0f, 1.0f);

        this->transform = Rectangle<float> (0.0f, 0.0f, -18.0f, 0.0f);
    }

};
#endif /* ISVPresetDataStr_h */
