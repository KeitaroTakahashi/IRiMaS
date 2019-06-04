//
//  IRWaveformObjectUI2.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 02/06/2019.
//

#ifndef IRWaveformObjectUI2_hpp
#define IRWaveformObjectUI2_hpp

#include "IRWaveformObjectUI.hpp"
#include "IRViewPort.hpp"
#include "IRMeasureGrid.hpp"



class IRWaveformObjectUI2 : IRUIAudioFoundation
{
public:
    
    enum IRWaveformUIStatus
    {
        AudioFileImportCompleted,
        AudioFileChanged,
        DescriptorSelected
    };
    
private:
    
    
    // ==================================================
    
    std::shared_ptr<IRWaveformObjectUI> waveform;
    
    float automation_width_ratio = 1.0;
    
    int xMargin = 5;
    int yMargin = 1;
    
    // ==================================================
    
    int gridSize = 30;
    
    IRMeasureGrid verticalGrid;
    IRMeasureGrid horizontalGrid;
    
    class ComponentForViewPort : public Component
    {
    public:
        ComponentForViewPort(IRWaveformObjectUI* waveform,
                             IRMeasureGrid* verticalGrid,
                             IRMeasureGrid* horizontalGrid,
                             int gridSize) : gridSize(gridSize)
        {
            this->waveform = waveform;
            this->verticalGrid = verticalGrid;
            this->horizontalGrid = horizontalGrid;
            
            addAndMakeVisible(waveform);
            addAndMakeVisible(verticalGrid);
            addAndMakeVisible(horizontalGrid);
            
        }
        
        ~ComponentForViewPort() {}
        
        void resized() override
        {
            this->waveform->setBounds(this->gridSize,
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
        
        IRWaveformObjectUI* waveform;
        IRMeasureGrid* verticalGrid;
        IRMeasureGrid* horizontalGrid;
        
        int gridSize;
    };
};
#endif /* IRWaveformObjectUI2_hpp */
