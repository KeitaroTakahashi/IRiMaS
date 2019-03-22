//
//  IRSpectrogram.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 03/09/2018.
//

#ifndef IRSpectrogram_h
#define IRSpectrogram_h

#include "IRFoundation.h"
#include "IRAudioReader.hpp"
#include "SoundPlayer.hpp"

#include "IRFFTSequence.h"

class IRSpectrogram : public AudioAppComponent,
                      public ChangeBroadcaster,
                      private ChangeListener,
                      private IRAudio::Listener
{
public:
    IRSpectrogram()
    {
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
        
        this->FFTSequence = new IRFFTSequence(4096, 2048, IRWindow::TYPE::HAMMING);
    }
    
    ~IRSpectrogram()
    {
        delete this->player;
        delete this->FFTSequence;
    }
    //==========================================================================

    struct vertex2d{
        int x;
        float y;
        
        vertex2d(int x, float y) : x(x), y(y) {}
    };
    
    struct vertex3d{
        int x;
        int y;
        float z;
        vertex3d(int x, int y, float z) : x(x), y(y), z(z) {}
    };
    // ------------------------------------------------------------

    struct spectrumStr{
        std::vector<vertex2d>maxBuffer;
        std::vector<vertex2d>minBuffer;
        spectrumStr() {}
        spectrumStr(std::vector<vertex2d> maxBuffer, std::vector<vertex2d>minBuffer) :
        maxBuffer(maxBuffer), minBuffer(minBuffer)
        {}
    };
    // ------------------------------------------------------------
   
    struct vertex3Str{
        std::vector<vertex3d> maxBuffer;
        std::vector<vertex3d> minBuffer;
        
        vertex3Str() {}
        vertex3Str(std::vector<vertex3d> maxBuffer, std::vector<vertex3d> minBuffer) :
        maxBuffer(maxBuffer), minBuffer(minBuffer)
        {}
    };
    
    typedef std::vector<vertex3Str> sonogramStr;
    
    
    //==========================================================================
    // OpenAudioFile
    // # DO NOT write any task expected to be done after opening file here!
    // # write it in changeListenerCallback method.
    void openFile()
    {
        if(! this->audioFile.openFile()){
            std::cout << "Error : could not open\n";
        }
    }
    //==========================================================================
    
    /*
    std::vector<float> getSpectrumAtIndex(int index)
    {
        if(index < (this->FFTSequence->getNumFrame()-1))
        {
            
        }
        
        // return std::vector<float>; // FD QUICK FIX TO PREVENT WRONG TYPE
    }
     */

    void makeSpectrum()
    {
        if(this->FFTSequence != nullptr)
        {
            if(this->FFTSequence->getNumFrame() > 0)
            {
                this->FFTSequence->FFT();
            }
        }
        
        this->FFTSequence->cartopol();
        /*
        std::cout << "______ " << std::endl;
        for(int i=0;i<this->FFTSequence->ffthalfsize; i++){
            std::cout << this->FFTSequence->getPower()[10][i] << std::endl;
        }*/
    }
    
    //==========================================================================

    // ------------------------------------------------------------
    // callback functions
    // ------------------------------------------------------------
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        if(source == &this->audioFile)
        {
            if(this->audioFile.isFileLoadCompleted)
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
                        removeChildComponent(&this->openButton);
                        this->drawSpectrum = true;
                        repaint();
                    }
                    
                }
            }
        }else if(source == this->player)
        {
            if(this->player->isPlaying())
            {
                
            }else if(this->player->isPausing())
            {
                
            }else{ // stop
                
            }
        }
    }
    //  ------------------------------------------------------------
    // # IRAudioReader::Listener
    //  ------------------------------------------------------------
    // this method is called when the file import completed.
    void fileImportCompleted(IRAudio *obj) override
    {
        std::cout << "fileImportCompleted." << std::endl;
        //set audioBuffer to player
        //std::vector<int>v = {0,1};
        //this->player->setAudioBuffer(this->audioFile.getAudioBuffer(), false, this->audioFile.getSampleRate(),v);
        //this->player->setParameters(44100, 22050, 44100, false);
        
        std::vector<float> buffer = this->audioFile.getAudioBufferInVector();
        
        this->FFTSequence->setAudioData(buffer);

        makeSpectrum();
        
        if(this->start == 0)
        {
            this->start = 0;
            this->duration = this->FFTSequence->ffthalfsize/4;
        }

    }
    //  ------------------------------------------------------------
    //this method is called when file status changed
    void fileStatusChanged(IRAudio *obj) override
    {
        
    }
    //==========================================================================
    //  ------------------------------------------------------------
    virtual void resized() override
    {
        this->openButton.setBounds(0,0,getWidth(),getHeight());
    }
    //  ------------------------------------------------------------
    void paint(Graphics& g)override
    {
        if(this->drawSpectrum)
        {
            auto area = getLocalBounds();//.reduced (2);
            
            g.setColour (SYSTEMCOLOUR.contents);
            g.fillRoundedRectangle(area.toFloat(), 5.0f);
            
            g.setColour(SYSTEMCOLOUR.fundamental);
            drawSpectrumChannel(g, area, this->start, this->duration, 1.0, 10);

            //drawSpectrumChannel(g, area, 0, this->FFTSequence->ffthalfsize/4, 1.0, 10);
            //drawSonogramChannel(g, area, 0, this->FFTSequence->ffthalfsize/4, 0, this->FFTSequence->getNumFrame(), 1.0);
        }
    }
    //  ------------------------------------------------------------
    void drawSpectrumChannel(Graphics& g, const Rectangle<int>& area,
                             const int startBin, const int endBin,
                             const float verticalZoomFactor,
                             const int powerIndex)
    {
        jassert(startBin < endBin);
        const int nBin = endBin - startBin;
        

        spectrumStr spectrumBuf = makeSpectrum(area, startBin, endBin, verticalZoomFactor, powerIndex);
        
        std::vector<vertex2d> maxBuf = spectrumBuf.maxBuffer;
        std::vector<vertex2d> minBuf = spectrumBuf.minBuffer;

        
        for(int i=0; i<(nBin-1); i++)
        {
            float minY = area.getHeight() - (minBuf[i].y) * area.getHeight();
            float minY2 = area.getHeight() - (minBuf[i+1].y) * area.getHeight();

            float maxY = area.getHeight() - (maxBuf[i].y) * area.getHeight();
            
            g.drawLine(minBuf[i].x, round(minY), maxBuf[i].x, round(maxY));
            g.drawLine(maxBuf[i].x, round(maxY), minBuf[i+1].x, round(minY2));
        }
    }
    //  ------------------------------------------------------------
    spectrumStr makeSpectrum(const Rectangle<int>& area,
                             const int startBin, const int endBin,
                             const float verticalZoomFactor,
                             const int powerIndex)
    {
        const int nBin = endBin - startBin;
        
        std::vector<vertex2d> maxBuf(nBin, vertex2d(0,-9999));
        std::vector<vertex2d> minBuf(nBin, vertex2d(0,9999));
        
        float pixelPerSample = (float)area.getWidth() / (float)nBin;

        std::vector<float> powerBuf = this->FFTSequence->getPower()[powerIndex];
        float maxPower = this->FFTSequence->getMaxPower()[powerIndex];

        int i;
        for(i = 0; i < nBin; i++)
        {
            float x = (float)i * pixelPerSample;
            int index = roundf(x);
            
            float power = powerBuf[i + startBin] / maxPower;

            if(maxBuf[i].y < power) maxBuf[i] = vertex2d(index, power);
            if(minBuf[i].y > power) minBuf[i] = vertex2d(index, power);
            
        }
        
        return spectrumStr(maxBuf, minBuf);
    }
    //  ------------------------------------------------------------
    sonogramStr makeSonogram(const Rectangle<int>& area,
                                          const int startBin, const int endBin,
                                          const int startIndex, const int endIndex,
                                          const float verticalZoomFactor)
    {
        const ScopedLock sl (callbackLock);
        
        jassert(startBin < endBin);
        jassert(startIndex < endIndex);
        
        const int nBin = endBin - startBin;
        const int nIndex = endIndex - startIndex;

        float pixelPerSampleH = (float)area.getHeight() / (float)nBin;
        float pixelPerSampleW = (float)area.getWidth()  / (float)nIndex;

        //dummy
        std::vector<vertex3d> min (nBin, vertex3d(0, 0, -9999));
        std::vector<vertex3d> max (nBin, vertex3d(0, 0, 9999));
        // sonogram
        sonogramStr Sonogram(nIndex, vertex3Str(max, min));
        
        int i,j;
        //sonogramStr sonogram;
        /*
        for(i = 0; i < nIndex; i++)
        {
            float x = (float)i * pixelPerSampleW;
            int xIndex = round(x);
            
            // spectrum
            spectrumStr spectrumBuf = makeSpectrum(area, startBin, endBin, verticalZoomFactor, i);
            std::vector<vertex2d> maxBuf = spectrumBuf.maxBuffer;
            std::vector<vertex2d> minBuf = spectrumBuf.minBuffer;
            
            for(j = 0; j < nBin; j++)
            {
                float maxPower = maxBuf[j].y;
                int maxY = maxBuf[j].x;
                float minPower = minBuf[j].y;
                int minY = minBuf[j].x;
 
                if(Sonogram[i].maxBuffer[j].y < maxPower) Sonogram[i].maxBuffer[j] = vertex3d(xIndex, maxY, maxPower);
                if(Sonogram[i].minBuffer[j].y > minPower) Sonogram[i].minBuffer[j] = vertex3d(xIndex, minY, minPower);
 
            }
        }*/
        
        std::vector<std::vector<float>> power = this->FFTSequence->getPower();
        std::vector<float> maxPower = this->FFTSequence->getMaxPower();
        for(i=0;i<nIndex;i++)
        {
            float x = (float)i * pixelPerSampleW;
            int xIndex = round(x);
            
            for(j=startBin;j<endBin;j++)
            {
                float p = power[i][j] / maxPower[i];
                float y = (float)j * pixelPerSampleH;
                int yIndex = round(y);
                this->sonogramBuffer.push_back(vertex3d(xIndex, yIndex, p));
            }
        }
        
        
        return Sonogram;
    }
    
    void drawSonogramChannel(Graphics& g, const Rectangle<int>& area,
                             const int startBin, const int endBin,
                             const int startIndex, const int endIndex,
                             const float verticalZoomFactor)
    {
        jassert(startBin < endBin);
        const int nBin = endBin - startBin;
        const int nIndex = endIndex - startIndex;
        
        // const float yIncrement = (area.getHeight() / nBin);
        const float xIncrement = (area.getWidth() / nIndex);
        
        sonogramStr sonogramBuf = makeSonogram(area,
                                               startBin, endBin,
                                               startIndex, endIndex,
                                               verticalZoomFactor);
        

        std::vector<vertex3d> s = this->sonogramBuffer;

        int i;
        for(int w = 0; w < (nIndex-1); w++)
        {
            //std::vector<vertex3d> maxBuf = sonogramBuf[w].maxBuffer;
            //std::vector<vertex3d> minBuf = sonogramBuf[w].minBuffer;
            
            int width = round((float)w * xIncrement);
            for(i=0; i<(nBin-1); i++)
            {
                
                //vertex3d max3d = maxBuf[i];
                //vertex3d min3d = minBuf[i];
                int index = w * nBin;
                
                int x = s[index+i].x;
                int y = area.getHeight() - s[index+i].y;
                
                unsigned char c1 = (unsigned char)(255.0*s[index + i].z);
                g.setColour(Colour(c1, c1, c1));
                g.drawRect(x, y, width, 1);
                
                /*
                unsigned char c1 = (unsigned char)(255*maxBuf[i].z);
                g.setColour(Colour(c1, c1, c1));
                g.drawRect(max3d.x, area.getHeight() - max3d.y, width, 1);
                unsigned char c2 = (unsigned char)(255*minBuf[i].z);
                g.setColour(Colour(c2, c2, c2));
                g.drawRect(min3d.x, area.getHeight() -  min3d.y, width, 1);
                */
            

            }
        }
 
    }
    //  ------------------------------------------------------------
    void setStart(double ms) { this->start = ms; }
    double getStart() const { return this->start; }
    //  ------------------------------------------------------------
    void setDisplayDuration(double ms) { this->duration = ms; }
    double getDisplayDuration() const { return this->duration; }
    
    double getTotalLength() const {
        if(this->drawSpectrum)
            return this->FFTSequence->getFFTSize();
        else return 0;
    }
    //  ------------------------------------------------------------
    void setChannelIndex(int channelIndex) { this->channelIndex = channelIndex; }
    //  ------------------------------------------------------------
    void setZoomFactor(double zoomFactor) { this->zoomFactor = zoomFactor; }
    double getZoomFactor() const { return this->zoomFactor; }
    //  ------------------------------------------------------------
    double getSampleRate() const { return this->audioFile.getSampleRate(); }
    //  ------------------------------------------------------------
    bool isWaveformPainted() const { return this->drawSpectrum; }
    //  ------------------------------------------------------------
    
    //==========================================================================
    // AudioAppComponent
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        this->player->prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
    virtual void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override
    {
        this->player->getNextAudioBlock(bufferToFill);
    }
    virtual void releaseResources() override
    {
        this->player->releaseResources();
    }
    
    // ------------------------------------------------------------

    SoundPlayerClass* getPlayer() const { return this->player; }
    //==========================================================================

    IRAudio audioFile;
    SoundPlayerClass *player;
    
private:
    

    
    TextButton openButton;
    
    bool drawSpectrum = false;
    
    AudioFormatManager formatManager;
    
    CriticalSection callbackLock;
    
    IRFFTSequence* FFTSequence;
    
    
    std::vector<vertex3d> sonogramBuffer;
    
    std::vector<vertex2d> maxBuffer;
    std::vector<vertex2d> minBuffer;
    
    std::vector<spectrumStr> spectrumBuffer;
    // ------------------------------------------------------------
    // parameters
    double start = 0.0;
    double duration = 0.0;
    int channelIndex = 0;
    double zoomFactor = 1.0;
    // ===========================================================================
    // system appearance
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    
};

#endif /* IRSpectrogram_h */
