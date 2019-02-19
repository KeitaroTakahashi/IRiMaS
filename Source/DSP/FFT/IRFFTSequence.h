//
//  IRFFTSequence.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 05/09/2018.
//

#ifndef IRFFTSequence_h
#define IRFFTSequence_h
#include "IRFFT.h"
#include "IRSignalWindowLib.h"

class IRFFTSequence : public IRFFT
{
    // ============================================================
public:
    IRFFTSequence(int fftsize, int hopsize, IRWindow::TYPE windowType) : IRFFT(fftsize),
    fftsize(fftsize),
    hopsize(hopsize),
    windowType(windowType),
    windowLib(fftsize, windowType)
    {
        
    }
    // ------------------------------------------------------------
    ~IRFFTSequence() {}
    // ------------------------------------------------------------
    void setAudioData(std::vector<float> data)
    {
        this->data = data;
        makeSegment();
    }

    void FFT()
    {
        this->complexResult.clear();
        fftForwardSetup();
        for(int i=0; i<getNumFrame(); i++)
        {
            setSamples(&this->frameData[i][0], 0, this->fftsize);
            fftExecute();
            this->complexResult.push_back(copyComplexData(getComplexResult()));
        }
        destroySetup();
    }
    
    void IFFT()
    {
        fftBackwardSetup();
        
        destroySetup();
    }
    
    void cartopol()
    {
        int i,j;
        this->maxPower.clear();
        this->power.clear();
        this->phase.clear();
    
        for(i=0;i<getNumFrame();i++)
        {
            fftw_complex* c = this->complexResult[i];
            std::vector<float>buf;
            float max = 0;
            for(j=0;j<this->ffthalfsize;j++){
                buf.push_back(sqrt(c[j][0] * c[j][0] + c[j][1] * c[j][1]));
                if(buf[j] > max) max = buf[j];
            }
            
            this->power.push_back(buf);
            this->maxPower.push_back(max);
        }
    }
    
    // ------------------------------------------------------------
    int getNumFrame() const { return (int)this->frameData.size(); }
    std::vector<fftw_complex*> getComplexResultList() { return this->complexResult; }
    std::vector<std::vector<float>> getPower() { return this->power; }
    std::vector<float> getMaxPower() const { return this->maxPower; }
    // ============================================================
private:
    
    void makeSegment()
    {
        int currentIndex = 0;
        int dataSize = (int)this->data.size();
        this->frameData.clear();
        
        do{
            if(currentIndex + this->fftsize < dataSize){
                // process windowing
                std::vector<float>vec = this->windowLib.windowingVector(std::vector<float>(&this->data[currentIndex],
                                                                                           &this->data[currentIndex + this->fftsize]));
                
               
                this->frameData.push_back(vec);

                currentIndex += this->hopsize;
            }else{
                std::vector<float>buf;
                int remainCount = 0;
                for(int i=currentIndex;i<currentIndex+this->fftsize;i++){
                    if(i<dataSize){
                        buf.push_back(data[i]);
                        remainCount ++;
                    }
                    else buf.push_back(0);
                }
                //process windowing
                std::vector<float>vec = this->windowLib.windowingVector(buf,remainCount);
                this->frameData.push_back(vec);
                break;
            }
        }while(true);
        
        std::cout << "audio segment for FFT made. nframe = " << this->frameData.size() << std::endl;
    }
    
    int fftsize;
    int hopsize;
    
    std::vector<float> data;
    std::vector<std::vector<float>> frameData;
    std::vector<fftw_complex*> complexResult;
    std::vector<std::vector<float>> power;
    std::vector<float> maxPower;
    std::vector<std::vector<float>> phase;
    
    IRWindow::TYPE windowType;
    
    IRWindow::IRSignalWindowLib windowLib;
};
#endif /* IRFFTSequence_h */
