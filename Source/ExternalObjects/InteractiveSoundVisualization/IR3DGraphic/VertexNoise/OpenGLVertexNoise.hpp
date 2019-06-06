//
//  OpenGLVertexNoise.hpp
//  OpenGLVertexNoiseTest - App
//
//  Created by Keitaro on 15/03/2019.
//

#ifndef OpenGLVertexNoise_hpp
#define OpenGLVertexNoise_hpp

#include <fstream>
#include <string>

#include "../JuceLibraryCode/JuceHeader.h"
#include "OpenGL/OpenGL.h"
#include "3DModelObjectLoader.h"
#include "DemoController.h"
#include "KTextLoader.h"

class OpenGLVertexNoise : public Component,
private OpenGLRenderer,
private AsyncUpdater

{
public:
    OpenGLVertexNoise();
    ~OpenGLVertexNoise();
    // ==================================================
    
    void paint (Graphics&) override;
    void resized() override;
    
    // ==================================================
    
    void newOpenGLContextCreated() override;
    
    void openGLContextClosing() override;
    
    // This is a virtual method in OpenGLRenderer, and is called when it's time
    // to do your GL rendering.
    void renderOpenGL() override;
    
    // ==================================================
    
    void freeAllContextObjects();
    
    void createShaders();
    // ==================================================
    void stopRendering();
    
    void startRendering();
    // ==================================================
    
    void setLightPosition(Rectangle<float> val);
    void setIntensity(float val);
    void setFineness(float val);

    void setQuaterStretchX(Rectangle<float> val);
    void setQuaterStretchY(Rectangle<float> val);

    void setHalfStretchX(Point<float> val);
    void setHalfStretchY(Point<float> val);
    void setHalfStretchZ(Point<float> val);

    void setTransform(Rectangle<float> val);

    void setMaterialColour(Colour colour);
    void setLightingColour(Colour colour);
    // ==================================================

    void setOffsetX(float offsetX);
    void setOffsetY(float offsetY);
    void setOffsetZ(float offsetZ);
    
    void setAmountX(float amountX);
    void setAmountY(float amountY);
    void setAmountZ(float amountZ);
    

    
    // if x, y, z is under 0, then ignore
    void setPositiveStretch(float x, float y, float z);
    void setNegativeStretch(float x, float y, float z);
   
    void setStretchX(float x, float y, float z, float w);
    void setStretchY(float x, float y, float z, float w);

    void setStretchAmount(float amount);

    // ==================================================

    float getOffsetX() const { return this->offsetX; };
    float getOffsetY() const { return this->offsetY; };
    float getOffsetZ() const { return this->offsetZ; };
    
    float getAmountX() const { return this->amountX; }
    float getAmountY() const { return this->amountY; }
    float getAmountZ() const { return this->amountZ; }
    
    void setPositionX(float positionX) { this->positionX = positionX; }
    float getPositionX() const { return this->positionX; }
    void setPositionY(float positionY) { this->positionY = positionY; }
    float getPositionY() const { return this->positionY; }
    void setPositionZ(float positionZ) { this->positionZ = positionZ; }
    float getPositionZ() const { return this->positionZ; }
    
    // ==================================================
    
    bool isRendering() const { return this->isRenderingFlag; }
    // ==================================================
    
    Matrix3D<float> getProjectionMatrix() const;
    Matrix3D<float> getViewMatrix() const;
    void setShaderProgram (const String& vertexShader, const String& fragmentShader);
    
    
    Draggable3DOrientation draggableOrientation;
    bool doBackgroundDrawing = false;
    float scale = 0.05, rotationSpeed = 0.007;
    BouncingNumber bouncingNumber;
    
    
private:
    
    bool isRenderingFlag = false;
    
    void handleAsyncUpdate() override;
    
    void drawBackground2DStuff (float desktopScale);
    
    OpenGLContext openGLContext;
    
    std::unique_ptr<DemoController> controlsOverlay;
    
    float rotation = 30;
    
    std::unique_ptr<OpenGLShaderProgram> shader;
    std::unique_ptr<Shape> shape;
    std::unique_ptr<Attributes> attributes;
    std::unique_ptr<Uniforms> uniforms;
    
    OpenGLTexture texture;
    //DemoTexture* textureToUse = nullptr;
    //DemoTexture* lastTexture  = nullptr;
    
    String newVertexShader, newFragmentShader, statusText;
    
    struct BackgroundStar
    {
        SlowerBouncingNumber x, y, hue, angle;
    };
    
    BackgroundStar stars[3];
    
    
    void updateShader();
    
    const char* vertexShader;
    const char* fragmentShader;
    
    // ==============================
    // vertex parameters
    float offsetX = 10.0;
    float offsetY = 10.0;
    float offsetZ = 10.0;
    
    float amountX = 1.0;
    float amountY = 1.0;
    float amountZ = 1.0;
    
    float fineness = 0.1;
    float intensity = 0.1;
    // ==============================
    // stretch
    Rectangle<float> positiveStretch;
    Rectangle<float> negativeStretch;

    Rectangle<float> stretchX;
    Rectangle<float> stretchY;
    
    Rectangle<float> transform;
    float stretchAmount = 1.0;
    
    
    // ==============================

    float lightPositionX = 5.0f;
    float lightPositionY = 1.0f;
    float lightPositionZ = 15.0f;
    float lpy = -15.0f;
    float lightPositionYSpeed = 0.1;
    
    float positionX = 0.0;
    float positionY = 0.0;
    float positionZ = -18.0;

    // ==============================
    Rectangle<float> materialColour;
    Rectangle<float> lightingColour;
    
    // ==============================

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLVertexNoise)
    
    
};


#endif /* OpenGLVertexNoise_hpp */
