//
//  GLSLSpectrogram.cpp
//  IRiMaS
//
//  Created by Keitaro on 26/06/2019.
//

#include "GLSLSpectrogram.hpp"

GLSLSpectrogram::GLSLSpectrogram(IRNodeObject* nodeObject) :
IRUIAudioFoundation(nodeObject)
{
    setOpaque(false);
    
    openGLContext.setRenderer (this);
    openGLContext.attachTo (*this);
    openGLContext.setContinuousRepainting (true);
    openGLContext.setSwapInterval(30);
    this->isRenderingFlag = true;
    
    setSize(500, 500);
    
    createSpectrogramView();
    
    this->test.setButtonText("Test");
    addAndMakeVisible(&this->test);
    
}

GLSLSpectrogram::~GLSLSpectrogram()
{
    this->openGLContext.detach();
}

void GLSLSpectrogram::resized()
{
    this->test.setBounds(10,10,200,200);
}

void GLSLSpectrogram::newOpenGLContextCreated()
{
    freeAllContextObjects();
    createShaders();
    
}

void GLSLSpectrogram::openGLContextClosing()
{
    freeAllContextObjects();
}

void GLSLSpectrogram::createShaders()
{
    File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + "/Contents/Resources/materials/Sources/GLSL/Spectrogram/spectrogram.vert");
    
    IRTextLoader vertexText;
    vertexText.load(file.getFullPathName().toStdString());
    
    this->vertexShader = vertexText.getConstChar();
    
    File file2 = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + "/Contents/Resources/materials/Sources/GLSL/Spectrogram/spectrogram.frag");
    
    IRTextLoader fragmentText;
    fragmentText.load(file2.getFullPathName().toStdString());
    
    this->fragmentShader = fragmentText.getConstChar();
    
    std::unique_ptr<OpenGLShaderProgram> newShader (new OpenGLShaderProgram (openGLContext));
    String statusText;
    
    if (newShader->addVertexShader (OpenGLHelpers::translateVertexShaderToV3 (vertexShader))
        && newShader->addFragmentShader (OpenGLHelpers::translateFragmentShaderToV3 (fragmentShader))
        && newShader->link())
    {
        plane     .reset();
        attributes.reset();
        uniforms  .reset();
        
        shader.reset (newShader.release());
        shader->use();
        
        plane     .reset (new ShapeSp (openGLContext, this->mesh));
        attributes.reset (new AttributesSp (openGLContext, *shader));
        uniforms  .reset (new UniformsSp (openGLContext, *shader));
        
        statusText = "GLSL: v" + String (OpenGLShaderProgram::getLanguageVersion(), 2);
    }
    else
    {
        statusText = newShader->getLastError();
    }
}

void GLSLSpectrogram::createSpectrogramView()
{

    // init
    this->mesh.vertices.clear();
    this->mesh.indices.clear();
    this->mesh.normals.clear();
    this->mesh.textureCoords.clear();
    this->mesh = WavefrontObjFile::Mesh();

    
    std::cout << "createSpectrogramView wh = " << getWidth() << ", " << getHeight() << std::endl;
    
    
    int i, j;
    float w = (float)getWidth();
    float h = (float)getHeight();
    float h_w = w / 2.0;
    float h_h = h / 2.0;
    int w_1 = w - 1;
    int h_1 = h - 1;
    
    float div = 1.0;
    int index = 0;
    
    int64 t = Time::currentTimeMillis();
    for(i = 0; i < int(w); i ++)
    {
        for(j = 0; j < int(h); j ++)
        {
            float x = ((float)i - h_w)*div;
            float y = ((float)j - h_h)*div;
            
            WavefrontObjFile::Vertex vert;
            vert.x = x;
            vert.y = y;
            vert.z = 0.0f;
            this->mesh.vertices.add(vert);
            
            WavefrontObjFile::TextureCoord tex;
            
            tex.x = j / h;
            
            this->mesh.textureCoords.add(tex);
            
         
            if(i < w_1 && j < h_1)
            {
                 this->mesh.indices.add(index);
                 this->mesh.indices.add(index + h);
                 this->mesh.indices.add(index + h + 1);
                
                 this->mesh.indices.add(index);
                 this->mesh.indices.add(index + 1);
                 this->mesh.indices.add(index + h + 1);
                
            }
            
            //std::cout << (float)i/div << ", " << (float)j/div << std::endl;
            index ++;
        }

    }
    this->isMeshUpdated = true;
    //std::vector<sp_normalStr> f;
    
    repaint();
    
}

