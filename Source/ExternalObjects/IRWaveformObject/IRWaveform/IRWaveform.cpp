
#include "IRWaveform.hpp"





IRWaveform::IRWaveform()
:   thumbnailCache(5),
thumbnail(512, formatManager, thumbnailCache)
{
    
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
    
    this->audioFile.addChangeListener(this);
    this->audioFile.addListener(this);
    
    this->player = new SoundPlayerClass();
    this->player->addChangeListener(this);
    
}


IRWaveform::~IRWaveform()
{
    // delete this->player; // leave it - the owner will delete it.
}


// OpenAudioFile
// # DO NOT write any task expected to be done after opening file here!
// # write it in changeListenerCallback method.
void IRWaveform::openFile()
{
    if (! this->audioFile.openFile()){
        std::cout << "Error : could not open\n";
    }
}


void IRWaveform::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &this->audioFile)
    {
        if (this->audioFile.isFileLoadCompleted)
        {
            std::cout << "open audio file success\n";
            String path = this->audioFile.getPath();
            
            String pathToOpen;
            pathToOpen.swapWith(path);
            std::cout << pathToOpen << std::endl;
            
            if(pathToOpen.isNotEmpty())
            {
                
                {
                    const ScopedLock sl (this->callbackLock); // lock thread
                    File file (pathToOpen);
                    
                    this->thumbnail.setSource(new FileInputSource(file));
                    
                    // initialize display duration if no user defined duration is set.
                    if(this->duration == 0)
                        this->duration = this->thumbnail.getTotalLength();
                    
                    removeChildComponent(&this->openButton);
                    this->drawWaveform = true;
                    repaint();
                }
                
            }
        }
    }
    else if (source == this->player)
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


// this method is called when the file import completed.
void IRWaveform::fileImportCompleted(IRAudio *obj)
{
    //set audioBuffer to player
    std::vector<int>v = {0,1};
    this->player->setAudioBuffer(this->audioFile.getAudioBuffer(), false, this->audioFile.getSampleRate(),v);
    //this->player->setParameters(44100, 22050, 44100, false);
    
}


//this method is called when file status changed
void IRWaveform::fileStatusChanged(IRAudio *obj)
{
    
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
        
        //std::cout<<"paint duration = " << this->duration << std::endl;
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
    return this->audioFile.getSampleRate();
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




