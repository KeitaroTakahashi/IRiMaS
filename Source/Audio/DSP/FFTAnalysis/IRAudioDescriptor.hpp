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

enum FFTDescriptor{
    FFT_MAGNITUDE,
    FFT_CENTROID,
    FFT_SPREAD,
    FFT_FLATNESS,
    FFT_NOISINESS,
    FFT_PITCH,
    FFT_MFCS,
    FFT_MFCCS,
    FFT_BFCS,
    FFT_BFCCS,
    
    FFT_LinearPower,
    FFT_LogPower,
    FFT_Phase,
    
    NONE
};


class IRFFTDescriptor : public IRFFTSequence
{
public:
    

    
    IRFFTDescriptor(int fftsize, int hopsize, IRWindow::TYPE windowType = IRWindow::TYPE::HANNING);
    ~IRFFTDescriptor();
    
    // @param1 number of argument
    // @param2... IRFFTDescriptor::FFTDescriptor 
    void setDescriptor(int arg_num, ...);
    
    void operateAnalysis();
    
    
    void calcMagnitude();
    void calcCentroid(); // this also calculates magnitude
    void calcSpread();
    void calcFlatness();
    void calcPitch();
    void calcNoisiness();
    
    
    std::vector<std::vector<float>> getMagnitude() const { return this->magnitude; }
    std::vector<std::vector<float>> getCentroid() const { return this->centroid; }
    std::vector<std::vector<float>> getSpread() const { return this->spread; }
    std::vector<std::vector<float>> getFlatness() const { return this->flatness; }
    std::vector<std::vector<float>> getPitch() const { return this->pitch; }
    std::vector<std::vector<float>> getNoisiness() const { return this->noisiness; }
    
    std::vector<std::vector<float>> getMFCs() const { return this->MFCs; }
    std::vector<std::vector<float>> getMFCCs() const { return this->MFCCs; }
    std::vector<std::vector<float>> getBFCs() const { return this->BFCs; }
    std::vector<std::vector<float>> getBFCCs() const { return this->BFCCs; }
    
    std::vector<FFTDescriptor> getDescriptorList() const { return this->descriptorList; }
    std::vector<std::string> getDescriptorNameList() const { return this->descriptorNameList; }
    
    std::string getDescriptorName(FFTDescriptor descriptor);
    // ============================================================
    
    
    bool hasMagnitudeCalculated()       const { return this->hasMagnitudeCalculatedFlag; }
    bool hasCentroidCalculated()        const { return this->hasCentroidCalculatedFlag; }
    bool hasSpreadCalculated()          const { return this->hasSpreadCalculatedFlag; }
    bool hasPitchCalculated()           const { return this->hasPitchCalculatedFlag; }
    bool hasFlatnessCalculated()       const { return this->hasFlatnessCalculatedFlag; }

    bool hasNoisinessCalculated()       const { return this->hasNoisinessCalculatedFlag; }

    // ============================================================

private:
    
    void calcDescriptor(FFTDescriptor descriptor);

    
    
    std::vector<FFTDescriptor> descriptorList;
    std::vector<std::string> descriptorNameList;
    
    std::vector<std::vector<float>> magnitude;
    std::vector<std::vector<float>> centroid;
    std::vector<std::vector<float>> spread;
    std::vector<std::vector<float>> flatness;
    std::vector<std::vector<float>> pitch;
    std::vector<std::vector<float>> noisiness;
    std::vector<std::vector<float>> MFCs;
    std::vector<std::vector<float>> MFCCs;

    std::vector<std::vector<float>> BFCs;
    std::vector<std::vector<float>> BFCCs;
    
    // ============================================================
    bool hasMagnitudeCalculatedFlag = false;
    bool hasCentroidCalculatedFlag = false;
    bool hasSpreadCalculatedFlag = false;
    bool hasFlatnessCalculatedFlag = false;
    bool hasPitchCalculatedFlag = false;
    bool hasNoisinessCalculatedFlag = false;
    // ============================================================

};

#endif /* IRAudioDescriptor_hpp */