void GLSLSpectrogram::updateShader()
{
    if(this->isMeshUpdated)
    {
        plane     .reset();
        plane     .reset (new ShapeSp      (openGLContext, this->mesh));
    
        triggerAsyncUpdate();
        
        this->isMeshUpdated = false;
    }
}



void GLSLSpectrogram::freeAllContextObjects()
{
    plane     .reset();
    shader    .reset();
    attributes.reset();
    uniforms  .reset();
    //texture   .release();
}

void GLSLSpectrogram::renderOpenGL()
{
    jassert(OpenGLHelpers::isContextActive());
    
    auto desktopScale = (float) this->openGLContext.getRenderingScale();
    
    OpenGLHelpers::clear(Colours::black);

    updateShader();
    
    if(shader.get() == nullptr)
        return;
    
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glViewport(0, 0, roundToInt(desktopScale*getWidth()), roundToInt (desktopScale * getHeight()));

    shader->use();
    
    
    if (uniforms->projectionMatrix.get() != nullptr)
        uniforms->projectionMatrix->setMatrix4 (getProjectionMatrix().mat, 1, false);
    
    if (uniforms->viewMatrix.get() != nullptr)
        uniforms->viewMatrix->setMatrix4 (getViewMatrix().mat, 1, false);
    
    /*
    if (uniforms->lightPosition.get() != nullptr)
        uniforms->lightPosition->set (this->lightPositionY, 5.0f, 15.0f, 0.0f);
     */
  
    // diffuse colour
    
    if (uniforms->materialColour.get() != nullptr)
        uniforms->materialColour->set (0.5f, 0.84f, 0.1f, 0.0f);
    
    if (uniforms->DiffuseColour.get() != nullptr)
        uniforms->DiffuseColour->set (0.13f, 1.0f, 0.09f, 0.0f);
    
    if (uniforms->SpecularColour.get() != nullptr)
        uniforms->SpecularColour->set (0.71f, 1.0f, 0.77f, 0.0f);
    
    plane->draw (openGLContext, *attributes, GL_TRIANGLES);
    
    // Reset the element buffers so child Components draw correctly
    openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, 0);
    openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
}

Matrix3D<float> GLSLSpectrogram::getProjectionMatrix() const
{
    auto w = 1.0f / (scale + 0.1f);
    auto h = w * getLocalBounds().toFloat().getAspectRatio (false);
    //return Matrix3D<float>::fromFrustum (-w, w, -h, h, 1.0f, 1200.0f);
    
    return Matrix3D<float>::fromFrustum (-w, w, -h, h, 1.0f, 1200.0f);

}

Matrix3D<float> GLSLSpectrogram::getViewMatrix() const
{
    auto viewMatrix = Vector3D<float> (0.0f, 0.0f, this->positionZ);
    //auto rotationMatrix = Matrix3D<float>::rotation ({ 0, 0, 0 });
    return viewMatrix;
}

void GLSLSpectrogram::setShaderProgram (const String& vertexShader, const String& fragmentShader)
{
    newVertexShader = vertexShader;
    newFragmentShader = fragmentShader;
}


void GLSLSpectrogram::startRendering()
{
    if(!isRendering())
    {
        this->openGLContext.setContinuousRepainting(true);
        this->isRenderingFlag = true;
    }
}

void GLSLSpectrogram::stopRendering()
{
    if(isRendering())
    {
        this->openGLContext.setContinuousRepainting(false);
        this->isRenderingFlag = false;
    }
}

// === private

void GLSLSpectrogram::handleAsyncUpdate()
{
    
}


// ==================================================
void GLSLSpectrogram::mouseDown(const MouseEvent& e)
{
    
}

void GLSLSpectrogram::mouseUp(const MouseEvent& e)
{

}


// ==================================================
