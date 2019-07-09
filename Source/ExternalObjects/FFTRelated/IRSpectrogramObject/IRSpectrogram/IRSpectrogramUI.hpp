//
//  IRSpectrogramUI.hpp
//  IRiMaS
//
//  Created by Keitaro on 08/07/2019.
//

#ifndef IRSpectrogramUI_hpp
#define IRSpectrogramUI_hpp

#include "IRSpectrogram.hpp"
#include "IRViewPort.hpp"
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

    void setMagnitudeAmount(float val) { this->spectrogram->setMagnitudeAmount(val); }

    
private:
    // ==================================================
    //UI
    IRSpectrogramUIStatus status;
    IRNodeObject* parent;

    
    int gridSize = 30;
    
    IRMeasureGrid verticalGrid;
    IRMeasureGrid horizontalGrid;
    
    // audio data
    void getFilePtr(File file);
    void fileImportCompleted();
    DataAllocationManager<IRAudio>* audioData = nullptr;
    File file;
    String path;
    //
    IRViewPort spectrogramView;
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
    std::shared_ptr<IRSpectrogram> spectrogram;
    float spectrogram_width_ratio = 1.0;
    
    // ==================================================

    
    class ComponentForViewPort : public Component
    {
    public:
        ComponentForViewPort(IRSpectrogram* spectrogram,
                             IRMeasureGrid* verticalGrid,
                             IRMeasureGrid* horizontalGrid,
                             int gridSize) : gridSize(gridSize)
        {
            this->spectrogram = spectrogram;
            this->verticalGrid = verticalGrid;
            this->horizontalGrid = horizontalGrid;
            
            addAndMakeVisible(spectrogram);
            addAndMakeVisible(verticalGrid);
            addAndMakeVisible(horizontalGrid);
            
        }
        
        ~ComponentForViewPort() {}
        
        void resized() override
        {
            this->spectrogram->setBounds(this->gridSize,
                                         this->gridSize,
                                         getWidth() - this->gridSize,
                                         getHeight() - this->gridSize);
            this->verticalGrid->setBounds(0,
                                          this->gridSize,
                                          this->gridSize,
                                          getHeight() - this->gridSize);
            this->verticalGrid->createGrids();
            this->horizontalGrid->setBounds(this->gridSize,
                                            0,
                                            getWidth() - this->gridSize,
                                            this->gridSize);
            this->horizontalGrid->createGrids();
        }
        
        void setVisibleArea(Rectangle<int> area)
        {
            this->verticalGrid->setBounds(area.getX(),
                                          this->gridSize,
                                          this->gridSize,
                                          getHeight() - this->gridSize);
        }
        
        IRSpectrogram* spectrogram;
        IRMeasureGrid* verticalGrid;
        IRMeasureGrid* horizontalGrid;
        
        int gridSize;
    };
    
    std::shared_ptr<ComponentForViewPort> componentForViewPort;

    // ==================================================
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRSpectrogramUI)

};

#endif /* IRSpectrogramUI_hpp */
