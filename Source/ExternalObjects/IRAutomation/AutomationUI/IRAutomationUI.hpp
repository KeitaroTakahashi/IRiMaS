//
//  IRAutomation.hpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 02/05/2019.
//

#ifndef IRAutomation_hpp
#define IRAutomation_hpp

#include "JuceHeader.h"

#include "IRUIFoundation.hpp"
#include "InteractiveAutomation.hpp"
#include "AutomationController.hpp"
#include "IRViewPort.hpp"
#include "IRFFTAnalysis.h"

class IRAutomationUI : public IRUIFoundation,
public ChangeBroadcaster
{
public:
    
    enum IRAutomationUIStatus
    {
        AudioFileImportCompleted,
        AudioFileChanged,
        DescriptorSelected
    };
    
    IRAutomationUI(IRNodeObject* nodeObject);
    ~IRAutomationUI();
    // ==================================================
    void resized() override;
    // ==================================================
    
    void demoData(int num);
    
    void setDescriptor(IRAnalysisDataStr& data);
    
    void openFile();
    void openFile(String path);
        
    String getFilePath() const { return this->path; }
    
    IRAutomationUIStatus getStatus() const { return this->status; }
    
    virtual void fileImportCompletedAction() = 0;
    
    DataAllocationManager<IRAudio>* getAudioData() { return this->audioData; }
    
private:
    
    IRAutomationUIStatus status;
    
    IRNodeObject* parent;
    
    void getFilePtr(File file);
    void fileImportCompleted();
    
    DataAllocationManager<IRAudio>* audioData = nullptr;

    
    // ==================================================
    
    IRViewPort automationView;
    
    void visibleAreaChanged(Rectangle<int> area);
    
    int previousOffsetX = 0;
    
    // ==================================================
    void zoomInClicked();
    void zoomOutClicked();
    void movableClicked(IRAutomation::movableStatus status);
    // ==================================================

    // setup
    bool isVerticalMovable = true;
    bool isHorizontalMovable = true;
    
    // all setter
    void setMovable(bool movable, bool verticalMovable, bool horizontalMovable);
    
    // ==================================================
    
    std::shared_ptr<InteractiveAutomation> automation;
    
    float automation_width_ratio = 1.0;
    
    int xMargin = 5;
    int yMargin = 1;
    
    int controllerWidth = 40;
    
    File file;
    String path;
};

#endif /* IRAutomation_hpp */
