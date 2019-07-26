//
//  IRSpectrogram.cpp
//  IRiMaS
//
//  Created by Keitaro on 26/06/2019.
//

#include "IRSpectrogram.hpp"

IRSpectrogram::IRSpectrogram(IRNodeObject* nodeObject) :
IRUIAudioFoundation(nodeObject),
parent(nodeObject),
zoomRatio(Point<float>(1.0,1.0))
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
    setOpaque(false);
    if (auto* peer = getPeer())
        peer->setCurrentRenderingEngine (0);
    
    this->openGLContext.attachTo(*getTopLevelComponent());
    /*
    String url = File::getSpecialLocation(File::currentApplicationFile).getFullPathName() +
    "/Contents/Resources/materials/Sources/GLSL/Spectrogram/KSpectrogram.frag";*/
    
    String url = "/Users/keitaro/program/JUCE/IRiMaS/materials/Sources/GLSL/Spectrogram/KSpectrogram.frag";
    
    this->fragURL = url;
    this->fragmentText.load (url.toStdString());
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
    
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->sp_w, this->sp_h, GL_DEPTH_COMPONENT, GL_FLOAT, this->buffer);
    this->updateTexture = false;
}

void IRSpectrogram::calcPixel(IRDescriptorStr* data)
{
    // data info
    int nframe = data->getNumFrame();
    int fftsize = data->getFFTSize() / 8;
    int ffthalfsize = data->getFFTSize() / 2;
    
    int powerSize = nframe * ffthalfsize;
    
    //visible area
    int w = this->visibleArea.getWidth();
    int h = this->visibleArea.getHeight();
    int x = this->visibleArea.getX();
    int y = this->visibleArea.getY();
    
    // viewport size
    float parentW = (float)w * this->zoomRatio.getX();
    float parentH = (float)h * this->zoomRatio.getY();
    
    
    //std::cout << " >>>>>> visible area " << x << " of (" << w << ", " << h << ") : parent " << parentW << ", " << parentH << " <<<<<< " << std::endl;
    float startFrame = (float)x * (float)nframe/parentW;
    float drawFrameNum = (float)(w) * (float)nframe / parentW;
    float startFFTSize =(float)y * (float)fftsize / parentH;
    float drawFFTSize = (float)(h) * (float)fftsize / parentH;
    
    
    float texture_w = (drawFrameNum < this->MAX_TEXTURE_SIZE)?
    drawFrameNum : this->MAX_TEXTURE_SIZE;
    float texture_h = (drawFFTSize < this->MAX_TEXTURE_SIZE)?
    drawFFTSize : this->MAX_TEXTURE_SIZE;
    
    float texRatioX = texture_w / drawFrameNum;
    float texRatioY = texture_h / drawFFTSize;
    
    if(this->buffer != nullptr) delete[] this->buffer;
    int texSize = floor(texture_w) * floor(texture_h);
    this->buffer = new float [texSize];
    
    // use normalized data 0.0~1.0
    const float* power = data->getNormalizedData();

    int i;
    // initialize
    for(i=0;i<texSize;i++) this->buffer[i] = 0.0;
    
    int currentX = floor(startFrame);
    int currentY = floor(startFFTSize);

    
    int endX = floor(startFrame + drawFrameNum);
    int endY = floor(startFFTSize + drawFFTSize);
    
    
    if(endX >= nframe)
    {
        int dis = endX - nframe;
        currentX -= dis;
        endX -= dis;
    }
    if(endY >= fftsize)
    {
        int dis = endY - fftsize;
        currentY -= dis;
        endY -= dis;
    }
    /*
    std::cout << "==========\n";
    std::cout <<"texture size = " << texture_w << " h " << texture_h << " : ratio = " << texRatioX << " : texSize = " << texSize << std::endl;
    
    std::cout << "start x = " << currentX << " to " << endX << " of " << nframe << " : startY " << currentY << " to " << endY << " of "<< fftsize<<std::endl;
    */
    this->sp_w = floor(texture_w);
    this->sp_h = floor(texture_h);
    //std::cout << "sp_w = " << this->sp_w << ", " << this->sp_h << " : texSize = "<< texSize << std::endl;

    int tex_x = 0;
    int tex_y = 0;
    
    int texY_sp_w = 0;;
    
    int int_startFrame = floor(startFrame);
    // texture
    while(1) // h
    {
        //std::cout << "y " << tex_y << " of " << endY << std::endl;
        if(tex_y >= this->sp_h) break;
        if(currentY >= endY) break;
        tex_x = 0;
        currentX = int_startFrame;
        texY_sp_w = tex_y * this->sp_w;
        
        while(1) // w
        {
            if(tex_x >= this->sp_w) break;
            if(currentX >= endX) break;
            
            int bufferIndex = texY_sp_w + tex_x;
            int pIndex = (int)floor(currentX * ffthalfsize + currentY);
            float p = power[pIndex];

            //std::cout << "currentX = " << currentX << " of " << floor(startFrame) << " : w =  " << this->sp_w <<std::endl;
            if(this->buffer[bufferIndex] < p) this->buffer[bufferIndex] = p;
            
            //std::cout << "buffer["<<bufferIndex <<"] = "<<this->buffer[tex_x] << " | " << (int)floor(tex_x * ffthalfsize + tex_y) << " power[" << currentX << "][" << currentY << "] = " << p << std::endl;

            tex_x += 1;
            currentX += 1.0;
        }
        
        tex_y += 1;
        currentY += 1.0;
    }

}

