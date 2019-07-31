
#include "IRWaveform.hpp"





IRWaveform::IRWaveform(IRNodeObject* parent) :
IRUIAudioFoundation(parent),
thumbnailCache(5),
thumbnail(512, formatManager, thumbnailCache),
visiblePos(Point<int>(0,0)),
playingLine(0,0,0,0)
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
    
    setFps(17);
    
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
    {
        this->audioData->getData()->removeListener(this);
        getFileManager()->discardFilePtr(IRFileType::IRAUDIO, this->audioData, this->parent, this->file);
    }
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
        
        //std::cout << "thumbnail changeMessage chan = " << this->thumbnail.getNumChannels() << std::endl;
        this->drawWaveform = true;

        repaint();
    }
}

void IRWaveform::makeThumbnail(String path)
{
    //{
      //  const ScopedLock sl (this->callbackLock); // lock thread
        File file (path);
        
        std::cout << "thumnail = " << path << std::endl;
        this->thumbnail.setSource(new FileInputSource(file));
        
        // initialize display duration if no user defined duration is set.
        if(this->duration == 0)
            this->duration = this->thumbnail.getTotalLength();
        
        removeChildComponent(&this->openButton);
        //repaint();
    //}
}
//==========================================================================
// Callback functions
//==========================================================================

void IRWaveform::fileImportCompleted()
{

    std::cout << "IRWaveform : fileImportCompleted\n";
    this->audioData = static_cast<DataAllocationManager<IRAudio>*>(getFileManager()->getFileObject());
    
    this->audioData->getData()->addListener(this);
    
    //set audioBuffer to player
    std::vector<int>v = {0,1};
    this->player->setAudioBuffer(this->audioData->getData()->getAudioBuffer(), false, this->audioData->getData()->getSampleRate(),v);
    
   // set received Ptr to the Link System
    this->parent->setAudioLink(this->audioData->getData());

    makeThumbnail(file.getFullPathName());

}


void IRWaveform::resized()
{
    this->openButton.setBounds(0,0,getWidth(),getHeight());
}


void IRWaveform::paint(Graphics& g)
{
    //std::cout << "+++++ IRWaveform repaint() +++++ \n";
    if (this->drawWaveform)
    {
        auto area = getLocalBounds();//.reduced (2);
        
        g.setColour (SYSTEMCOLOUR.background);
        //g.fillRoundedRectangle(area.toFloat(), 5.0f);
        g.fillRect(area.toFloat());
        
        g.setColour (SYSTEMCOLOUR.contents);
        //g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
        g.drawRect(area.toFloat(), 1.0);
        
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
        
        // line
        g.setColour(Colours::red);
        g.drawLine(this->playingLine.getX(),
                   this->playingLine.getY(),
                   this->playingLine.getWidth(),
                   this->playingLine.getHeight(),
                   3.0);
        
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
    
    
    startAnimation();
}


void IRWaveform::stop()
{
    this->player->setLooping(false);
    this->player->stop();
    
    stopAnimation();
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



// --------------------------------------------------
void IRWaveform::linkZoomInfo(Component* comp)
{
    if(this->audioData != nullptr)
    {
        auto data = this->audioData->getData();
        data->linkZoomInOutWithSharedComponents(comp);
    }
}

void IRWaveform::linkCurrentPlayedFrame(Component* comp)
{
    if(this->audioData != nullptr)
    {
        auto data = this->audioData->getData();
        data->linkAudioPlaywithSharedComponents(comp);
    }
}


void IRWaveform::linkViewPosition(Component* comp)
{
    if(this->audioData != nullptr)
    {
        auto data = this->audioData->getData();
        data->setViewPortPosition(this->visiblePos);
        data->linkViewPortPositionWithSharedComponents(comp);
    }
}


// --------------------------------------------------


void IRWaveform::zoomInOutOperatedFromComponent(IRAudio* obj)
{
    auto comp = obj->getEmittingComponent();
    // check if the emmiting component is not this object otherwise we will face on the infinitive loop
    if(comp != nodeObject)
    {
        
        setZoomInfo(obj->getZoomInfo());
        //std::cout << "zoominfo of "<< nodeObject << " = " << this->zoomInfo.getX() <<  " from " << comp << std::endl;
        
        this->status = zoomInfoShared;
        sendChangeMessage();
       
    }else{
        //std::cout <<"zoomInfo same \n";
    }
}
void IRWaveform::audioPlayOperatedFromComponent(IRAudio* obj)
{
    
    auto comp = obj->getEmittingComponent();
    // check if the emmiting component is not this object otherwise we will face on the infinitive loop
    if(comp != nodeObject)
    {
        // stop pther playing process in this case to avoid unneccesary lisk
        if(this->player->isPlaying()) this->player->stop();
        
        
        setCurrentPlayedFrame(obj->getCurrentPlayedFrame());
        createPlayingLine(obj->getCurrentPlayedFrame());
        this->status = currentPlayedFrameShared;
        sendChangeMessage();
    }
}

void IRWaveform::viewPortPositionFromComponent(IRAudio *obj)
{
    auto comp = obj->getEmittingComponent();
    // check if the emmiting component is not this object otherwise we will face on the infinitive loop
    if(comp != nodeObject)
    {
        this->visiblePos = obj->getViewPortPosition();
        this->status = viewPosShared;
        sendChangeMessage();
    }
}

// --------------------------------------------------

void IRWaveform::updateAnimationFrame()
{
    if(this->audioData != nullptr)
    {
        if(this->player->isPlaying())
        {
            auto data = this->audioData->getData();
            int64 playingPosition = this->player->getStartPosition() +
            this->player->getNextReadPosition();
            
            data->setCurrentPlayedFrame(playingPosition);
            
            data->linkAudioPlaywithSharedComponents(nodeObject);
            
            createPlayingLine(playingPosition);
        }
    }
}

void IRWaveform::createPlayingLine(int64 currentFrame)
{
    if(this->audioData != nullptr)
    {
        auto data = this->audioData->getData();
        double sampleRate = data->getSampleRate();
        double duration = getDisplayDuration();
        double start = getStart();
        
        double p = (double)currentFrame - (start * sampleRate);
        
        if(p > 0)
        {
            double w = sampleRate * duration;
            
            float ratio = p / w;
            
            int x_pos = floor((float)getWidth() * ratio);
            
            this->playingLine = Rectangle<int>(x_pos, 0, x_pos, getHeight());
            repaint();

        }
    }
}
