//
//  IRAudioDescriptor.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/04/2019.
//

#include "IRAudioDescriptor.hpp"

IRFFTDescriptor::IRFFTDescriptor(int fftsize, int hopsize, IRWindow::TYPE windowType):
IRFFTSequence(fftsize,hopsize,windowType)
{
    this->descriptorList.clear();
}

IRFFTDescriptor::~IRFFTDescriptor()
{
    this->descriptorList.clear();
}

void IRFFTDescriptor::setDescriptor(int arg_num, ...)
{
    this->descriptorList.clear();
    this->descriptorNameList.clear();
    
    va_list args;
    int count;
    if (arg_num < 1) return;

    va_start(args , arg_num);
    
    this->descriptorList.clear();
    for (count = 0 ; count < arg_num ; count++) {
        FFTDescriptor d = static_cast<FFTDescriptor>(va_arg(args , int));
        this->descriptorList.push_back(d);
        this->descriptorNameList.push_back(getDescriptorName(d));
    }
    va_end(args);

}

void IRFFTDescriptor::operateAnalysis()
{
    if(!hasFFTOperated())
    {
        
        if(getNumFrame() > 0)
        {
            FFT();
            cartopol();
        }else{
            std::cout << "Error : IRFFTDescriptor() : Could not operate FFT number of frame = " << getNumFrame() << std::endl;
        }
    }
    
    for(auto de : descriptorList)
    {
        calcDescriptor(de);
    }
     
}

void IRFFTDescriptor::calcDescriptor(FFTDescriptor descriptor)
{
    switch (descriptor)
    {
        case FFT_MAGNITUDE:
            calcCentroid();
            break;
        case FFT_CENTROID:
            calcCentroid();
            break;
        case FFT_SPREAD:
            calcSpread();
            break;
        case FFT_FLATNESS:
            calcFlatness();
            break;
            
        case FFT_NOISINESS:
            calcNoisiness();
            break;
            
        case FFT_PITCH:
            calcPitch();
            break;
            
        case FFT_MFCS:
            
            break;
            
        case FFT_MFCCS:
            break;
            
        case FFT_BFCS:
            break;
            
        case FFT_BFCCS:
            break;
            
        case FFT_LinearPower:
            calcLinearPower();
            break;
        case FFT_LogPower:
            break;
            
        default:
            break;
            
    }
}

std::string IRFFTDescriptor::getDescriptorName(FFTDescriptor descriptor)
{
    switch (descriptor)
    {
        case FFT_MAGNITUDE:
            return "Magnitude";
            break;
        case FFT_CENTROID:
            return "Centroid";
            break;
            
        case FFT_SPREAD:
            return "Spread";
            break;
        case FFT_FLATNESS:
            return "Flatness";
            break;
            
        case FFT_NOISINESS:
            return "Noisiness";
            break;
            
        case FFT_PITCH:
            return "Pitch";
            break;
            
        case FFT_MFCS:
            return "MFCs";
            break;
            
        case FFT_MFCCS:
            return "MFCCs";
            break;
            
        case FFT_BFCS:
            return "BFCs";
            break;
            
        case FFT_BFCCS:
            return "BFCCs";
            break;
        case FFT_LinearPower:
            return "LienarPower";
            break;
        case FFT_LogPower:
            return "LogPower";
            break;
        default:
            return "UNKNOWN";
            break;
            
    }
}

// ==================================================

void IRFFTDescriptor::calcLinearPower()
{
    if(hasLinearPowerCalculated()) return;
    this->linearPower = getPower();
    this->hasLinearPowerCalculatedFlag = true;
}

void IRFFTDescriptor::calcMagnitude()
{
    
    if(hasMagnitudeCalculated()) return;
    
    int f, i;
    this->magnitude.clear();
    auto power = getPower();
    
    std::vector<float> m;

    for(f=0;f<getNumFrame();f++)
    {
        float mag = 0;

        for(i=0;i<this->ffthalfsize;i++){
            mag += power[f][i];
        }
        m.push_back(mag);
    }
    
    this->magnitude.push_back(m);
    
    this->hasMagnitudeCalculatedFlag = true;

}

void IRFFTDescriptor::calcCentroid()
{
    
    if(hasCentroidCalculated()) return;

    int i, f;
    float freq_accum = 0;
    float spread_accum = 0;
    float mag = 0;
    float cent = 0;
    float sp = 0;
    float freq_bin = 44100.0 / float(getFFTSize());
    
    this->magnitude.clear();
    this->centroid.clear();
    
    auto power = getPower();

    // calc frequency per bin
    std::vector<float> freq_per_bin;
    freq_per_bin.reserve(this->ffthalfsize);
    for(i=0;i<this->ffthalfsize;i++)
    {
        freq_per_bin.push_back(float(i) * freq_bin);
    }
    
    std::vector<float> m;
    std::vector<float> c;
    std::vector<float> s;

    for(f=0;f<getNumFrame();f++)
    {
        mag = 0;
        cent = 0;
        sp = 0;
        freq_accum = 0;
        spread_accum = 0;

        for(i=0;i<this->ffthalfsize;i++){
            freq_accum += (freq_per_bin[i] * power[f][i]);
            mag += power[f][i];
        }
    
        if(mag == 0.0) cent = 0;
        else cent = freq_accum / mag;

        m.push_back(mag);
        c.push_back(cent);

        //spread
        for(i=0;i<this->ffthalfsize;i++){
            spread_accum += power[f][i] * fabs(freq_per_bin[i] * (float)i - cent);
        }
        
        if(mag == 0.0) sp = 0.0;
        else sp = spread_accum / mag;
        
        s.push_back(sp);
    }
    this->centroid.push_back(c);
    this->magnitude.push_back(m);
    this->spread.push_back(s);
    
    
    //delete [] freq_per_bin;
    
    this->hasCentroidCalculatedFlag = true;
    this->hasMagnitudeCalculatedFlag = true;
    this->hasSpreadCalculatedFlag = true;

}
// --------------------------------------------------
void IRFFTDescriptor::calcSpread()
{
    if(hasSpreadCalculated()) return;
    calcCentroid();
    this->hasSpreadCalculatedFlag = true;
}

// --------------------------------------------------
void IRFFTDescriptor::calcFlatness()
{
    if(hasFlatnessCalculated()) return;
    
    int i, f;
    float multi_accum = 0;
    float mag = 0;
    auto power = getPower();
    std::vector<float> flat;

    for(f=0;f<getNumFrame();f++)
    {
        mag = 0;
        
        multi_accum = power[f][0];

        for(i=0;i<this->ffthalfsize;i++){
            mag += power[f][i];
            multi_accum *= power[f][i];
            multi_accum = sqrt(multi_accum);
        }
    
        mag = mag / this->ffthalfsize;
        if(multi_accum != 0.0 && mag != 0.0)
        {
            flat.push_back(multi_accum / (mag));
        }
    }
    this->flatness.push_back(flat);
    
    this->hasFlatnessCalculatedFlag = true;
}
// --------------------------------------------------
void IRFFTDescriptor::calcPitch()
{
    if(hasPitchCalculated()) return;
    
    
    this->hasPitchCalculatedFlag = true;
}
// --------------------------------------------------
void IRFFTDescriptor::calcNoisiness()
{
    if(hasNoisinessCalculated()) return;
    this->hasNoisinessCalculatedFlag = true;
}
// --------------------------------------------------

// --------------------------------------------------

// ==================================================