void IRSpectrogram::loadDrawData(IRDescriptorStr* data)
{
  
    
}

void IRSpectrogram::setVisibleArea(Rectangle<int> area, Point<int> parentSize)
{
    int s = getHeight();
    if(s > 50) s = 50;
    int y = getHeight() / 2 - s / 2;
    
    // Subtract GridSize!! need to be fixed in smarter way
    this->visibleArea = Rectangle<int>(area.getX(),
                                       area.getY(),
                                       area.getWidth() - 30,
                                       area.getHeight() - 30);
    
    // follow always to the visibla area
    setBounds(this->visibleArea);
    
    this->parentSize = parentSize;
    //this->controller.setBounds(this->visibleArea.getX(), y, getWidth(), s);
    //this->controller.setBounds(0, y, getWidth(), s);

    this->previousOffsetX = this->visibleArea.getX();
    
    // just modify texture already created to save CUP usage
    reCalcDescriptor();
    std::cout << "w, h " << getWidth() << std::endl;
}

void IRSpectrogram::parentSizeChanged(int w, int h)
{
    this->parentSize = Point<int>(w, h);
    loadDescriptor();
}


void IRSpectrogram::setMagnitudeAmount(float val)
{
    this->magnitudeAmount = val;
    repaint();
}

void IRSpectrogram::loadDescriptor()
{
    std::cout << "loadDescriptor\n";
    if(this->audioUpdated)
    {
        auto data = this->audioData->getData();
        if(!data->isCalculated(FFTDescriptor::FFT_LinearPower, this->fftsize))
        {
            if(!data->operateAnalysis(FFTDescriptor::FFT_LinearPower, this->fftsize, this->hopsize))
            {
                std::cout <<"ERROR : IRSpectrogram() : Could not operate FFT_LinearPower Analysis\n";
                return;
            }
        }
        
        calcPixel(data->getDescriptor(FFTDescriptor::FFT_LinearPower,
                                      this->fftsize));

        
        this->isTextureCreated = false;
        std::cout << "refresh texture\n";
        updateFragment();
            
    }
}

void IRSpectrogram::reCalcDescriptor()
{
    
    if(this->audioUpdated)
    {
        auto data = this->audioData->getData();
        calcPixel(data->getDescriptor(FFTDescriptor::FFT_LinearPower,
                                      this->fftsize));
        
        this->updateTexture = true;
        updateFragment();
    }

}

// ==================================================

void IRSpectrogram::mouseDown(const MouseEvent &e)
{
    this->mouseDownPos = e.getEventRelativeTo(this).getPosition();

    //loadDescriptor();
    
    if(e.mods.isCtrlDown())
    {
        addAndMakeVisible(&this->controller);
    }else{
        if(this->controller.isVisible())
            removeChildComponent(&this->controller);
    }
    
}

void IRSpectrogram::mouseMove(const MouseEvent &e)
{
    this->currentMousePos = e.getEventRelativeTo(this).getPosition();

}

// ==================================================

void IRSpectrogram::shaderTask(Graphics& g)
{
    //std::cout << "shaderTask\n";
    if (shader.get() == nullptr || shader->getFragmentShaderCode() != fragmentCode)
    {
        //shader.reset();
        if (fragmentCode.isNotEmpty() && this->fragmentRefreshed)
        {
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
        shader->fillRect (g.getInternalContext(),
                          getLocalBounds()
                          );
        
        
    }
}

void IRSpectrogram::createTexture()
{
    glDeleteTextures(1, &this->textureID);

    this->textureID = 0;
    glGenTextures(1, &this->textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->sp_w, this->sp_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, this->buffer);
    
}

void IRSpectrogram::updateFragment()
{
    this->fragmentText.load (this->fragURL.toStdString());

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
    if(this->updateTexture) update();
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
    
    loadDescriptor();
    
    // call virtual function
    fileImportCompletedAction();
}
// ==================================================
//Controller

void IRSpectrogram::zoomInClicked()
{
    if(this->zoomInClickedCallback != nullptr)
        this->zoomInClickedCallback();
    this->isTextureCreated = false;
    loadDescriptor();
    
}
void IRSpectrogram::zoomOutClicked()
{
    if(this->zoomOutClickedCallback != nullptr)
        this->zoomOutClickedCallback();
    
    // refresh tecture!
    this->isTextureCreated = false;
    loadDescriptor();
}
void IRSpectrogram::commentClicked()
{
    //showComment
}


// ==================================================
