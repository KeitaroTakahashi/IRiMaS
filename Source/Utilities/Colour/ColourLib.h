//
//  ColourLib.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 24/08/2018.
//

#ifndef ColourLib_h
#define ColourLib_h

#include "JuceHeader.h"
#include "singletonClass.hpp"

namespace IR
{
    
    
    struct colorPattern
    {
        Colour fundamental = Colour(0,0,0);
        Colour background = Colour(0,0,0);
        Colour contents = Colour(0,0,0);
        Colour titleText = Colour(0,0,0);
        Colour text = Colour(0,0,0);
    };
    
    struct darkBrownSet : public colorPattern
    {
        darkBrownSet()
        {
            this->background = Colour(255,255,255);
            this->fundamental = Colour(45,30,14);
            this->contents = Colour(122,98,71);
            this->titleText = Colour(0,0,0);
            this->text = Colour(255,255,255);
        }
    };
    
    struct darkMagentaSet : public colorPattern
    {
        darkMagentaSet()
        {
            this->background = Colour(255,255,255);
            this->fundamental = Colour(61,7,35);
            this->contents = Colour(125,77,99);
            this->titleText = Colour(0,0,0);
            this->text = Colour(255,255,255);
        }
    };
    
    struct darkBlueSet : public colorPattern
    {
        darkBlueSet()
        {
            this->background = Colour(255,255,255);
            this->fundamental = Colour(2,27,53);
            this->contents = Colour(71,88,116);
            this->titleText = Colour(0,0,0);
            this->text = Colour(255,255,255);
        }
    };
    
    struct whiteSet : public colorPattern
    {
        whiteSet()
        {
            this->background = Colour(255,255,255);
            this->fundamental = Colour(255,255,255);
            this->contents = Colour(220,220,220);
            this->titleText = Colour(30,30,30);
            this->text = Colour(30,30,30);
        }
    };
    
    class IRColours
    {
    public:
        Colour fundamental;
        Colour background;
        Colour contents;
        Colour titleText;
        Colour text;
        
        IRColours() {}
        
        IRColours(colorPattern pattern)
        {
            this->background = pattern.background;
            this->fundamental = pattern.fundamental;
            this->contents = pattern.contents;
            this->titleText = pattern.titleText;
            this->text = pattern.text;
        }
        
        void set(colorPattern pattern)
        {
            this->fundamental = pattern.fundamental;
            this->background = pattern.background;
            this->contents = pattern.contents;
            this->titleText = pattern.titleText;
            this->text = pattern.text;
        }
        
    };
    
    class HSV2RGB
    {
    public:
        HSV2RGB(float hue, float saturation, float brightness) :
        _hue(hue),
        _saturation(saturation),
        _brightness(brightness)
        {
            
        }
        
        ~HSV2RGB() {}
        /*
        void convert()
        {
            const float chroma = _brightness * _saturation;
            const float hNormalize = _hue / 60.0;
            const float x = _chroma * ( 1.0 - abs(hNormalize%2.0) - 1.0);
            
            if(hNormalize < 1.0)
            {
                r = x;
                b = x;
            }else if(hNormalize < 2.0){
                r = x;
                b = chroma;
            }else if(hNormalize < 3.0){
                
            }
        }*/
        
    private:
        float _hue;
        float _saturation;
        float _brightness;
        
        float r;
        float g;
        float b;
    };

}

#endif /* ColourLib_h */
