//
//  IRSpectrogram.cpp
//  IRiMaS
//
//  Created by Keitaro on 26/06/2019.
//

#include "IRSpectrogram.hpp"

IRSpectrogram::IRSpectrogram(IRNodeObject* nodeObject) :
IRUIAudioFoundation(nodeObject),
parent(nodeObject)
{
    init();
    
    this->openButton.setButtonText("Open Audio File");
    addAndMakeVisible(&this->openButton);
    this->openButton.onClick = [this]{ openButtonClicked(); };
    
    this->controller.setZoomInEvent([this]{zoomInClicked();});
    this->controller.setZoomOutEvent([this]{zoomOutClicked();});
    this->controller.setCommentEvent([this]{commentClicked();});
    //openFile();

}

IRSpectrogram::~IRSpectrogram()
{
    std::cout << "~IRSpectrogram\n";
    stopTimer();

    if(!this->isOpenGLComponentClosed)
        closeOpenGLComponent();
    
    if(this->audioData != nullptr)
        getFileManager()->discardFilePtr(IRFileType::IRAUDIO, this->audioData, this->parent, this->file);
    
    std::cout << "~IRSpectrogram ENDS\n";

}

void IRSpectrogram::closeOpenGLComponent()
{
    // delete Texture
    glDeleteTextures(1, &this->textureID);
    openGLContext.detach();
    shader.reset();
    
    this->isOpenGLComponentClosed = true;
}

void IRSpectrogram::init()
{
    setOpaque(true);
    if (auto* peer = getPeer())
        peer->setCurrentRenderingEngine (0);
    
    this->openGLContext.attachTo(*getTopLevelComponent());
    /*
    String url = File::getSpecialLocation(File::currentApplicationFile).getFullPathName() +
    "/Contents/Resources/materials/Sources/GLSL/Spectrogram/KSpectrogram.frag";*/
    
    String url = "/Users/keitaro/program/JUCE/IRiMaS/materials/Sources/GLSL/Spectrogram/KSpectrogram.frag";
    
    this->fragURL = url;
    IRTextLoader fragmentText (url.toStdString());
    this->fragmentCode = fragmentText.getStdString();
    this->fragmentRefreshed = true;
    
    startTimer(100);
    
    setSize(500,500);
    
   // createDemoTexture();
}

// ==================================================

void IRSpectrogram::resized()
{
    if(getHeight() < 50) setBounds(getX(), getY(), getWidth(), 50);

    this->openButton.setBounds(0, 0, getWidth(), getHeight());
    
    
    int s = getHeight();
    if(s > 50) s = 50;
    int y = getHeight() / 2 - s / 2;
    this->controller.setBounds(this->visibleArea.getX(), y, getWidth(), s);

}

void IRSpectrogram::paint(Graphics& g)
{
    shaderTask(g);
}

// ==================================================

void IRSpectrogram::openFile()
{
    
    FileChooser chooser("Select an image file...",
                        {},
                        "*.wav, *.aif, *.aiff, *.mp3");
    if (chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        this->file = file;
        auto p = file.getFullPathName();
        this->path = p;
        
        String pathToOpen;
        pathToOpen.swapWith(p);
        
        if(pathToOpen.isNotEmpty())
        {
            getFilePtr(this->file);
        }
    }
}

void IRSpectrogram::getFilePtr(File file)
{
    // set a callback function which is called when file load is completed.
    // get a pointer of the audio file
    std::function<void()> callback = [this]{fileImportCompleted();};
    
    
    getFileManager()->getFilePtrWithCallBack(IRFileType::IRAUDIO,
                                             file,
                                             this->parent,
                                             callback);
    // notify changes to IRProject to update IRFileInspecter
    this->parent->notifyNodeObjectModification();
    
}

void IRSpectrogram::openButtonClicked()
{
    openFile();
    removeChildComponent(&this->openButton);
}
// ==================================================


void IRSpectrogram::update()
{
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->sp_w, this->sp_h, GL_DEPTH_COMPONENT, GL_FLOAT, this->buffer);
}

