//
//  IRSignalWindowLib.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 05/09/2018.
//

#ifndef IRSignalWindowLib_h
#define IRSignalWindowLib_h

namespace IRWindow
{
    enum TYPE{
        NONE,
        HANNING,
        HAMMING,
        BLACKMAN
    };


class IRSignalWindowLib
{
public:
    IRSignalWindowLib(int framesize, TYPE windowType) :
    framesize(framesize),
    windowType(windowType)
    {
        createWindow(windowType);
    }
    ~IRSignalWindowLib()
    {
    }
    // ------------------------------------------------------------
    
    TYPE getWindowType() const { return this->windowType; }
    int getFrameSize() const { return this->framesize; }
    
    // ------------------------------------------------------------
    // process windowing
    
    void windowing(float* buffer){
        if(this->windowType != TYPE::NONE)
            for(int i=0;i<this->framesize;i++)
                buffer[i] *= this->window[i];
    }
    
    std::vector<float> windowingVector(std::vector<float>buffer, int length = 0){
        if(length == 0) { length = this->framesize; }
        if(this->windowType != TYPE::NONE)
            for(int i=0;i<length;i++)
                buffer[i] *= this->window[i];
        return buffer;
    }
    // ------------------------------------------------------------
    // reset framesize
    void setFrameSize(const int framesize)
    {
        this->framesize = framesize;
        this->window.clear();
        this->window.reserve(framesize);
    }
private:
    void createWindow(TYPE windowType){
        switch(windowType)
        {
            case TYPE::HANNING:
                createHanning();
                break;
            case TYPE::HAMMING:
                createHamming();
                break;
            case TYPE::BLACKMAN:
                createBlackman();
                break;
            default:
                break;
        }
    }
    // ------------------------------------------------------------
    void noWindow(){
        for(int i=0;i<this->framesize;i++)
            this->window.push_back(1.0);
    }
    // ------------------------------------------------------------
    void createHanning()
    {
        for(int i=0;i<this->framesize;i++){
            this->window.push_back(0.5 - 0.5 * cos( 2 * M_PI * i));
        }
    }
    // ------------------------------------------------------------
    void createHamming()
    {
        for(int i=0;i<this->framesize;i++){
            this->window.push_back(0.54 - 0.46 * cos( 2 * M_PI * i));
        }
    }
    // ------------------------------------------------------------
    void createBlackman()
    {
        for(int i=0;i<this->framesize;i++){
            this->window.push_back(0.42 - 0.5 * cos( 2 * M_PI * i) + 0.08 * cos( 4 * M_PI * i));
        }
    }

    

private:
    int framesize;
    TYPE windowType;
    
    std::vector<float> window;
};

}
#endif /* IRSignalWindowLib_h */
