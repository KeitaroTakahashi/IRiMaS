//
//  IRAudioDescriptor.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/04/2019.
//

#ifndef IRAudioDescriptor_hpp
#define IRAudioDescriptor_hpp
#include <stdarg.h>

#include "IRFFTSequence.h"



class IRFFTDescriptor : public IRFFTSequence
{
public:
    
    enum FFTDescriptor{
        FFT_MAGNITUDE,
        FFT_PITCH,
        FFT_CENTROID,
        FFT_SPREAD,
        FFT_MFCCS,
        FFT_BFCCS
    };
    
    IRFFTDescriptor(int fftsize, int hopsize, IRWindow::TYPE windowType);
    ~IRFFTDescriptor();
    
    // @param1 number of argument
    // @param2... IRFFTDescriptor::FFTDescriptor 
    void setDescriptor(int arg_num, ...);
    
private:
    
    std::vector<FFTDescriptor> descriptorList;
    
};

#endif /* IRAudioDescriptor_hpp */
