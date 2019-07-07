//
//  IRSpectrogram.hpp
//  IRiMaS
//
//  Created by Keitaro on 26/06/2019.
//

#ifndef IRSpectrogram_hpp
#define IRSpectrogram_hpp

#include "IRUIAudioFoundation.hpp"
#include "GLSLSpectrogram.hpp"

class IRSpectrogram : public IRUIAudioFoundation,
private Timer
{
public:
    IRSpectrogram(IRNodeObject* nodeObject);
    ~IRSpectrogram();
    
    void init();
    // ==================================================

    void resized() override;
    void paint(Graphics& g) override;
    
    // ==================================================

    void openFile();
    void getFilePtr(File file);

    // ==================================================
    void mouseDown(const MouseEvent &e) override;
    void mouseMove(const MouseEvent& e) override;
    
    
    // ==================================================
    // redraw spectrogram
    void update();
    
    void loadDrawData(IRAnalysisDataStr data);
    
private:
    
    // ==================================================
    // UI
    IRNodeObject* parent = nullptr;
    // auido data file
    File file;
    String path;
    
    Point<int> mouseDownPos;
    Point<int> currentMousePos;
    
    DataAllocationManager<IRAudio>* audioData = nullptr;
    void fileImportCompleted();
    bool audioUpdated = false;
    
    void timerCallback() override
    {
        //repaint();
    }
    
    TextButton openButton;
    void openButtonClicked();
    // ==================================================
    //Spectrogram
    int sp_w = 100;
    int sp_h = 1024;
    
    // ==================================================
    //OpenGL
    OpenGLContext openGLContext;
    std::unique_ptr<OpenGLGraphicsContextCustomShader> shader;
    String fragmentCode;
    GLuint textureID;
    bool isTextureCreated = false;
    float* buffer = nullptr;
    String fragURL;
    
    bool fragmentRefreshed = false;


    void shaderTask(Graphics& g);
    void createTexture();
    void createDemoTexture();
    void updateFragment();
    void setUniform(OpenGLShaderProgram& program);

    // fps
    int fps_index = 0;
    int fps_previousTime = 0;
    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRSpectrogram)

};

#endif /* IRSpectrogram_hpp */
