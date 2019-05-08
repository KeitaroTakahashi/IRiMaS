
#include "IRImageViewer.hpp"

IRImageViewer::IRImageViewer(IRNodeObject* parent) :
IRUIFoundation(parent),
imgLoader(parent)
{
    this->parent = parent;
    setSize(100,100);
    addAndMakeVisible(&this->openButton);
    this->openButton.setButtonText("open image");
    
    // ===========================================================================
    // system appearance
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    this->openButton.setColour(TextButton::buttonColourId, SYSTEMCOLOUR.fundamental);
    this->setEnabled(true);
    this->openButton.onClick = [this]{ openFile(); };
    this->imgLoader.addChangeListener(this);
}


IRImageViewer::~IRImageViewer()
{
    
}


void IRImageViewer::paint(Graphics& g)
{
    
    if (this->imgRef != nullptr)
    {
        g.drawImageTransformed (this->imgRef->getImageData(),
                                AffineTransform::scale (getWidth()  / (float) this->imgRef->getWidth(),
                                                        getHeight() / (float) this->imgRef->getHeight()), false);
    }
    
}


void IRImageViewer::resized()
{
    this->openButton.setBounds(0,0,getWidth(),getHeight());
}


void IRImageViewer::openFile()
{
    bool isOk = this->imgLoader.open();
    if(!isOk) return;
    // receive pointer of the image file from FileManager
    this->imgRef = this->imgLoader.getData();
    
    if(this->imgRef == nullptr)
    {
        std::cout << "Image data not loaded!\n";
        return;
    }
    
    Point<int> fixedSize = this->imgLoader.sizeFix();
    
    setSize(fixedSize.getX(), fixedSize.getY());
    
    std::cout << "openFIle fixedSize = " << fixedSize.getX() << ", " << fixedSize.getY() << std::endl;
}


void IRImageViewer::openFile(String filePath)
{
    this->imgLoader.open(filePath);
    // receive pointer of the image file from FileManager
    this->imgRef = this->imgLoader.getData();
}


void IRImageViewer::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == &this->imgLoader)
    {
        if(this->imgLoader.isFileLoadCompleted)
        {
            removeChildComponent(&this->openButton);
            
            //inform parent the size change
            sendChangeMessage();
        }
    }
}


double IRImageViewer::getAspectRatio() const
{
    return this->imgLoader.getAspectRatio();
}


String IRImageViewer::getFilePath() const
{
    return this->imgLoader.getPath();
}


void IRImageViewer::setFilePath(String path)
{
    this->imgLoader.open(path);
}



