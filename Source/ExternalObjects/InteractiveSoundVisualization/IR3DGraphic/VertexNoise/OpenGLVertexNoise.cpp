//
//  OpenGLComponent.cpp
//  OpenGLComponentTest - App
//
//  Created by Keitaro on 15/03/2019.
//

#include "OpenGLVertexNoise.hpp"

OpenGLVertexNoise::OpenGLVertexNoise() :
positiveStretch(1.0f, 1.0f, 1.0f, 1.0f),
negativeStretch(1.0f, 1.0f, 1.0f, 1.0f),
stretchX(1.0f, 1.0f, 1.0f, 1.0f),
stretchY(1.0f, 1.0f, 1.0f, 1.0f),
materialColour(0.5f, 0.84f, 0.1f, 1.0f),
lightingColour(0.71f, 1.0f, 0.77f, 1.0f)
{
    
    if(auto* peer = getPeer())
        peer->setCurrentRenderingEngine(0);
        
    setOpaque(false);

    openGLContext.setRenderer (this);
    openGLContext.attachTo (*this);
    openGLContext.setContinuousRepainting (true);
    openGLContext.setSwapInterval(swapInterval);
    this->isRenderingFlag = true;
    
    stopRendering();
    /*
    this->test.setButtonText("test");
    this->test.setColour(TextButton::buttonColourId, Colours::transparentWhite);
    addAndMakeVisible(&this->test);
    */
    setSize(500, 500);
}

OpenGLVertexNoise::~OpenGLVertexNoise()
{
    this->openGLContext.detach();
}

void OpenGLVertexNoise::newOpenGLContextCreated()
{
    std::cout << "newOpenGLContextCreated\n";

    // nothing to do in this case - we'll initialise our shaders + textures
    // on demand, during the render callback.
    freeAllContextObjects();
    
    //if (controlsOverlay.get() != nullptr)
    //   controlsOverlay->updateShader();
    
    createShaders();
}

void OpenGLVertexNoise::openGLContextClosing()
{
    
    std::cout << "openGLContextClosing\n";
    // When the context is about to close, you must use this callback to delete
    // any GPU resources while the context is still current.
    freeAllContextObjects();
    
    // if (lastTexture != nullptr)
    //    setTexture (lastTexture);
}

void OpenGLVertexNoise::freeAllContextObjects()
{
    shape     .reset();
    shader    .reset();
    attributes.reset();
    uniforms  .reset();
    texture   .release();
}

