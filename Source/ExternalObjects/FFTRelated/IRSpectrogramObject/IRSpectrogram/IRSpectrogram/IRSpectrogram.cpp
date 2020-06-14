//
//  IRSpectrogram.cpp
//  IRiMaS
//
//  Created by Keitaro on 26/06/2019.
//

#include "IRSpectrogram.hpp"

IRSpectrogram::IRSpectrogram(IRNodeObject* nodeObject, IRStr* str) :
IRUIAudioFoundation(nodeObject, str),
IRHeavyWeightComponent(this, "IRSpectrogram"),
parent(nodeObject),
zoomInfo(juce::Point<float>(1.0,1.0))
{
    init();
    
    this->openButton.setButtonText("Open Audio File");
    addAndMakeVisible(&this->openButton);
    this->openButton.onClick = [this]{ openButtonClicked(); };
    
    this->controller.setZoomInEvent([this]{zoomInClicked();});
    this->controller.setZoomOutEvent([this]{zoomOutClicked();});
    this->controller.setCommentEvent([this]{commentClicked();});
    //openFile();
    
    //setFps(17);

    setFps(100);
  
    
}


IRSpectrogram::~IRSpectrogram()
{
    std::cout << "~IRSpectrogram\n";
    stopTimer();

    deleteBuffer();
    if(!this->isOpenGLComponentClosed)
        closeOpenGLComponent();
    
    if(this->audioData != nullptr)
    {
        this->audioData->getData()->removeListener(this);
        getFileManager().discardFilePtr(IRFileType::IRAUDIO, this->audioData, this->parent, this->file);
    }
    
    std::cout << "~IRSpectrogram ENDS\n";

}

void IRSpectrogram::closeOpenGLComponent()
{
    // delete Texture
    glDeleteTextures(1, &this->textureID);
    //openGLContext.detach();
    shader.reset();
    
    this->isOpenGLComponentClosed = true;
}

void IRSpectrogram::init()
{
    //setOpaque(true);
    if (auto* peer = getPeer())
        peer->setCurrentRenderingEngine (0);
    
    //this->openGLContext.attachTo(*getTopLevelComponent());
    
    String url = File::getSpecialLocation(File::currentApplicationFile).getFullPathName();
#if JUCE_MAC
    url += "/Contents/Resources/materials/Sources/GLSL/Spectrogram/KSpectrogram.frag";
#elif JUCE_IOS
    url += "/materials/Sources/GLSL/Spectrogram/KSpectrogram.frag";
#endif
    
    File f(url);
    if(!f.exists())
    {
        std::cout << "Error : " << url << " does not exist!!\n";
        AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon, "Fatal Error", "IRSpectrogram : frag file " + url + "not found! Please contact a developer with this error message.");
    }
    
    
    
    this->fragURL = url;
    this->fragmentText.load (url.toStdString());
    this->fragmentCode = fragmentText.getStdString();
    this->fragmentRefreshed = true;
    
    startTimer(100);
    setSize(500,500);
    
   // createDemoTexture();
}

void IRSpectrogram::heavyComponentRefreshed()
{
    
    std::cout << "heavyComponentRefreshed\n";
    loadDescriptor();
}

// ==================================================

void IRSpectrogram::resized()
{
    if(getHeight() < 50) setBounds(getX(), getY(), getWidth(), 50);

    //this->openButton.setBounds(0, 0, getWidth(), getHeight());
    
    int s = getHeight();
    if(s > 50) s = 50;
    int y = getHeight() / 2 - s / 2;
    this->controller.setBounds(this->visibleArea.getX(), y, getWidth(), s);

}

void IRSpectrogram::paint(Graphics& g)
{
    bench.start();
    shaderTask(g);
    
    g.setColour(Colours::red);
    g.drawLine(this->playingLine.getX(),
               this->playingLine.getY(),
               this->playingLine.getWidth(),
               this->playingLine.getHeight(),
               3.0);
    std::cout << "shaderTask " << bench.stop() << std::endl;
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
      
        
        if(this->path.length() > 0)
        {
            getFilePtr(this->file);
        }
    }
}


void IRSpectrogram::openFile(String path)
{
    
    this->file = File(path);
       
    this->path = path;
    if(this->path.length() > 0)
    {
        getFilePtr(this->file);
    }
}

