//
//  IRSpectrogram.cpp
//  IRiMaS
//
//  Created by Keitaro on 26/06/2019.
//

#include "IRSpectrogram.hpp"

IRSpectrogram::IRSpectrogram(IRNodeObject* nodeObject) :
IRUIAudioFoundation(nodeObject)
{
    
}

IRSpectrogram::~IRSpectrogram()
{
    openGLContext.detach();
    shader.reset();
}

void IRSpectrogram::init()
{
    setOpaque(true);
    if (auto* peer = getPeer())
        peer->setCurrentRenderingEngine (0);
    
    this->openGLContext.attachTo(*getTopLevelComponent());
    
    IRTextLoader fragmentText ("/Users/keitaro/program/JUCE/OpenGLProject/Source/Shader/frag5.frag");
    this->fragmentShader = fragmentText.getConstChar();
    fragmentDocument.replaceAllContent (this->fragmentShader);
}

// ==================================================

void IRSpectrogram::resized()
{
}

void IRSpectrogram::paint(Graphics& g)
{
    
}

void IRSpectrogram::update()
{
}

// ==================================================