void IRSpectrogram::loadDrawData(IRDescriptorStr* data)
{
    
    int w = getWidth();
    int h = getHeight();
    
    this->sp_w = data->getNumFrame();
    this->sp_h = data->getFFTSize()/4;
    int ffthalfsize = data->getFFTSize() / 2;

    // use normalized data 0.0~1.0
    const float* power = data->getNormalizedData();
    
    //this->buffer = data;

    std::cout << "loadDrawData : (w,h) = " << this->sp_w << ", " << this->sp_h << std::endl;
  
    if(this->buffer != nullptr) delete this->buffer;
    this->buffer = new float [this->sp_w * this->sp_h];
    int i, j;
    float max = -9999;
    
    float incrementX = this->sp_w / (float)w;
    float incrementY = this->sp_h / (float)h;
    
    /*
    for(i = 0; i < h; i ++)
    {
        for(j = 0; j < w; j ++)
        {
            
            this->buffer[i*w+j] = power[];
        }
    }*/
    
  
    for(i = 0; i < this->sp_h; i ++)
    {
        for(j = 0; j < this->sp_w; j ++)
        {
            int iw = i * this->sp_w;
            int jw = j * ffthalfsize;

            this->buffer[iw + j] = power[jw + i];
            
            //if(power[j][i] > max) max = power[j][i];
            
        
        }
    }
    
    std::cout << "loadData done\n";
  
    /*
    for(i = 0; i < this->sp_h; i ++)
    {
        for(j = 0; j < this->sp_w; j ++)
        {
            int iw = i * this->sp_w;
            
            this->buffer[iw + j] /= max;
        }
    }
 */
    
    
    this->fragmentRefreshed = true;
    
    
}

void IRSpectrogram::setVisibleArea(Rectangle<int> area)
{
    int s = getHeight();
    if(s > 50) s = 50;
    int y = getHeight() / 2 - s / 2;
    this->visibleArea = area;
    //this->controller.setBounds(this->visibleArea.getX(), y, getWidth(), s);
    //this->controller.setBounds(0, y, getWidth(), s);

    this->previousOffsetX = this->visibleArea.getX();
    
    std::cout << "w, h " << getWidth() << std::endl;
}

void IRSpectrogram::setMagnitudeAmount(float val)
{
    this->magnitudeAmount = val;
    repaint();
}
// ==================================================

void IRSpectrogram::mouseDown(const MouseEvent &e)
{
    this->mouseDownPos = e.getEventRelativeTo(this).getPosition();

    
    if(this->audioUpdated)
    {
        
        std::cout << "audioUpdated true\n";
        auto data = this->audioData->getData();
        //auto de = data->operateAnalysis(FFTDescriptor::FFT_LinearPower, this->fftsize, this->hopsize);
        
        
        if(!data->isCalculated(FFTDescriptor::FFT_LinearPower, this->fftsize))
        {
            std::cout << "do analysis \n";
            if(!data->operateAnalysis(FFTDescriptor::FFT_LinearPower, this->fftsize, this->hopsize))
            {
                std::cout <<"ERROR : IRSpectrogram() : Could not operate FFT_LinearPower Analysis\n";
                return;
            }
        }
        

        loadDrawData(data->getDescriptor(FFTDescriptor::FFT_LinearPower,
                                         this->fftsize));
        
        
      
        
        this->audioUpdated = false;
        this->isTextureCreated = false;
    }else{
        auto data = this->audioData->getData();

        loadDrawData(data->getDescriptor(FFTDescriptor::FFT_LinearPower,
                                         this->fftsize));

    }
    
    if(e.mods.isCtrlDown())
    {
        addAndMakeVisible(&this->controller);
    }else{
        if(this->controller.isVisible())
            removeChildComponent(&this->controller);
    }
    
    updateFragment();

}

void IRSpectrogram::mouseMove(const MouseEvent &e)
{
    this->currentMousePos = e.getEventRelativeTo(this).getPosition();

}

// ==================================================

