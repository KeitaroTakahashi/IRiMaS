//
//  IRSpectrogramUI.hpp
//  IRiMaS
//
//  Created by Keitaro on 08/07/2019.
//

#ifndef IRSpectrogramUI_hpp
#define IRSpectrogramUI_hpp

#include "IRSpectrogramComponent.hpp"
#include "IRSpectrogramViewPort.hpp"
#include "IRMeasureGrid.hpp"


class IRSpectrogramUI : public IRUIFoundation,
public ChangeBroadcaster
{
public:
    
    enum IRSpectrogramUIStatus
    {
        AudioFileImportCompleted,
        AudioFileChanged,
        DescriptorSelected
    };
    
    IRSpectrogramUI(IRNodeObject* nodeObject);
    ~IRSpectrogramUI();
    // ==================================================
    void resized() override;
    void paint(Graphics& g) override;
    // ==================================================
    
    IRSpectrogramUIStatus getStatus() const { return this->status; }

    void openFile();
    void openFile(String path);
    
    String getFilePath() const { return this->path; }
    
    // ==================================================
    virtual void fileImportCompletedAction() = 0;
    DataAllocationManager<IRAudio>* getAudioData() { return this->audioData; }
    
    // ==================================================

    void setMagnitudeAmount(float val) { this->spectrogram->getComponent()->setMagnitudeAmount(val); }

    void mouseDown(const MouseEvent& e) override
    {
        zoomInClicked();
    }
    
private:
    // ==================================================
    //UI
    IRSpectrogramUIStatus status;
    IRNodeObject* parent;
    
    // audio data
    void getFilePtr(File file);
    void fileImportCompleted();
    DataAllocationManager<IRAudio>* audioData = nullptr;
    File file;
    String path;
    // ==================================================
    //ViewPort
    std::shared_ptr<IRSpectrogramViewUI> spectrogramView;
    std::shared_ptr<IRSpectrogramComponent> spectrogram;
    float spectrogram_width_ratio = 1.0;
    
    void zoomInClicked();
    void zoomOutClicked();
    void visibleAreaChanged(Rectangle<int> area);

    int previousOffsetX = 0;
    int xMargin = 5;
    int yMargin = 1;
    // ==================================================
    // setup
    bool isVerticalMovable = true;
    bool isHorizontalMovable = true;
    
    // all setter
    void setMovable(bool movable, bool verticalMovable, bool horizontalMovable);
    
    // ==================================================
    
   
    // ==================================================

    // ==================================================
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRSpectrogramUI)

};

#endif /* IRSpectrogramUI_hpp */