void OpenGLVertexNoise::renderOpenGL()
{
    jassert(OpenGLHelpers::isContextActive());
    
    
    
    auto desktopScale = (float) this->openGLContext.getRenderingScale();
    
    OpenGLHelpers::clear(Colours::black);
    /*
    if(doBackgroundDrawing)
        drawBackground2DStuff(desktopScale);
     */
    
    updateShader();
    
    if(shader.get() == nullptr)
        return;
    
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //openGLContext.extensions.glActiveTexture(GL_TEXTURE0);
    //glEnable(GL_TEXTURE_2D);
    
    glViewport(0, 0, roundToInt(desktopScale*getWidth()), roundToInt (desktopScale * getHeight()));
    
    //texture.bind();
    
    //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    shader->use();
    
    
    if (uniforms->projectionMatrix.get() != nullptr)
        uniforms->projectionMatrix->setMatrix4 (getProjectionMatrix().mat, 1, false);
    
    if (uniforms->viewMatrix.get() != nullptr)
        uniforms->viewMatrix->setMatrix4 (getViewMatrix().mat, 1, false);
    
    if (uniforms->lightPosition.get() != nullptr)
        uniforms->lightPosition->set (this->lightPositionX,
                                      this->lightPositionY,
                                      this->lightPositionZ,
                                      0.0f);
    
    // diffuse colour
    
    if (uniforms->materialColour.get() != nullptr)
        uniforms->materialColour->set (this->materialColour.getVal1(),
                                       this->materialColour.getVal2(),
                                       this->materialColour.getVal3(),
                                       1.0f);

    
    if (uniforms->DiffuseColour.get() != nullptr)
        uniforms->DiffuseColour->set(this->lightingColour.getVal1(),
                                     this->lightingColour.getVal2(),
                                     this->lightingColour.getVal3(),
                                     1.0f);
    
    
    if (uniforms->SpecularColour.get() != nullptr)
        uniforms->SpecularColour->set (0.71f, 1.0f, 0.77f, 0.0f);
    
    // stretch
    
    if (uniforms->positiveStretch.get() != nullptr)
        uniforms->positiveStretch->set (this->positiveStretch.getVal1(),
                                        this->positiveStretch.getVal2(),
                                        this->positiveStretch.getVal3());
    
    if(uniforms->negativeStretch.get() != nullptr)
        uniforms->negativeStretch->set (this->negativeStretch.getVal1(),
                                        this->negativeStretch.getVal2(),
                                        this->negativeStretch.getVal3());
    
    if (uniforms->stretchX.get() != nullptr)
        uniforms->stretchX->set (this->stretchX.getVal1(),
                                 this->stretchX.getVal2(),
                                 this->stretchX.getVal3(),
                                 this->stretchX.getVal4());
    
    if (uniforms->stretchY.get() != nullptr)
        uniforms->stretchY->set (this->stretchY.getVal1(),
                                 this->stretchY.getVal2(),
                                 this->stretchY.getVal3(),
                                 this->stretchY.getVal4());
    
    if(uniforms->stretch_amount.get() != nullptr)
        uniforms->stretch_amount->set (this->stretchAmount);
    
    
    //vertex noise
    if (uniforms->vertexOffset.get() != nullptr){
        uniforms->vertexOffset->set (this->offsetX, this->offsetY, this->offsetZ);
    }
    
    if (uniforms->vertexScaleIn.get() != nullptr)
        uniforms->vertexScaleIn->set (this->fineness);
    
    if (uniforms->vertexScaleOut.get() != nullptr)
        uniforms->vertexScaleOut->set (this->intensity);
    
    if (uniforms->vertexXYZAmount.get() != nullptr)
        uniforms->vertexXYZAmount->set (this->amountX, this->amountY, this->amountZ);
    
    
    /*    if (uniforms->texture.get() != nullptr)
     uniforms->texture->set ((GLint) 0);
     
     
     
     
     if (uniforms->bouncingNumber.get() != nullptr)
     uniforms->bouncingNumber->set (bouncingNumber.getValue());
     
     */

  
    
    shape->draw (openGLContext, *attributes, GL_TRIANGLES);
    
    
    // Reset the element buffers so child Components draw correctly
    openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
    openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    
    
    this->rotation += (float) rotationSpeed;
    this->offsetX += this->vertexNoiseSpeed;
    this->offsetY += this->vertexNoiseSpeed;
    this->offsetZ += this->vertexNoiseSpeed;
    
    //this->lpy = (this->lpy + this->lightPositionYSpeed);
    
    
    // add counter one every frame
    this->counter ++;
    // it may cause error but can't avoid it.
    if(this->counter == ULONG_MAX) this->counter = 0;
}

Matrix3D<float> OpenGLVertexNoise::getProjectionMatrix() const
{
    auto w = 1.0f / (scale + 0.1f);
    auto h = w * getLocalBounds().toFloat().getAspectRatio (false);
    
    return Matrix3D<float>::fromFrustum (-w, w, -h, h, 4.0f, 100.0f);
}

Matrix3D<float> OpenGLVertexNoise::getViewMatrix() const
{
    // auto viewMatrix = draggableOrientation.getRotationMatrix()
    //* Vector3D<float> (0.0f, 1.0f, -10.0f);
    
    //auto viewMatrix = Vector3D<float> (0.0f, -0.5f, this->positionZ);
    auto viewMatrix = Vector3D<float> (this->transform.getVal1(), this->transform.getVal2(), this->transform.getVal3());

    //auto rotationMatrix = Matrix3D<float>::rotation ({ rotation, rotation, -0.3f });
    //auto rotationMatrix = Matrix3D<float>::rotation ({ 0, 30, -3 });
    auto rotationMatrix = Matrix3D<float>::rotation ({ 0, 0, 0 });

    //auto rotationMatrix = Matrix3D<float>::rotation ({ 0, 90, 0 });
    
    
    return rotationMatrix * viewMatrix;
    //return viewMatrix;
}

/*
 void OpenGLVertexNoise::setTexture (DemoTexture* t)
 {
 lastTexture = textureToUse = t;
 }*/

void OpenGLVertexNoise::setShaderProgram (const String& vertexShader, const String& fragmentShader)
{
    newVertexShader = vertexShader;
    newFragmentShader = fragmentShader;
}

void OpenGLVertexNoise::paint (Graphics& g)
{
    
}

void OpenGLVertexNoise::resized()
{
    //controlsOverlay->setBounds (getLocalBounds());
    draggableOrientation.setViewport (getLocalBounds());
    
    //this->test.setBounds(10, 10, 200, 100);
    
}






// === private

void OpenGLVertexNoise::handleAsyncUpdate()
{
    //controlsOverlay->statusLabel.setText (statusText, dontSendNotification);
}

