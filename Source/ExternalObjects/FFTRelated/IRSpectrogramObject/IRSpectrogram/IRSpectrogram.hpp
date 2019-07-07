//
//  IRSpectrogram.hpp
//  IRiMaS
//
//  Created by Keitaro on 26/06/2019.
//

#ifndef IRSpectrogram_hpp
#define IRSpectrogram_hpp

#include "IRUIAudioFoundation.hpp"
#include "GLSLSpectrogram.hpp"

class IRSpectrogram : public IRUIAudioFoundation
{
public:
    IRSpectrogram(IRNodeObject* nodeObject);
    ~IRSpectrogram();
    
    void init();
    // ==================================================

    void resized() override;
    void paint(Graphics& g) override;
    
    // ==================================================
    
    // ==================================================
    // redraw spectrogram
    void update();
    
private:
    // ==================================================
    OpenGLContext openGLContext;
    
    std::unique_ptr<OpenGLGraphicsContextCustomShader> shader;
    CodeDocument fragmentDocument;
    String fragmentCode;
    
    const char* fragmentShader = nullptr;

    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRSpectrogram)

};

#endif /* IRSpectrogram_hpp */