void IRSpectrogram::getFilePtr(File file)
{
    // set a callback function which is called when file load is completed.
    // get a pointer of the audio file
    std::function<void()> callback = [this]{fileImportCompleted();};
    
    // create random ID to identify the retrieved ptr.
    KeRandomStringGenerator a;
    this->randomIDForPtr = a.createStrings(10);
    
    getFileManager().getFilePtrWithCallBack(IRFileType::IRAUDIO,
                                            file,
                                            this->parent,
                                            this->randomIDForPtr,
                                            callback);
    // notify changes to IRProject to update IRFileInspecter
    this->parent->notifyNodeObjectModification();

    
    
}

void IRSpectrogram::openButtonClicked()
{
    openFile();
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
    int fftsize = data->getFFTSize() / this->verticalScale;
    int ffthalfsize = data->getFFTSize() / 2;
    
    //visible area
    int w = this->visibleArea.getWidth();
    int h = this->visibleArea.getHeight();
    int x = this->visibleArea.getX();
    int y = this->visibleArea.getY();
    
    float maxTex_w = (getWidth() < this->MAX_TEXTURE_SIZE)? getWidth() : this->MAX_TEXTURE_SIZE;
    float maxTex_h = (getHeight() < this->MAX_TEXTURE_SIZE)? getHeight() : this->MAX_TEXTURE_SIZE;
    
    maxTex_w = this->MAX_TEXTURE_SIZE;
    maxTex_h = this->MAX_TEXTURE_SIZE;
    // viewport size
    float parentW = (float)w * this->zoomInfo.getX();
    float parentH = (float)h * this->zoomInfo.getY();
    
    
    
    
    //std::cout << " >>>>>> visible area " << x << " of (" << w << ", " << h << ") : parent " << parentW << ", " << parentH << " <<<<<< " << std::endl;
    float startFrame = (float)x * (float)nframe/parentW;
    float drawFrameNum = (float)(w) * (float)nframe / parentW;
    float startFFTSize =(float)y * (float)fftsize / parentH;
    float drawFFTSize = (float)(h) * (float)fftsize / parentH;
    
    float texture_w = (drawFrameNum < maxTex_w)?
    drawFrameNum : maxTex_w;
    float texture_h = (drawFFTSize < maxTex_h)?
    drawFFTSize : maxTex_h;
    
    float w_increment = texture_w / drawFrameNum;
    float h_increment = texture_h / drawFFTSize;
    
    // delete buffer if already allocated
    deleteBuffer();
    int texSize = (int)(texture_w) * (int)(texture_h);
    this->buffer = new float [texSize];
    
    // use normalized data 0.0~1.0
    const float* power = data->getNormalizedData();

    int i;
    // initialize
    for(i=0;i<texSize;i++) this->buffer[i] = 0.0;
    
    int currentX = (int)(startFrame);
    int currentY = (int)(startFFTSize);
    int endX = (int)(startFrame + drawFrameNum);
    int endY = (int)(startFFTSize + drawFFTSize);
    
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
   
    //std::cout << "==========\n";
    //std::cout <<"texture size = " << texture_w << " h " << texture_h << " : texSize = " << texSize << std::endl;
    
    //std::cout << "start x = " << currentX << " to " << endX << " of " << nframe << " : startY " << currentY << " to " << endY << " of "<< fftsize<<std::endl;
 
    this->sp_w = (int)(texture_w);
    this->sp_h = (int)(texture_h);
    //std::cout << "sp_w = " << this->sp_w << ", " << this->sp_h << " : texSize = "<< texSize << " : nframe "<< nframe << " : fftsize = " << fftsize << std::endl;
    //std::cout << "w_increment " << w_increment << " h_increment " << h_increment << std::endl;

    float tex_x = 0;
    float tex_y = 0;
    
    int texY_sp_w = 0;;
    
    int int_startFrame = (int)(startFrame);
    
    int bufferIndex = 0;
    int pIndex = 0;
    float p = 0.0;
    // texture
    bench3.start();
    
    

    while(1) // h
    {
        //std::cout << "y " << tex_y << " of " << endY << std::endl;
        if(tex_y >= this->sp_h) break;
        if(currentY >= endY) break;
        tex_x = 0;
        currentX = int_startFrame;
        texY_sp_w = (int)(tex_y) * this->sp_w;
        bench2.start();

        while(1) // w
        {
            if(tex_x >= this->sp_w) break;
            if(currentX >= endX) break;
            
            bufferIndex = texY_sp_w + (int)(tex_x);
            pIndex = (currentX * ffthalfsize + currentY);
            //pIndex = currentX + currentY;
            p = power[pIndex];

           // std::cout << "currentX = " << currentX << " of " << (int)(startFrame) << " : w =  " << this->sp_w <<std::endl;
            if(this->buffer[bufferIndex] < p) this->buffer[bufferIndex] = p;
           // std::cout << "buffer["<<bufferIndex <<"] = "<<this->buffer[(int)(int)(tex_x)] << " | " << (int)(int)(tex_x * ffthalfsize + tex_y) << " power[" << currentX << "][" << currentY << "] = " << p << std::endl;

            tex_x += w_increment;
            currentX += 1;
            //currentX += ffthalfsize;
            
        }

        tex_y += h_increment;
        currentY += 1;
    }
    bench3.result("while loop");
    
    //std::cout << "finish current x = " << currentX << " : y = " << currentY << std::endl;
    //std::cout << "finish tex x = " << tex_x << " : y = " << tex_y << std::endl;
    
}

