//
//  ISVPresetDataStr.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 05/06/2019.
//

#ifndef ISVPresetDataStr_h
#define ISVPresetDataStr_h
#include "JuceHeader.h"
#include "KVector.h"


struct ISVPresetDataStr
{
    String presetTitle;
    
    KVector<float> lightPosition;
    
    KVector<float> materialColour;
    KVector<float> lightingColour;
    
    // Top Left, Buttom Left, Buttom Right, Top Right
    KVector<float> quaterStretchX;
    KVector<float> quaterStretchY;
    
    // Left, Right
    KVector<float> halfStretchX;
    KVector<float> halfStretchY;
    KVector<float> halfStretchZ;
    
    // 0.0f, 0.0f, -18.0f
    KVector<float> transform;
    
    // vertex noise
    float intensity = 0.0;
    float fineness = 0.0;
    float vertexNoiseSpeed = 0.0;
    
    float amount;
    
    ISVPresetDataStr()
    {
        this->materialColour = KVector<float>(0.5f, 0.84f, 0.1f, 1.0f);
        this->lightingColour = KVector<float>(0.71f, 1.0f, 0.77f, 1.0f);
        
        this->lightPosition = KVector<float> (5.0f, 15.0f, 0.0f, 0.0f);

        this->quaterStretchX = KVector<float> (1.0f, 1.0f, 1.0f, 1.0f);
        this->quaterStretchY = KVector<float> (1.0f, 1.0f, 1.0f, 1.0f);
        
        this->halfStretchX = KVector<float> (1.0f, 1.0f);
        this->halfStretchY = KVector<float> (1.0f, 1.0f);
        this->halfStretchZ = KVector<float> (1.0f, 1.0f);

        this->transform = KVector<float> (0.0f, 0.0f, -18.0f, 0.0f);
        
        this->amount = 0.0;
    }
    
    ISVPresetDataStr operator- (ISVPresetDataStr b)
    {
        ISVPresetDataStr result;
        result.materialColour = this->materialColour - b.materialColour;
        result.lightingColour = this->lightingColour - b.lightingColour;
        result.lightPosition = this->lightPosition - b.lightPosition;

        result.quaterStretchX = this->quaterStretchX - b.quaterStretchX;
        result.quaterStretchY = this->quaterStretchY - b.quaterStretchY;

        result.halfStretchX = this->halfStretchX - b.halfStretchX;
        result.halfStretchY = this->halfStretchY - b.halfStretchY;
        result.halfStretchZ = this->halfStretchZ - b.halfStretchZ;
        

        result.transform = this->transform - b.transform;
        result.intensity = this->intensity - b.intensity;
        result.fineness = this->fineness - b.fineness;
        result.vertexNoiseSpeed = this->vertexNoiseSpeed - b.vertexNoiseSpeed;
        result.amount = this->amount - b.amount;

        
        return result;
    }
    
    ISVPresetDataStr operator* (float a)
    {
        ISVPresetDataStr result;
        
        result.materialColour = this->materialColour * a;
        result.lightingColour = this->lightingColour * a;

        result.lightPosition = this->lightPosition * a;
        
        result.quaterStretchX = this->quaterStretchX * a;
        result.quaterStretchY = this->quaterStretchY * a;
        
        result.halfStretchX = this->halfStretchX * a;
        result.halfStretchY = this->halfStretchY * a;
        result.halfStretchZ = this->halfStretchZ * a;
        
        result.transform = this->transform * a;
        result.intensity = this->intensity * a;
        result.fineness = this->fineness * a;
        result.vertexNoiseSpeed = this->vertexNoiseSpeed * a;
        
        result.amount = this->amount * a;

        return result;
    }
    
    void show()
    {
        std::cout << " === ISVPresetDataPtr === \n";
        std::cout << "materialColour "; this->materialColour.show();
        std::cout << "lightingColour "; this->lightingColour.show();

        std::cout << "lightPosition "; this->lightPosition.show();

        std::cout << "quaterStretchX "; this->quaterStretchX.show();
        std::cout << "quaterStretchY "; this->quaterStretchY.show();
        
        std::cout << "halfStretchX "; this->halfStretchX.show();
        std::cout << "halfStretchY "; this->halfStretchY.show();
        std::cout << "halfStretchZ "; this->halfStretchZ.show();
        
        std::cout << "intensity = " << this->intensity << std::endl;
        std::cout << "fineness = " << this->fineness << std::endl;
        std::cout << "vertexNoise speed = " << this->vertexNoiseSpeed << std::endl;


        std::cout << "transform "; this->transform.show();
        std::cout << "amount " << this->amount << std::endl;

    }
    
    

};
#endif /* ISVPresetDataStr_h */