void IRSpectrogram::shaderTask(Graphics& g)
{
    if (shader.get() == nullptr || shader->getFragmentShaderCode() != fragmentCode)
    {
        //shader.reset();
        if (fragmentCode.isNotEmpty() && this->fragmentRefreshed)
        {
            std::cout << "compilin...\n";
            shader.reset (new OpenGLGraphicsContextCustomShader (fragmentCode));
            shader->onShaderActivated = [this](OpenGLShaderProgram& program){setUniform(program);};
            
            if(!this->isTextureCreated)
            {
                createTexture();
                this->isTextureCreated = true;
            }
    
            auto result = shader->checkCompilation (g.getInternalContext());
            if (result.failed()) shader.reset();
            
            this->fragmentRefreshed = false;
        }
    }
    
    if (shader.get() != nullptr)
    {
        std::cout << "fill\n";
        shader->fillRect (g.getInternalContext(), getLocalBounds());
    }
}

void IRSpectrogram::createTexture()
{
    std::cout << "createTexture()\n";
    
    glDeleteTextures(1, &this->textureID);

    
    this->textureID = 0;
    glGenTextures(1, &this->textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,this->sp_w, this->sp_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, this->buffer);
}

void IRSpectrogram::updateFragment()
{
    IRTextLoader fragmentText (this->fragURL.toStdString());
    String newCode = fragmentText.getStdString();
    this->fragmentCode = newCode;
    this->fragmentRefreshed = true;
    
    repaint();
}

void IRSpectrogram::setUniform(OpenGLShaderProgram& program)
{
    
    // IMPORTANT : get the actual scale of the screen because MacOSX uses Retina display which has double piexles
    int scale = Desktop::getInstance().getDisplays().getMainDisplay().scale;
    int w = getWidth() * scale;
    int h = getHeight() * scale;
    
    program.setUniform("resolution", w, h);
    
    program.setUniform("mouse",
                       this->currentMousePos.getX(),
                       this->currentMousePos.getY());
    
    program.setUniform("mouseDownPos",
                       this->mouseDownPos.getX(),
                       this->mouseDownPos.getY());
    
    program.setUniform("dataSize",
                       this->sp_w,
                       this->sp_h);
    
    program.setUniform("mag", this->magnitudeAmount);
    
    // we need to bind the texture every time.
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    // pass the texture buffer to Shader
    GLint bufIndex = glGetUniformLocation(program.getProgramID(), "buffer");
    glUniform1i(bufIndex, 0);
    
    // =======
    
    this->fps_index ++;
    
    //if(this->fps_index == 100)
    //{
        this->fps_index = 0;
        
        int nowTime = Time::getMillisecondCounter();
        
        nowTime -= this->fps_previousTime;
        
        std::cout << "one frame in " << (float)nowTime / 100.0 << "ms." << std::endl;
        
        this->fps_previousTime = Time::getMillisecondCounter();
        
    //}
}


// ==========
/*
void IRSpectrogram::createDemoTexture()
{
    if(this->buffer != nullptr) delete this->buffer;
    this->buffer = new float [this->sp_w * this->sp_h];
    
    int hw = this->sp_w / 2.0;
    int hh = this->sp_h / 2.0;
    for(int i = 0; i < this->sp_h; i ++)
    {
        for(int j = 0; j < this->sp_w; j ++)
        {
            int iw = i * this->sp_w;
            if(i > hh)
            {
                if(j > hw) this->buffer[iw + j] = (float)(rand() % 100) / 100.0;
                else this->buffer[iw + j] = 1.0;
            }else
            {
                if(j > hw) this->buffer[iw + j] = 1.0;
                else this->buffer[iw + j] = (float)(rand() % 100) / 100.0;
            }
        }
    }
}
 */

// ==========

void IRSpectrogram::fileImportCompleted()
{
    std::cout << "fileImportCompleted\n";
    this->audioData = static_cast<DataAllocationManager<IRAudio>*>(getFileManager()->getFileObject());

    this->audioUpdated = true;
    
    // call virtual function
    fileImportCompletedAction();
}
// ==================================================
//Controller

void IRSpectrogram::zoomInClicked()
{
    if(this->zoomInClickedCallback != nullptr)
        this->zoomInClickedCallback();
}
void IRSpectrogram::zoomOutClicked()
{
    if(this->zoomOutClickedCallback != nullptr)
        this->zoomOutClickedCallback();
}
void IRSpectrogram::commentClicked()
{
    //showComment
}


// ==================================================
