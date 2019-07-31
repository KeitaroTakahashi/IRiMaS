//
//  IRAnalysisUI.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/04/2019.
//

#ifndef IRAnalysisUI_hpp
#define IRAnalysisUI_hpp

#include "IRFoundation.h"
#include "IRUIAudioFoundation.hpp"
#include "IRAudioReader.hpp"
#include "IRAudioDescriptor.hpp"

class IRAnalysisUI : public IRUIAudioFoundation,
private ChangeListener,
public ChangeBroadcaster
{
public:
    IRAnalysisUI(IRNodeObject* parent);
    ~IRAnalysisUI();
    
    void openFile();
    void openFile(String path);
    
    void changeListenerCallback (ChangeBroadcaster* source) override {};

    
    void fileImportCompleted();
    void getFilePtr(File file);
    
    double getSampleRate() const;
    
    virtual void resized() override;
    void paint(Graphics& g) override;
    
    // ==================================================
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
    virtual void releaseResources() override;
    // ==================================================
    
    String getPath() const { return this->path; }
    
    DataAllocationManager<IRAudio>* audioData = nullptr;
    
    // ==================================================
    // link system
    virtual void audioPtrDelivery(IRAudio *obj) override;
    
    // ==================================================
    
    void deinitializeAudioData();
    
    std::function<void()> completeAnalysis;
    
    
    std::shared_ptr<IRAnalysisDataStr> magData;
    std::shared_ptr<IRAnalysisDataStr> centData;
private:
    
    
    // ==================================================
    // analysis
    void executeAnalysis();
    bool analysisDone = false;
    
    int nframe = 0;
    int fftsize = 2048;
    int hopsize = 1024;
    
    // ==================================================

    File file;
    String path;
    IRNodeObject* parent;
    TextButton openButton;
    
    // parameters
    double start = 0.0;
    double duration = 0.0;
    int channelIndex = 0;
    double zoomFactor = 1.0;
    
    
    //thread lock
    CriticalSection callbackLock;
    
    std::vector<float> rawData;
    
    std::shared_ptr<IRFFTDescriptor> FFTSequence;
    


    
    // system appearance
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    IRFileManager& FILEMANAGER = singleton<IRFileManager>::get_instance();
};

#endif /* IRAnalysisUI_hpp */