void OpenGLVertexNoise::drawBackground2DStuff (float desktopScale)
{
    // Create an OpenGLGraphicsContext that will draw into this GL window..
    std::unique_ptr<LowLevelGraphicsContext> glRenderer (createOpenGLGraphicsContext (openGLContext,
                                                                                      roundToInt (desktopScale * getWidth()),
                                                                                      roundToInt (desktopScale * getHeight())));
    
    if (glRenderer.get() != nullptr)
    {
        Graphics g (*glRenderer);
        g.addTransform (AffineTransform::scale (desktopScale));
        
        for (auto s : stars)
        {
            auto size = 0.25f;
            
            // This stuff just creates a spinning star shape and fills it..
            Path p;
            p.addStar ({ getWidth()  * s.x.getValue(),
                getHeight() * s.y.getValue() },
                       7,
                       getHeight() * size * 0.5f,
                       getHeight() * size,
                       s.angle.getValue());
            
            auto hue = s.hue.getValue();
            
            g.setGradientFill (ColourGradient (Colours::green.withRotatedHue (hue).withAlpha (0.8f),
                                               0, 0,
                                               Colours::red.withRotatedHue (hue).withAlpha (0.5f),
                                               0, (float) getHeight(), false));
            g.fillPath (p);
        }
    }
}

void OpenGLVertexNoise::updateShader()
{
    if (newVertexShader.isNotEmpty() || newFragmentShader.isNotEmpty())
    {
        std::unique_ptr<OpenGLShaderProgram> newShader (new OpenGLShaderProgram (openGLContext));
        
        if (newShader->addVertexShader (OpenGLHelpers::translateVertexShaderToV3 (newVertexShader))
            && newShader->addFragmentShader (OpenGLHelpers::translateFragmentShaderToV3 (newFragmentShader))
            && newShader->link())
        {
            shape     .reset();
            attributes.reset();
            uniforms  .reset();
            
            shader.reset (newShader.release());
            shader->use();
            
            shape     .reset (new Shape      (openGLContext));
            attributes.reset (new Attributes (openGLContext, *shader));
            uniforms  .reset (new Uniforms   (openGLContext, *shader));
            
            statusText = "GLSL: v" + String (OpenGLShaderProgram::getLanguageVersion(), 2);
        }
        else
        {
            statusText = newShader->getLastError();
        }
        
        triggerAsyncUpdate();
        
        newVertexShader   = {};
        newFragmentShader = {};
    }
}


void OpenGLVertexNoise::createShaders()
{
    //File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + "/Contents/Resources/materials/Sources/GLSL/Lighting/lighting_vertex.txt");
    File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + "/Contents/Resources/materials/Sources/GLSL/Lighting/multipleLights_vertex.vert");
    
    IRTextLoader vertexText;
    vertexText.load(file.getFullPathName().toStdString());
    
    this->vertexShader = vertexText.getConstChar();
    
    
    // File file2 = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + "/Contents/Resources/materials/Sources/GLSL/Lighting/lighting_fragment.txt");
    File file2 = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + "/Contents/Resources/materials/Sources/GLSL/Lighting/multipleLights_fragment.frag");
    
    IRTextLoader fragmentText;
    fragmentText.load(file2.getFullPathName().toStdString());
    
    this->fragmentShader = fragmentText.getConstChar();
    
    std::unique_ptr<OpenGLShaderProgram> newShader (new OpenGLShaderProgram (openGLContext));
    String statusText;
    
    if (newShader->addVertexShader (OpenGLHelpers::translateVertexShaderToV3 (vertexShader))
        && newShader->addFragmentShader (OpenGLHelpers::translateFragmentShaderToV3 (fragmentShader))
        && newShader->link())
    {
        shape     .reset();
        attributes.reset();
        uniforms  .reset();
        
        shader.reset (newShader.release());
        shader->use();
        
        shape     .reset (new Shape (openGLContext));
        attributes.reset (new Attributes (openGLContext, *shader));
        uniforms  .reset (new Uniforms (openGLContext, *shader));
        
        statusText = "GLSL: v" + String (OpenGLShaderProgram::getLanguageVersion(), 2);
    }
    else
    {
        statusText = newShader->getLastError();
    }
}



// ================

void OpenGLVertexNoise::setLightPosition(KVector<float> val)
{
    this->lightPositionX = val.getVal1();
    this->lightPositionY = val.getVal2();
    this->lightPositionZ = val.getVal3();
}

