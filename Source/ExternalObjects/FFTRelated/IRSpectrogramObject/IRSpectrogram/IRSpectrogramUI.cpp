//
//  IRSpectrogramUI.cpp
//  IRiMaS
//
//  Created by Keitaro on 08/07/2019.
//

#include "IRSpectrogramUI.hpp"

IRSpectrogramUI::IRSpectrogramUI(IRNodeObject* nodeObject) :
IRUIFoundation(nodeObject),
verticalGrid(IRGridStr::IRMeasureGridType::VERTICAL),
horizontalGrid(IRGridStr::IRMeasureGridType::HORIZONTAL)
{
    this->parent = nodeObject;
    
    addAndMakeVisible(&this->spectrogramView);
    
    this->spectrogram = std::make_shared<IRSpectrogram>(nodeObject);
    
    this->verticalGrid.setRange(0, 40);
    this->verticalGrid.addMouseListener(this, false);
    
    this->horizontalGrid.setRange(0, 40);
    this->horizontalGrid.addMouseListener(this, false);
    
    this->componentForViewPort = std::make_shared<ComponentForViewPort>(this->spectrogram.get(),
                                                                        &this->verticalGrid,
                                                                        &this->horizontalGrid,
                                                                        this->gridSize);
    
    this->spectrogramView.setViewedComponent(this->componentForViewPort.get());
    this->spectrogramView.visibleAreaChangedCallback = [this](Rectangle<int> area){ visibleAreaChanged(area); };

}

IRSpectrogramUI::~IRSpectrogramUI()
{
    if(this->audioData != nullptr)
        getFileManager()->discardFilePtr(IRFileType::IRAUDIO, this->audioData, this->parent, this->file);
    
    this->spectrogram->closeOpenGLComponent();
    this->componentForViewPort.reset();
    this->spectrogram.reset();
}


// ==================================================

void IRSpectrogramUI::paint(Graphics& g)
{
    g.fillAll(SYSTEMCOLOUR.background);
    
    g.setColour (SYSTEMCOLOUR.contents);
    //g.drawRoundedRectangle (area.toFloat(), 5.0f, 4.0f);
    g.drawRect(getLocalBounds().toFloat(), 1.0);
    
    
}
void IRSpectrogramUI::resized()
{
    int x = this->xMargin;
    int y = this->yMargin;
    int w = getWidth() - this->xMargin*2;
    int h = getHeight() - this->yMargin*2;
    
    int spectrogramMarginY = 20;
    
    this->spectrogramView.setBounds(x, y, w, h);
    this->spectrogramView.setViewPosition(this->previousOffsetX, 0);
    
    
    
    
    this->componentForViewPort->setBounds(0,
                                          0,
                                          w * this->spectrogram_width_ratio,
                                          h - spectrogramMarginY);
    

}
// ==================================================

void IRSpectrogramUI::setMovable(bool movable, bool verticalMovable, bool horizontalMovable)
{
    /*
    Array<vertex*> vs = this->automation->getVerteces();
    
    for(auto v : vs)
    {
        v->setMovable(movable, verticalMovable, horizontalMovable);
    }
    
    this->automation->setMovable(movable, verticalMovable, horizontalMovable);
 */
}

// ==================================================

void IRSpectrogramUI::visibleAreaChanged(Rectangle<int> area)
{
    this->spectrogram->setVisibleArea(area);
    this->componentForViewPort->setVisibleArea(area);
    
    this->previousOffsetX = area.getX();
}

// ==================================================


void IRSpectrogramUI::openFile()
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

void IRSpectrogramUI::openFile(String path)
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

void IRSpectrogramUI::getFilePtr(File file)
{
    
    std::cout << "getFilePtr\n";
    // set a callback function which is called when file load is completed.
    // get a pointer of the audio file
    std::function<void()> callback = [this]{fileImportCompleted();};
    
    
    getFileManager()->getFilePtrWithCallBack(IRFileType::IRAUDIO,
                                             file,
                                             this->parent,
                                             callback);
    
    // notify changes to IRProject to update IRFileInspecter
    this->parent->notifyNodeObjectModification();
    
}

void IRSpectrogramUI::fileImportCompleted()
{
    this->audioData = static_cast<DataAllocationManager<IRAudio>*>(getFileManager()->getFileObject());
    
    this->status = AudioFileImportCompleted;
    sendChangeMessage();
    
    fileImportCompletedAction();
    
    std::cout << "fileImportCompleted!\n";
    
}
