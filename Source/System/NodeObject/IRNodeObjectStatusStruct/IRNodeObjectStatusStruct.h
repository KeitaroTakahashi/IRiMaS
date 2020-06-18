//
//  IRNodeObjectStatusStruct.h
//  IRiMaS
//
//  Created by Keitaro on 11/06/2020.
//

#ifndef IRNodeObjectStatusStruct_h
#define IRNodeObjectStatusStruct_h

struct IRNodeObjectStatusStr
{
    Rectangle<float> relativeBounds;
    Rectangle<int> bounds;
    Rectangle<int> ordinaryBounds;
    Rectangle<int> encloseBounds;
    
    bool wrap = false;
    Colour wrapColour;
    
    float startTimeCode = 0;
    float endTimeCode = 0;
    
    IRNodeObjectStatusStr()
    {
        
    }
};
#endif /* IRNodeObjectStatusStruct_h */