void IRSpectrogram::loadDrawData(IRDescriptorStr* data)
{
  
    
}

void IRSpectrogram::setVisibleArea(Rectangle<int> area, juce::Point<int> parentSize)
{
    int s = getHeight();
    if(s > 50) s = 50;
    //int y = getHeight() / 2 - s / 2;
    
    // Subtract GridSize!! need to be fixed in smarter way
    this->visibleArea = Rectangle<int>(area.getX(),
                                       area.getY(),
                                       area.getWidth() - 20,
                                       area.getHeight() - 20);
    
    // follow always to the visibla area
    setBounds(this->visibleArea);
    
    this->parentSize = parentSize;
    
    
    // link current view position to other objects
    if(this->previousOffsetX != this->visibleArea.getX())
    {
        if(this->audioData != nullptr)
        {
            auto data = this->audioData->getData();
            data->setViewPortPosition(this->visibleArea.getPosition());
            data->linkViewPortPositionWithSharedComponents(nodeObject);
        }
        
        
    }
    this->previousOffsetX = this->visibleArea.getX();
    
    // just modify texture already created to save CUP usage
    reCalcDescriptor();
    
    createPlayingLine(getCurrentPlayedFrame());

    std::cout << "w, h " << getWidth() << std::endl;
}

void IRSpectrogram::parentSizeChanged(int w, int h)
{
    this->parentSize = juce::Point<int>(w, h);
    loadDescriptor();
}

// ==================================================
// from Preference GUI
void IRSpectrogram::setMagnitudeAmount(float val)
{
    this->magnitudeAmount = val;
    repaint();
}

void IRSpectrogram::setHorizontalScale(float val)
{
    this->horizontalScale = val;
    loadDescriptor();
}

void IRSpectrogram::setVerticalScale(float val)
{
    this->verticalScale = val;
    loadDescriptor();
}
// ==================================================
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
        
        bench.start();
        calcPixel(data->getDescriptor(FFTDescriptor::FFT_LinearPower,
                                      this->fftsize));
        std::cout << " ============== calc() : " << bench.stop() << " ms." << std::endl;
        
       
        this->isTextureCreated = false;
        updateFragment();
            
    }
}

