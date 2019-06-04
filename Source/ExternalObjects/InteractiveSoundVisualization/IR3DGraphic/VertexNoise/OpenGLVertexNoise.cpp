//
//  OpenGLComponent.cpp
//  OpenGLComponentTest - App
//
//  Created by Keitaro on 15/03/2019.
//

#include "OpenGLVertexNoise.hpp"

OpenGLVertexNoise::OpenGLVertexNoise() :
materialColour(0.5f, 0.84f, 0.1f, 1.0f),
lightingColour(0.71f, 1.0f, 0.77f, 1.0f),
positiveStretch(1.0f, 1.0f, 1.0f, 1.0f),
negativeStretch(1.0f, 1.0f, 1.0f, 1.0f)
{
    /*
     if(auto* peer = getPeer())
     peer->setCurrentRenderingEngine(0);
     */
    
    setOpaque(true);
    //this->controlsOverlay.reset(new DemoController());
    //addAndMakeVisible (controlsOverlay.get());
    
    openGLContext.setRenderer (this);
    openGLContext.attachTo (*this);
    openGLContext.setContinuousRepainting (true);
    openGLContext.setSwapInterval(33);
    this->isRenderingFlag = true;
    
    stopRendering();
    
    setSize(500, 500);
}

OpenGLVertexNoise::~OpenGLVertexNoise()
{
    this->openGLContext.detach();
}

void OpenGLVertexNoise::newOpenGLContextCreated()
{
    // nothing to do in this case - we'll initialise our shaders + textures
    // on demand, during the render callback.
    freeAllContextObjects();
    
    //if (controlsOverlay.get() != nullptr)
    //   controlsOverlay->updateShader();
    
    createShaders();
}

