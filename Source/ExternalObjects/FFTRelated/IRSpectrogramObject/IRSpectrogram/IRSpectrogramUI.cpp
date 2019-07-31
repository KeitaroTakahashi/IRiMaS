//
//  IRSpectrogramUI.cpp
//  IRiMaS
//
//  Created by Keitaro on 08/07/2019.
//

#include "IRSpectrogramUI.hpp"

IRSpectrogramUI::IRSpectrogramUI(IRNodeObject* nodeObject) :
IRUIFoundation(nodeObject)
{
    this->parent = nodeObject;
    
    this->spectrogram = std::make_shared<IRSpectrogramComponent>(nodeObject);
    this->spectrogram->getComponent()->zoomInOutSharedCallback = [this]{ zoomInoutSharedAction(); };
    
    this->spectrogram->getComponent()->currentPlayedFrameSharedCallback = [this] { currentPlayedFrameSharedAction(); };
    this->spectrogram->getComponent()->viewPortPositionSharedCallback = [this] { viewPortPositionSharedAction(); };
    
    this->spectrogramView = std::make_shared<IRSpectrogramViewUI>(this->spectrogram.get(),
                                                       0, 40,
                                                       0, 40);
    addAndMakeVisible(this->spectrogramView.get());

    this->spectrogramView->visibleAreaChangedCallback = [this](Rectangle<int> area){ visibleAreaChanged(area);};
    
    this->spectrogram->getComponent()->zoomInClickedCallback = [this]{ zoomInClicked(); };
    this->spectrogram->getComponent()->zoomOutClickedCallback = [this]{ zoomOutClicked(); };

}

IRSpectrogramUI::~IRSpectrogramUI()
{
    if(this->audioData != nullptr)
        getFileManager()->discardFilePtr(IRFileType::IRAUDIO, this->audioData, this->parent, this->file);
    
    this->spectrogram->getComponent()->closeOpenGLComponent();
    this->spectrogram.reset();
    this->spectrogramView.reset();
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
    zoomResize();
    this->spectrogram->getComponent()->linkZoomInfo(nodeObject);

}


// ==================================================

void IRSpectrogramUI::heavyComponentRefreshed()
{
    this->spectrogram->getComponent()->heavyComponentRefreshed();
}

// ==================================================


void IRSpectrogramUI::zoomResize()
{
    int x = this->xMargin;
    int y = this->yMargin;
    int w = getWidth() - this->xMargin*2;
    int h = getHeight() - this->yMargin*2;
    
    int spectrogramMarginY = 10;
    
    this->spectrogramView->setBounds(x, y, w, h);
    this->spectrogramView->setViewPosition(this->previousOffsetX, 0);
    this->spectrogramView->setComponentBounds(0,
                                              0,
                                              w * this->spectrogram_width_ratio,
                                              h - spectrogramMarginY);
    
    this->spectrogram->setMainComponentBounds(Rectangle<int>(0,0,w-20,h-30));

    this->spectrogram->getComponent()->setZoomInfo(Point<float>(this->spectrogram_width_ratio, 1.0));
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
    //std::cout << "IRSpectrogramUI::visibleAreaChanged\n";
    this->spectrogramView->setVisibleArea(area);
    this->spectrogram->setVisibleArea(area);

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
// ==================================================
//controller

void IRSpectrogramUI::zoomInClicked()
{
    std::cout << "IRSpectrogramUI zoomInClicked!! " << spectrogram_width_ratio << std::endl;
    this->spectrogram_width_ratio *= 2;
    resized();
}
void IRSpectrogramUI::zoomOutClicked()
{
    this->spectrogram_width_ratio /= 2;
    if(this->spectrogram_width_ratio < 1.0)
        this->spectrogram_width_ratio = 1.0;
    resized();
}


// ==================================================

void IRSpectrogramUI::zoomInoutSharedAction()
{
    this->spectrogram_width_ratio = this->spectrogram->getComponent()->getZoomInfo().getX();
    std::cout << "changeListenerCallback zoomInClicked!! " << spectrogram_width_ratio << std::endl;
    
    zoomResize();
}

void IRSpectrogramUI::currentPlayedFrameSharedAction()
{
    
}

void IRSpectrogramUI::viewPortPositionSharedAction()
{
    auto viewPos = this->spectrogram->getComponent()->getVisibleArea();
    this->spectrogramView->setViewPosition(viewPos.getX(), viewPos.getY());

}

void IRSpectrogramUI::changeListenerCallback (ChangeBroadcaster* source)
{
   
}
