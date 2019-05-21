
#include "IRWaveform.hpp"





IRWaveform::IRWaveform(IRNodeObject* parent) :
IRUIAudioFoundation(parent),
thumbnailCache(5),
thumbnail(512, formatManager, thumbnailCache)
{
 
    this->parent = parent;
    // We need to add this to the ChangeListener of thumbnail to repaint() waveform
    // this process is important when thumbnail calls setSource() and replace the wavedara and paint it, otherwise it does not complete painting.
    // in the changeListenerCallback(), we need to call repaint()
    this->thumbnail.addChangeListener(this);
    
    this->formatManager.registerBasicFormats();
    
    addAndMakeVisible(&this->openButton);
    this->openButton.setButtonText("open audio");
    
    this->openButton.setColour(TextButton::buttonColourId, SYSTEMCOLOUR.fundamental);
    this->setEnabled(true);
    this->openButton.onClick = [this]{ openFile(); };
    
    this->player = new SoundPlayerClass(parent);
    this->player->addChangeListener(this);
    
}


IRWaveform::~IRWaveform()
{
    // delete this->player; // leave it - the owner will delete it.
    // remove pointer
    deinitializeAudioData();
}

void IRWaveform::deinitializeAudioData()
{
    setStart(0);
    setDisplayDuration(0);
    
    if(this->audioData != nullptr)
        getFileManager()->discardFilePtr(IRFileType::IRAUDIO, this->audioData, this->parent, this->file);
}

// OpenAudioFile
// # DO NOT write any task expected to be done after opening file here!
// # write it in changeListenerCallback method.
/*
void IRWaveform::openFile()
{
    if (! this->audioFile.openFile()){
        std::cout << "Error : could not open\n";
    }
}
 */

void IRWaveform::openFile()
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

void IRWaveform::openFile(String path)
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

void IRWaveform::getFilePtr(File file)
{
    
    std::cout << "getFilePtr\n";
    // set a callback function which is called when file load is completed.
    // get a pointer of the audio file
    std::function<void()> callback = [this]{fileImportCompleted();};
    getFileManager()->getFilePtrWithCallBack(IRFileType::IRAUDIO,
                                             file,
                                             this->parent,
                                             callback);
    
    makeThumbnail(file.getFullPathName());
    
    // notify changes to IRProject to update IRFileInspecter
    this->parent->notifyNodeObjectModification();
    
}


void IRWaveform::changeListenerCallback (ChangeBroadcaster* source)
{
   if (source == this->player)
    {
        if (this->player->isPlaying())
        {
            
        }
        else if (this->player->isPausing())
        {
            
        }
        else
        { // stop
            
        }
    }
    else if (source == &this->thumbnail)
    {
        repaint();
    }
}

void IRWaveform::makeThumbnail(String path)
{
    {
        const ScopedLock sl (this->callbackLock); // lock thread
        File file (path);
        
        std::cout << "thumnail = " << path << std::endl;
        this->thumbnail.setSource(new FileInputSource(file));
        
        // initialize display duration if no user defined duration is set.
        if(this->duration == 0)
            this->duration = this->thumbnail.getTotalLength();
        
        removeChildComponent(&this->openButton);
        this->drawWaveform = true;
        repaint();
    }
}
//==========================================================================
// Callback functions
//==========================================================================

void IRWaveform::fileImportCompleted()
{
    this->audioData = static_cast<DataAllocationManager<IRAudio>*>(getFileManager()->getFileObject());
    //set audioBuffer to player
    std::vector<int>v = {0,1};
    this->player->setAudioBuffer(this->audioData->getData()->getAudioBuffer(), false, this->audioData->getData()->getSampleRate(),v);
    
   // set received Ptr to the Link System
    this->parent->setAudioLink(this->audioData->getData());

    
}


void IRWaveform::resized()
{
    this->openButton.setBounds(0,0,getWidth(),getHeight());
}


void IRWaveform::paint(Graphics& g)
{
    if (this->drawWaveform)
    {
        auto area = getLocalBounds();//.reduced (2);
        
        g.setColour (SYSTEMCOLOUR.contents);
        g.fillRoundedRectangle(area.toFloat(), 5.0f);
        
        g.setColour(SYSTEMCOLOUR.fundamental);
        //small margin
        Rectangle<int> thumbnailBounds (1,1, getWidth()-2, getHeight()-2);
        this->thumbnail.drawChannel(g,
                                    thumbnailBounds, // rounds rectangle
                                    this->start, // start in ms.
                                    this->start + this->duration, // end in ms.
                                    0, // channel index
                                    1.0f // zoom factor
                                    );
        
        std::cout<<"paint duration = " << this->duration << " : start = " << this->start << std::endl;
    }
}


void IRWaveform::setStart(double ms)
{
    this->start = ms;
}


double IRWaveform::getStart() const
{
    return this->start;
}


void IRWaveform::setDisplayDuration(double ms)
{
    this->duration = ms;
}


double IRWaveform::getDisplayDuration() const
{
    return this->duration;
}


double IRWaveform::getTotalLength() const
{
    if (this->drawWaveform)
        return this->thumbnail.getTotalLength();
    else return 0;
}


void IRWaveform::setChannelIndex(int channelIndex)
{
    this->channelIndex = channelIndex;
}


void IRWaveform::setZoomFactor(double zoomFactor)
{
    this->zoomFactor = zoomFactor;
}


double IRWaveform::getZoomFactor() const
{
    return this->zoomFactor;
}


double IRWaveform::getSampleRate() const
{
    return this->audioData->getData()->getSampleRate();
}


bool IRWaveform::isWaveformPainted() const
{
    return this->drawWaveform;
}


void IRWaveform::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    this->player->prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void IRWaveform::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    this->player->getNextAudioBlock(bufferToFill);
}


void IRWaveform::releaseResources()
{
    this->player->releaseResources();
}


// player control
void IRWaveform::play(int start, int duration, int offset, bool looping)
{
    this->player->setParameters(start, duration, offset, looping);
    this->player->setLooping(looping);
    this->player->start();
}


void IRWaveform::stop()
{
    this->player->setLooping(false);
    this->player->stop();
}


void IRWaveform::pausing()
{
    this->player->pause();
}


SoundPlayerClass* IRWaveform::getPlayer() const
{
    return this->player;
}


void IRWaveform::audioPtrDelivery(IRAudio *obj)
{
    std::cout << "audioPtrDelivery filename = " << obj->getFile().getFullPathName() << std::endl;
    //makeThumbnail(obj->getFile().getFullPathName());
    
    this->file = obj->getFile();

    getFilePtr(this->file);    
}