void IRSpectrogram::reCalcDescriptor()
{
    
    if(this->audioUpdated)
    {
        auto data = this->audioData->getData();
        bench.start();

        calcPixel(data->getDescriptor(FFTDescriptor::FFT_LinearPower,
                                      this->fftsize));
        std::cout << " ============== reCalcDescriptor() : " << bench.stop() << std::endl;
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

    if (shader.get() == nullptr || shader->getFragmentShaderCode() != fragmentCode)
    {
        //shader.reset();
        if (fragmentCode.isNotEmpty() && this->fragmentRefreshed)
        {
            std::cout << "shader reset\n";
            shader.reset (new OpenGLGraphicsContextCustomShader (fragmentCode));
            std::cout << "shader activated\n";

            shader->onShaderActivated = [this](OpenGLShaderProgram& program){setUniform(program);};
            
            if(!this->isTextureCreated)
            {
                createTexture();
                this->isTextureCreated = true;
            }
            std::cout << "shader compiled\n";

            auto result = shader->checkCompilation (g.getInternalContext());
            if (result.failed()) shader.reset();
            
            this->fragmentRefreshed = false;
        }else{
            std::cout << "fragmentCode empty and not refreshed\n";
        }
    }else{
        std::cout << "shader null or shaderCode not loaded\n";
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

void IRSpectrogram::fileImportCompleted()
{
    std::cout << "fileImportCompleted\n";
    this->audioData = static_cast<DataAllocationManager<IRAudio>*>(getFileManager().getFileObjectAndRemoveFromBuffer(this->randomIDForPtr));
    
    if(this->audioData != nullptr)
    {
        this->audioData->getData()->addListener(this);

        this->audioUpdated = true;
        
        loadDescriptor();
        
        // remove openButton
        removeChildComponent(&this->openButton);
        
        bringThisToFront();

        // call virtual function
        fileImportCompletedAction();
        
        if(this->audioFileImportCompletedCallback != nullptr)
            this->audioFileImportCompletedCallback();
    }else{
        KLib().showErrorMessage("Could not load audio data of " + this->path);
    }
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


// --------------------------------------------------
void IRSpectrogram::linkZoomInfo(Component* comp)
{
    if(this->audioData != nullptr)
    {
        auto data = this->audioData->getData();
        data->linkZoomInOutWithSharedComponents(comp);
    }
}

void IRSpectrogram::linkCurrentPlayedFrame(Component* comp)
{
    if(this->audioData != nullptr)
    {
        auto data = this->audioData->getData();
        data->linkAudioPlaywithSharedComponents(comp);
    }
}

// --------------------------------------------------


void IRSpectrogram::zoomInOutOperatedFromComponent(IRAudio* obj)
{
    auto comp = obj->getEmittingComponent();
    // check if the emmiting component is not this object otherwise we will face on the infinitive loop
    if(comp != nodeObject)
    {
        
        setZoomInfo(obj->getZoomInfo());
        std::cout << "zoominfo of "<< nodeObject << " = " << this->zoomInfo.getX() <<  " from " << comp << std::endl;
        
        /*
        this->status = zoomInfoShared;
        sendChangeMessage();
         */
        if(this->zoomInOutSharedCallback != nullptr) this->zoomInOutSharedCallback();
        
    }else{
        std::cout <<"zoomInfo same \n";
    }
}
void IRSpectrogram::audioPlayOperatedFromComponent(IRAudio* obj)
{
    auto comp = obj->getEmittingComponent();
    // check if the emmiting component is not this object otherwise we will face on the infinitive loop
    if(comp != nodeObject)
    {
        setCurrentPlayedFrame(obj->getCurrentPlayedFrame());
        createPlayingLine(obj->getCurrentPlayedFrame());
        //this->status = currentPlayedFrameShared;
        //sendChangeMessage();
        if(this->currentPlayedFrameSharedCallback != nullptr)
            this->currentPlayedFrameSharedCallback();
    }
}

void IRSpectrogram::viewPortPositionFromComponent(IRAudio *obj)
{
    
    auto comp = obj->getEmittingComponent();
    // check if the emmiting component is not this object otherwise we will face on the infinitive loop
    if(comp != nodeObject)
    {
        
        std::cout << "set new visible x " << obj->getViewPortPosition().getX() << " : previous = " << this->visibleArea.getX() << std::endl;
        this->visibleArea.setX(obj->getViewPortPosition().getX());
        this->visibleArea.setY(obj->getViewPortPosition().getY());
        
        // follow always to the visibla area
        setBounds(this->visibleArea);
        this->previousOffsetX = this->visibleArea.getX();
        // just modify texture already created to save CUP usage
        reCalcDescriptor();
        
        if(this->viewPortPositionSharedCallback != nullptr)
            this->viewPortPositionSharedCallback();
    }
}
// --------------------------------------------------

void IRSpectrogram::updateAnimationFrame()
{
    if(this->audioData != nullptr)
    {
        /*
        auto data = this->audioData->getData();
        int64 playingPosition = this->player->getStartPosition() +
        this->player->getNextReadPosition();
        
        data->setCurrentPlayedFrame(playingPosition);
        
        data->linkAudioPlaywithSharedComponents(nodeObject);
        
        createPlayingLine(playingPosition);
         */
    }
}


//receive only at this m
void IRSpectrogram::createPlayingLine(int64 currentFrame)
{
    if(this->audioData != nullptr)
    {
        auto data = this->audioData->getData();
        double sampleRate = data->getSampleRate();
        int64 nframe = data->getNumSamples();
        
        int width = this->visibleArea.getWidth() * this->zoomInfo.getX();
      
        double ratio = (double)currentFrame / (double)nframe;
        
        int currentX = floor((double)width * ratio) - this->visibleArea.getX();
        
        //std::cout << "spectrogram w " << width << " currentX " << currentX << " visibleX = " << this->visibleArea.getX() << std::endl;
        
        if(currentX < this->visibleArea.getX() + this->visibleArea.getWidth())
        {
            this->playingLine = Rectangle<int>( currentX, 0, currentX, getHeight());

        }
        
        repaint();
    
    }
}
// --------------------------------------------------
