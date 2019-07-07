//
//  GLSLSpectrogram.hpp
//  IRiMaS
//
//  Created by Keitaro on 26/06/2019.
//

#ifndef GLSLSpectrogram_hpp
#define GLSLSpectrogram_hpp

#include "JuceHeader.h"
#include "IRUIAudioFoundation.hpp"
#include "GLSLspectrogramHeaders.h"
#include "KTextLoader.h"

using namespace GLSLSpectrogramSpace;

class GLSLSpectrogram : public IRUIAudioFoundation,
private OpenGLRenderer,
private AsyncUpdater
{
public:
    GLSLSpectrogram(IRNodeObject* nodeObject);
    ~GLSLSpectrogram();
    // ==================================================

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
    
    void createSpectrogramView();
    
    // ==================================================
    void mouseDown(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    // ==================================================

    
    bool isRendering() const { return this->isRenderingFlag; }
    
    void startRendering();
    void stopRendering();
    
    // ==================================================
    
    Matrix3D<float> getProjectionMatrix() const;
    Matrix3D<float> getViewMatrix() const;
    void setShaderProgram (const String& vertexShader, const String& fragmentShader);
    
    
    float scale = 0.05, rotationSpeed = 0.007;
    
    
private:
    void handleAsyncUpdate() override;
    OpenGLContext openGLContext;
    
    TextButton test;
    
    // for shape
    WavefrontObjFile::Mesh mesh;
    bool isMeshUpdated = false;

    std::unique_ptr<OpenGLShaderProgram> shader;
    std::unique_ptr<ShapeSp> plane;
    std::unique_ptr<AttributesSp> attributes;
    std::unique_ptr<UniformsSp> uniforms;
    
    String newVertexShader, newFragmentShader, statusText;

    void updateShader();
    
    const char* vertexShader = nullptr;
    const char* fragmentShader = nullptr;

    float positionZ = -18.0;
    bool isRenderingFlag = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GLSLSpectrogram)
};


#endif /* GLSLSpectrogram_hpp */
