//
//  IRFFTSequence.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 05/09/2018.
//

#ifndef IRFFTSequence_h
#define IRFFTSequence_h
#include <iostream>
#include <math.h>
#include <vector>
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
        this->complexResult.reserve(getNumFrame());
        fftForwardSetup();
        for(int i=0; i<getNumFrame(); i++)
        {
            
            setSamples(&this->frameData[i][0], 0, this->fftsize);
            fftExecute();
            this->complexResult.push_back(copyComplexData(getComplexResult()));
        }
        destroySetup();
        
        this->hasFFTOperatedFlag = true;
        
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
        this->maxPower.reserve(getNumFrame());
        this->power.clear();
        this->power = std::vector<std::vector<float>>(getNumFrame(),
                                                      std::vector<float>(this->ffthalfsize));
        /*
        this->phase.clear();
        this->phase = std::vector<std::vector<float>>(getNumFrame(),
                                                      std::vector<float>(this->ffthalfsize));
         */
        
        std::cout << "FFT cartopol getNumFrame = " << getNumFrame() << std::endl;
        float max = 0;
        for(i=0;i<getNumFrame();i++)
        {
            fftw_complex* c = this->complexResult[i];
            max = 0;
            for(j=0;j<this->ffthalfsize;j++){
                this->power[i][j] = (sqrt(c[j][0] * c[j][0] + c[j][1] * c[j][1]));
                if(this->power[i][j] > max) max = this->power[i][j];
            }
            this->maxPower.push_back(max);
        }
    }
    
    void calcDescriptors();
    
    // ------------------------------------------------------------
    int getNumFrame() const { return (int)this->frameData.size(); }
    std::vector<fftw_complex*> getComplexResultList() { return this->complexResult; }
    std::vector<std::vector<float>> getPower() const { return this->power; }
    std::vector<float> getMaxPower() const { return this->maxPower; }
    
    bool hasFFTOperated()               const { return this->hasFFTOperatedFlag; }

    int getHopSize() const { return this->hopsize; }
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
        
        std::cout << "audio segment for FFT made. nframe = " << this->frameData.size() << " : fftsize = " << getFFTSize() << " : hopsize = " << getHopSize() <<std::endl;
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
    // ============================================================

    bool hasFFTOperatedFlag = false;
    
    


};
#endif /* IRFFTSequence_h */