void OpenGLVertexNoise::openGLContextClosing()
{
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
    
    if(doBackgroundDrawing)
        drawBackground2DStuff(desktopScale);
    
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
        uniforms->lightPosition->set (this->lightPositionY, 5.0f, 15.0f, 0.0f);
    
    // diffuse colour
    
    if (uniforms->materialColour.get() != nullptr)
        uniforms->materialColour->set (this->materialColour.getX(),
                                       this->materialColour.getY(),
                                       this->materialColour.getWidth(),
                                       this->materialColour.getHeight());

    
    if (uniforms->DiffuseColour.get() != nullptr)
        uniforms->DiffuseColour->set(this->lightingColour.getX(),
                                     this->lightingColour.getY(),
                                     this->lightingColour.getWidth(),
                                     this->lightingColour.getHeight());
    
    if (uniforms->SpecularColour.get() != nullptr)
        uniforms->SpecularColour->set (0.71f, 1.0f, 0.77f, 0.0f);
    
    // stretch
    
    if (uniforms->positiveStretch.get() != nullptr)
        uniforms->positiveStretch->set (this->positiveStretch.getX(),
                                        this->positiveStretch.getY(),
                                        this->positiveStretch.getWidth());
    
    if(uniforms->negativeStretch.get() != nullptr)
        uniforms->negativeStretch->set (this->negativeStretch.getX(),
                                        this->negativeStretch.getY(),
                                        this->negativeStretch.getWidth());
    
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
    //uniforms->lightPosition->set (-15.0f, 10.0f, 15.0f, 0.0f);
    
    
    /*    if (uniforms->texture.get() != nullptr)
     uniforms->texture->set ((GLint) 0);
     
     
     
     
     if (uniforms->bouncingNumber.get() != nullptr)
     uniforms->bouncingNumber->set (bouncingNumber.getValue());
     
     */
    
    
    // triangle
    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };
    GLuint vertexBuffer;
    openGLContext.extensions.glGenBuffers(1, &vertexBuffer);
    openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER,
                                          vertexBuffer);
    openGLContext.extensions.glBufferData(GL_ARRAY_BUFFER,
                                          sizeof(g_vertex_buffer_data),
                                          g_vertex_buffer_data,
                                          GL_STATIC_DRAW);
    
    openGLContext.extensions.glEnableVertexAttribArray(0);
    openGLContext.extensions.glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    openGLContext.extensions.glVertexAttribPointer(0,
                                                   3,
                                                   GL_FLOAT,
                                                   GL_FALSE,
                                                   0,
                                                   (void* )0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    openGLContext.extensions.glDisableVertexAttribArray(0);
    
    
    shape->draw (openGLContext, *attributes, GL_TRIANGLES);
    
    
    // Reset the element buffers so child Components draw correctly
    openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
    openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
    
    
    this->rotation += (float) rotationSpeed;
    //this->offsetX += rotationSpeed;
    //this->offsetY += rotationSpeed;
    //this->offsetZ += rotationSpeed;
    
    //this->lpy = (this->lpy + this->lightPositionYSpeed);
    this->lightPositionY = -15.0f;
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
    auto viewMatrix = Vector3D<float> (0.0f, 0.0f, this->positionZ);

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

void OpenGLVertexNoise::paint (Graphics&)  {}

void OpenGLVertexNoise::resized()
{
    //controlsOverlay->setBounds (getLocalBounds());
    draggableOrientation.setViewport (getLocalBounds());
    
    
    
    
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
    
    IRTextLoader vertexText (file.getFullPathName().toStdString());
    
    this->vertexShader = vertexText.getConstChar();
    
    
    // File file2 = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + "/Contents/Resources/materials/Sources/GLSL/Lighting/lighting_fragment.txt");
    File file2 = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + "/Contents/Resources/materials/Sources/GLSL/Lighting/multipleLights_fragment.frag");
    
    IRTextLoader fragmentText (file2.getFullPathName().toStdString());
    
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

void OpenGLVertexNoise::setFineness(float fineness)
{
    this->fineness = fineness;
}
void OpenGLVertexNoise::setIntensity(float intensity)
{
    this->intensity = intensity;
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
    if(x >= 0) this->positiveStretch.setX(x);
    if(y >= 0) this->positiveStretch.setY(y);
    if(z >= 0) this->positiveStretch.setWidth(z);

}

void OpenGLVertexNoise::setNegativeStretch(float x, float y, float z)
{
    if(x >= 0) this->negativeStretch.setX(x);
    if(y >= 0) this->negativeStretch.setY(y);
    if(z >= 0) this->negativeStretch.setWidth(z);
}

void OpenGLVertexNoise::setPositiveTBStretch(float x, float y, float z)
{
    if(x >= 0) this->positiveTBStretch.setX(x);
    if(y >= 0) this->positiveTBStretch.setY(y);
    if(z >= 0) this->positiveTBStretch.setWidth(z);
    
}

void OpenGLVertexNoise::setNegativeTBStretch(float x, float y, float z)
{
    if(x >= 0) this->negativeTBStretch.setX(x);
    if(y >= 0) this->negativeTBStretch.setY(y);
    if(z >= 0) this->negativeTBStretch.setWidth(z);
}

void OpenGLVertexNoise::setPositiveTBStretchPos(float x, float y, float z)
{
    if(x >= 0) this->positiveTBStretchPos.setX(x);
    if(y >= 0) this->positiveTBStretchPos.setY(y);
    if(z >= 0) this->positiveTBStretchPos.setWidth(z);
    
}

void OpenGLVertexNoise::setNegativeTBStretchPos(float x, float y, float z)
{
    if(x >= 0) this->negativeTBStretchPos.setX(x);
    if(y >= 0) this->negativeTBStretchPos.setY(y);
    if(z >= 0) this->negativeTBStretchPos.setWidth(z);
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
        //this->openGLContext.setContinuousRepainting(true);
        this->isRenderingFlag = true;
    }
}

void OpenGLVertexNoise::stopRendering()
{
    if(isRendering())
    {
        //this->openGLContext.setContinuousRepainting(false);
        this->isRenderingFlag = false;
    }
}
// ================

void OpenGLVertexNoise::setMaterialColour(Colour colour)
{
    this->materialColour = Rectangle<float>(colour.getFloatRed(),
                                               colour.getFloatGreen(),
                                               colour.getFloatBlue(),
                                               colour.getFloatAlpha());
}

void OpenGLVertexNoise::setLightingColour(Colour colour)
{
    this->lightingColour =    Rectangle<float>(colour.getFloatRed(),
                                               colour.getFloatGreen(),
                                               colour.getFloatBlue(),
                                               colour.getFloatAlpha());
}



