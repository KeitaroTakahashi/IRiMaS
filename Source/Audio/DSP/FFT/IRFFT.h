//
//  IRFFT.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 03/09/2018.
//

#ifndef IRFFT_h
#define IRFFT_h

#include "fftw3.h"

class IRFFT
{
public:
    IRFFT(int fftsize)
    : fftsize(fftsize)
    {
        this->ffthalfsize = this->fftsize/2;
        allocateMemory();
    }
    // ------------------------------------------------------------
    ~IRFFT()
    {
        freeMemory();
    }
    // ------------------------------------------------------------
    void setFFTSize(int fftsize)
    {
        freeMemory();
        this->fftsize = fftsize;
        allocateMemory();
    }
    // ------------------------------------------------------------
    void fftForwardSetup()
    {
        this->p = fftw_plan_dft_1d(this->fftsize, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    }
    // ------------------------------------------------------------
    void fftBackwardSetup()
    {
        this->p = fftw_plan_dft_1d(this->fftsize, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
    }
    // ------------------------------------------------------------
    void destroySetup()
    {
        fftw_destroy_plan(p);
    }
    // ------------------------------------------------------------
    void fftExecute()
    {
        fftw_execute(this->p);
    }
    // ------------------------------------------------------------
    /*
    void setSamples(float* buffer, int begin, int length = -1)
    {
        if(length == -1) length = this->fftsize;
        else if(length < 0) return;
        
        for(int i=0;i<length;i++){
            in[i][0] = buffer[begin + i];
            in[i][1] = 0;
        }
    }*/
    
    void setSamples(float* buffer, int length)
    {
        for(int i=0;i<length;i++)
        {
            in[i][0] = buffer[i];
            in[i][1] = 0;
        }
    }
    // ------------------------------------------------------------
    fftw_complex* getComplexResult() { return this->out; }
    
    fftw_complex* copyComplexData(fftw_complex* src)
    {
        fftw_complex* dst = (fftw_complex* ) fftw_malloc(sizeof(fftw_complex)*this->fftsize);
        memcpy(dst, src, sizeof(fftw_complex*)*this->fftsize);
        return dst;
    }
    // ------------------------------------------------------------
    int getFFTSize() const { return this->fftsize; }
private:
    // ------------------------------------------------------------
    void allocateMemory()
    {
        this->in = (fftw_complex* ) fftw_malloc(sizeof(fftw_complex)*this->fftsize);
        this->out = (fftw_complex* ) fftw_malloc(sizeof(fftw_complex)*this->fftsize);
    }
    // ------------------------------------------------------------

    void freeMemory()
    {
        fftw_free(this->in);
        fftw_free(this->out);
    }
    
public:
    int fftsize;
    int ffthalfsize;
    
private:
    fftw_complex *in, *out;
    fftw_plan p;
    int direction;
    
};

#endif /* IRFFT_h */
