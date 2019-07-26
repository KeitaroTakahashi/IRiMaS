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
#include "IRSpectrogramController.h"
#include "Benchmark.h"

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
    
    void loadDescriptor();
    void reCalcDescriptor();
    // ==================================================
    void mouseDown(const MouseEvent &e) override;
    void mouseMove(const MouseEvent& e) override;
    
    
    // ==================================================
    // redraw spectrogram
    void update();
    
    void loadDrawData(IRDescriptorStr* data);
    
    void setVisibleArea(Rectangle<int> area, Point<int> parentSize);
    void parentSizeChanged(int w, int h);
    
    // from Preference GUI
    void setMagnitudeAmount(float val);
    void setVerticalScale(float val);
    void setHorizontalScale(float val);


    //
    void closeOpenGLComponent();
    
    // ==================================================

    std::function<void()> zoomInClickedCallback = nullptr;
    std::function<void()> zoomOutClickedCallback = nullptr;

    // ==================================================
    
    void setZoomRatio(Point<float> r) { this->zoomRatio = r; }
    Point<float> getZoomRatio() const { return this->zoomRatio; }

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
    virtual void fileImportCompletedAction() {};
    bool audioUpdated = false;
    
    void timerCallback() override
    {
        //repaint();
    }
    
    TextButton openButton;
    void openButtonClicked();
    // ==================================================
    //controller
    IRSpectrogramController controller;
    int previousOffsetX = 0;
    Rectangle<int> visibleArea;
    Point<int>      parentSize;
    
    void zoomInClicked();
    void zoomOutClicked();
    void commentClicked();
    
    // ==================================================
    //Spectrogram
    int sp_w = 100;
    int sp_h = 1024;
    // max size of w and h of Spectrogram
    // 1024
    int MAX_TEXTURE_SIZE = 512;
    
    float ratio_x = 1.0;
    float ratio_y = 1.0;
    
    void calcPixel(IRDescriptorStr* data);
    
    float magnitudeAmount = 1.0;
    float verticalScale = 8.0;
    float horizontalScale = 1.0;
    
    int fftsize = 2048;
    int hopsize = 1024;
    
    Point<float> zoomRatio;

    // ==================================================
    //OpenGL
    OpenGLContext openGLContext;
    std::unique_ptr<OpenGLGraphicsContextCustomShader> shader;
    String fragmentCode;
    GLuint textureID;
    IRTextLoader fragmentText;
    bool isTextureCreated = false;
    bool updateTexture = false;
    float* buffer = nullptr;
    
    bool isOpenGLComponentClosed = false;
    
    //IRDescriptorStr* buffer = nullptr;
    
    
    String fragURL;
    
    bool fragmentRefreshed = false;


    void shaderTask(Graphics& g);
    void createTexture();
    //void createDemoTexture();
    void updateFragment();
    void setUniform(OpenGLShaderProgram& program);

    // fps
    int fps_index = 0;
    int fps_previousTime = 0;
    
    StopWatch bench;
    StopWatch bench2;
    StopWatch bench3;

    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRSpectrogram)

};

#endif /* IRSpectrogram_hpp */