void OpenGLVertexNoise::setQuaterStretchX(KVector<float> val)
{
    this->stretchX = val;
    //std::cout << "vertexNoise this->stretchX " << this->stretchX.getVal1() << ", " << this->stretchX.getVal2() << ", " << this->stretchX.getVal3() << ", " << this->stretchX.getVal3() << std::endl;

}
void OpenGLVertexNoise::setQuaterStretchY(KVector<float> val)
{
    this->stretchY = val;
    
    //std::cout << "vertexNoise this->stretchY " << this->stretchY.getVal1() << ", " << this->stretchY.getVal2() << ", " << this->stretchY.getVal3() << ", " << this->stretchY.getVal4() <<std::endl;
}
void OpenGLVertexNoise::setHalfStretchX(KVector<float> val)
{
    this->positiveStretch.setVal1(val.getVal1());
    this->negativeStretch.setVal1(val.getVal2());
}
void OpenGLVertexNoise::setHalfStretchY(KVector<float> val)
{
    this->positiveStretch.setVal2(val.getVal1());
    this->negativeStretch.setVal2(val.getVal2());
}
void OpenGLVertexNoise::setHalfStretchZ(KVector<float> val)
{
    this->positiveStretch.setVal3(val.getVal1());
    this->negativeStretch.setVal3(val.getVal2());
}
void OpenGLVertexNoise::setTransform(KVector<float> val)
{
    this->transform = val;
}

// ================

void OpenGLVertexNoise::setOffsetX(float offsetX)
{
    this->offsetX = offsetX;
}
void OpenGLVertexNoise::setOffsetY(float offsetY)
{
    this->offsetY = offsetY;
    
}
void OpenGLVertexNoise::setOffsetZ(float offsetZ)
{
    this->offsetZ = offsetZ;
    
}

void OpenGLVertexNoise::setVertexNoiseSpeed(float speed)
{
    this->vertexNoiseSpeed = speed;
}

void OpenGLVertexNoise::setFineness(float val)
{
    this->fineness = val;
}
void OpenGLVertexNoise::setIntensity(float val)
{
    this->intensity = val;
}

void OpenGLVertexNoise::setAmountX(float amountX)
{
    this->amountX = amountX;
}

void OpenGLVertexNoise::setAmountY(float amountY)
{
    this->amountY = amountY;
}

void OpenGLVertexNoise::setAmountZ(float amountZ)
{
    this->amountZ = amountZ;
}

void OpenGLVertexNoise::setPositiveStretch(float x, float y, float z)
{
    if(x >= 0) this->positiveStretch.setVal1(x);
    if(y >= 0) this->positiveStretch.setVal2(y);
    if(z >= 0) this->positiveStretch.setVal3(z);

}

void OpenGLVertexNoise::setNegativeStretch(float x, float y, float z)
{
    if(x >= 0) this->negativeStretch.setVal1(x);
    if(y >= 0) this->negativeStretch.setVal2(y);
    if(z >= 0) this->negativeStretch.setVal3(z);
}

void OpenGLVertexNoise::setStretchX(float x, float y, float z, float w)
{
    
    std::cout << "setStretch X " << x << ", " << y << std::endl;
    if(x >= 0) this->stretchX.setVal1(x);
    if(y >= 0) this->stretchX.setVal2(y);
    if(z >= 0) this->stretchX.setVal3(z);
    if(w >= 0) this->stretchX.setVal4(w);
}

void OpenGLVertexNoise::setStretchY(float x, float y, float z, float w)
{
    if(x >= 0) this->stretchY.setVal1(x);
    if(y >= 0) this->stretchY.setVal2(y);
    if(z >= 0) this->stretchY.setVal3(z);
    if(w >= 0) this->stretchY.setVal4(w);
}



void OpenGLVertexNoise::setStretchAmount(float amount)
{
    this->stretchAmount = amount;
}
// ================

void OpenGLVertexNoise::startRendering()
{
    if(!isRendering())
    {
        this->openGLContext.setContinuousRepainting(true);
        this->isRenderingFlag = true;
    }
}

void OpenGLVertexNoise::stopRendering()
{
    if(isRendering())
    {
        this->openGLContext.setContinuousRepainting(false);
        this->isRenderingFlag = false;
    }
}
// ================

void OpenGLVertexNoise::setMaterialColour(KVector<float> colour)
{
    this->materialColour = KVector<float>(colour.getVal1(),
                                          colour.getVal2(),
                                          colour.getVal3(),
                                          colour.getVal4());
    
}

void OpenGLVertexNoise::setLightingColour(KVector<float> colour)
{
    this->lightingColour =    KVector<float>(colour.getVal1(),
                                             colour.getVal2(),
                                             colour.getVal3(),
                                             colour.getVal4());
    

}



