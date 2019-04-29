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
    
    void setOffsetX(float offsetX);
    void setOffsetY(float offsetY);
    void setOffsetZ(float offsetZ);
    
    float getOffsetX() const { return this->offsetX; };
    float getOffsetY() const { return this->offsetY; };
    float getOffsetZ() const { return this->offsetZ; };
    
    void setFineness(float fineness);
    void setIntensity(float intensity);
    // ==================================================
    
    
    // ==================================================
    
    Matrix3D<float> getProjectionMatrix() const;
    Matrix3D<float> getViewMatrix() const;
    void setShaderProgram (const String& vertexShader, const String& fragmentShader);
    
    
    Draggable3DOrientation draggableOrientation;
    bool doBackgroundDrawing = false;
    float scale = 0.05, rotationSpeed = 0.007;
    BouncingNumber bouncingNumber;
    
    
private:
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
    
    
    // vertex parameters
    float offsetX = 0.0;
    float offsetY = 0.0;
    float offsetZ = 0.0;
    
    float fineness = 0.1;
    float intensity = 0.1;
    
    float lightPositionY = 1.0f;
    float lpy = -15.0f;
    float lightPositionYSpeed = 0.1;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLVertexNoise)
    
    
};


#endif /* OpenGLVertexNoise_hpp */
