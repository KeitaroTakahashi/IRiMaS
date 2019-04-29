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
    va_list args;
    int count;
    if (arg_num < 1) return;

    va_start(args , arg_num);
    
    this->descriptorList.clear();
    for (count = 0 ; count < arg_num ; count++) {
        this->descriptorList.push_back(static_cast<FFTDescriptor>(va_arg(args , int)));
    }
    va_end(args);

}


