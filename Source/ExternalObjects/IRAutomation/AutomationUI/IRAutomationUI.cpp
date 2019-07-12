//
//  IRAutomation.cpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 02/05/2019.
//

#include "IRAutomationUI.hpp"

IRAutomationUI::IRAutomationUI(IRNodeObject* nodeObject) : IRUIFoundation(nodeObject),
verticalGrid(IRGridStr::IRMeasureGridType::VERTICAL),
horizontalGrid(IRGridStr::IRMeasureGridType::HORIZONTAL)
{
    this->automation = std::make_shared<InteractiveAutomation>(nodeObject);
    addAndMakeVisible(&this->automationView);
    
    this->automation->zoomInClickedCallback = [this]{ zoomInClicked(); };
    this->automation->zoomOutClickedCallback = [this]{ zoomOutClicked(); };
    this->automation->movableClickedCallback = [this](IRAutomation::movableStatus status){ movableClicked(status); };
    
    this->verticalGrid.setRange(0, 40);
    this->verticalGrid.addMouseListener(this, false);
    
    this->horizontalGrid.setRange(0, 40);
    this->horizontalGrid.addMouseListener(this, false);

    
    this->componentForViewPort = std::make_shared<ComponentForViewPort>(this->automation.get(),
                                                                        &this->verticalGrid,
                                                                        &this->horizontalGrid,
                                                                        this->gridSize);
    
    
    // IRViewPort
    //this->automationView.setViewedComponent(this->automation.get());
    this->automationView.setViewedComponent(this->componentForViewPort.get());

    this->automationView.visibleAreaChangedCallback = [this](Rectangle<int> area){ visibleAreaChanged(area); };
    
   
    
    this->parent = nodeObject;
}


IRAutomationUI::~IRAutomationUI()
{
    if(this->audioData != nullptr)
        getFileManager()->discardFilePtr(IRFileType::IRAUDIO, this->audioData, this->parent, this->file);
}

// ==================================================

void IRAutomationUI::paint(Graphics& g)
{
    g.fillAll(SYSTEMCOLOUR.background);
    
    g.setColour (SYSTEMCOLOUR.contents);
    //g.drawRoundedRectangle (area.toFloat(), 5.0f, 4.0f);
    g.drawRect(getLocalBounds().toFloat(), 1.0);
    
    
}
void IRAutomationUI::resized()
{
    int x = this->xMargin;
    int y = this->yMargin;
    int w = getWidth() - this->xMargin*2;
    int h = getHeight() - this->yMargin*2;
    
    int automationMarginY = 20;
    
    this->automationView.setBounds(x, y, w, h);
    this->automationView.setViewPosition(this->previousOffsetX, 0);
    
   
   
    
    this->componentForViewPort->setBounds(0,
                                          0,
                                          w * this->automation_width_ratio,
                                          h - automationMarginY);
   
    
   

    
}
// ==================================================

void IRAutomationUI::zoomInClicked()
{
    this->automation_width_ratio *= 2;
    resized();
    this->automation->reCalcPos();
    this->verticalGrid.createGrids();
}

void IRAutomationUI::zoomOutClicked()
{
    this->automation_width_ratio /= 2;
    resized();
    this->automation->reCalcPos();
    this->verticalGrid.createGrids();
    
}

void IRAutomationUI::movableClicked(IRAutomation::movableStatus status)
{
    std::cout << "movableClicked : " << status << std::endl;
    
    switch(status)
    {
        case NOTMOVABLE:
            setMovable(false, false, false);
            break;
        case HORIZONTALMOVABLE:
            setMovable(true, false, true);
            break;
        case VERTICALMOVABLE:
            setMovable(true, true, false);
            break;
        case CROSSMOVABLE:
            setMovable(true, true, true);
            break;
        default:
            break;
            
    }
}

void IRAutomationUI::setMovable(bool movable, bool verticalMovable, bool horizontalMovable)
{
    
    Array<vertex*> vs = this->automation->getVerteces();
    
    for(auto v : vs)
    {
        v->setMovable(movable, verticalMovable, horizontalMovable);
    }
    
    this->automation->setMovable(movable, verticalMovable, horizontalMovable);
    
}

// ==================================================

void IRAutomationUI::visibleAreaChanged(Rectangle<int> area)
{
    this->automation->setVisibleArea(area);
    this->componentForViewPort->setVisibleArea(area);
    
    this->previousOffsetX = area.getX();
}

// ==================================================


void IRAutomationUI::demoData(int num)
{
    this->automation->demoData(num);
    //this->controller->setMovableStatus(IRAutomation::movableStatus::VERTICALMOVABLE);
}

void IRAutomationUI::setDescriptor(IRAnalysisDataStr& data)
{
    this->automation->setDescriptor(data);
}

void IRAutomationUI::openFile()
{
    
    FileChooser chooser("Select an image file...",
                        {},
                        "*.wav, *.aif, *.aiff, *.mp3");
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        this->file = file;
        auto p = file.getFullPathName();
        this->path = p;
        
        String pathToOpen;
        pathToOpen.swapWith(p);
        
        if(pathToOpen.isNotEmpty())
        {
            getFilePtr(this->file);
        }
    }
}

void IRAutomationUI::openFile(String path)
{
    if(path.isNotEmpty())
    {
        File f(path);
        this->file = f;
        this->path = path;
        
        if(f.exists())
        {
            getFilePtr(this->file);
        }
    }
}

void IRAutomationUI::getFilePtr(File file)
{
    
    std::cout << "getFilePtr\n";
    // set a callback function which is called when file load is completed.
    // get a pointer of the audio file
    std::function<void()> callback = [this]{fileImportCompleted();};
    
    
    getFileManager()->getFilePtrWithCallBack(IRFileType::IRAUDIO,
                                             file,
                                             this->parent,
                                             callback);
    std::cout << "notify!\n";

    // notify changes to IRProject to update IRFileInspecter
    this->parent->notifyNodeObjectModification();
    
}

void IRAutomationUI::fileImportCompleted()
{
    this->audioData = static_cast<DataAllocationManager<IRAudio>*>(getFileManager()->getFileObject());
    
    this->status = AudioFileImportCompleted;
    sendChangeMessage();
    
    fileImportCompletedAction();
    
    std::cout << "fileImportCompleted!\n";

}
