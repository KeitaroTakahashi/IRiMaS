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
#include "KeAnimationComponent.h"

class IRSpectrogram : public IRUIAudioFoundation,
public IRAudio::Listener,
public ChangeBroadcaster,
private KeAnimationComponent
{
public:
    
    enum IRSpectrogramStatus
    {
        DRAGOUT,
        DROPOUT,
        EDITMODECHANGE,
        PROJECTSAVE,
        zoomInfoShared,
        currentPlayedFrameShared
    };
    
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
    
    // called when this object needs to be refreshed
    void heavyComponentRefreshed();
    // ==================================================
    void mouseDown(const MouseEvent &e) override;
    void mouseMove(const MouseEvent& e) override;
    
    
    // ==================================================
    // redraw spectrogram
    void update();
    
    void loadDrawData(IRDescriptorStr* data);
    
    void setVisibleArea(Rectangle<int> area, Point<int> parentSize);
    Rectangle<int> getVisibleArea() const { return this->visibleArea; }
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
    
    std::function<void()> zoomInOutSharedCallback = nullptr;
    std::function<void()> currentPlayedFrameSharedCallback = nullptr;
    std::function<void()> viewPortPositionSharedCallback = nullptr;


    // ==================================================
    
    IRSpectrogramStatus status;
    IRSpectrogramStatus getStatus() const { return this->status; }
    // ==================================================
    //sharedInfo
    
    void setZoomInfo(Point<float> zoom) {
        this->zoomInfo = zoom;
        if(this->audioData != nullptr)
        {
            auto data = this->audioData->getData();
            data->setZoomInfo(this->zoomInfo);
        }
        loadDescriptor();
    }
    
    void setZoomInfo(float w, float h) {
        this->zoomInfo = Point<float>(w, h);
        setZoomInfo(this->zoomInfo);
    }
    
    void linkZoomInfo(Component* comp);
    
    
    Point<float> getZoomInfo() const { return this->zoomInfo; }
    
    void setCurrentPlayedFrame(int64 frame) { this->currentPlayedFrame = frame; }
    int64 getCurrentPlayedFrame() const { return this->currentPlayedFrame; }
    
    void linkCurrentPlayedFrame(Component* comp);
    
    // called by IRAudio
    void zoomInOutOperatedFromComponent(IRAudio* obj) override;
    void audioPlayOperatedFromComponent(IRAudio* obj) override;
    void viewPortPositionFromComponent(IRAudio* obj) override;
    
    void createPlayingLine(int64 currentFrame);

    
    // ==================================================
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
    
    //Point<float> zoomRatio;
    
    // ---------------------------------------------------------------------------
    // sharedInformation
    
    int64 currentPlayedFrame = 0;
    
    Point<float> zoomInfo;
    
    Rectangle<int> playingLine;
    
    // ---------------------------------------------------------------------------


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
    void updateAnimationFrame() override;

    
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
