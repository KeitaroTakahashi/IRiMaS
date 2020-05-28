//
//  IRWorkspaceCover.cpp
//  IRiMaS
//
//  Created by Keitaro on 11/05/2020.
//

#include "IRWorkspaceCover.hpp"

IRWorkspaceCover::IRWorkspaceCover(IRStr* str) :
IRHeavyWeightComponent(this, "IRWorkspaceCover"),
IRStrComponent(str)
{
    setOpaque(true);

    openGLInit();
}

IRWorkspaceCover::~IRWorkspaceCover()
{
    
}
// ==================================================

void IRWorkspaceCover::paint(Graphics& g)
{
    g.fillAll(Colours::white);
    //g.fillAll(Colours::yellow);

    g.setColour(Colours::black);
    if(isDrawGrids() && isEditMode()) shaderTask(g);

}

void IRWorkspaceCover::resized() 
{
    
}
// ==================================================
void IRWorkspaceCover::setEditMode(bool flag)
{
    this->isEditModeFlag = flag;
    repaint();
}
void IRWorkspaceCover::setBackgroundColour(Colour colour)
{
    this->backgroundColour = colour;
    repaint();
}


// ==================================================

void IRWorkspaceCover::openGLInit()
{
    
    if (auto* peer = getPeer())
            peer->setCurrentRenderingEngine (0);
                
        String url = File::getSpecialLocation(File::currentApplicationFile).getFullPathName();
    
    #if JUCE_MAC
        url += "/Contents/Resources/materials/Sources/GLSL/grid/KGrid2.frag";
    #elif JUCE_IOS
        url += "/materials/Sources/GLSL/grid/KGrid2.frag";
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
    
    //std::cout << this->fragmentCode << std::endl;
    

        
}


void IRWorkspaceCover::shaderTask(Graphics& g)
{
    if (shader.get() == nullptr || shader->getFragmentShaderCode() != fragmentCode)
    {
       
        //shader.reset();
        if (fragmentCode.isNotEmpty() && this->fragmentRefreshed)
        {

            glEnable (GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            
            shader.reset (new OpenGLGraphicsContextCustomShader (fragmentCode));

            shader->onShaderActivated = [this](OpenGLShaderProgram& program){setUniform(program);};
           
            auto result = shader->checkCompilation (g.getInternalContext());
            if (result.failed()){ shader.reset();
                std::cout << "Error : shader compile error\n";
                std::cout << result.getErrorMessage() << std::endl;
            }
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

void IRWorkspaceCover::setUniform(OpenGLShaderProgram& program)
{
    
    // IMPORTANT : get the actual scale of the screen because MacOSX uses Retina display which has double piexles
    int scale = Desktop::getInstance().getDisplays().getMainDisplay().scale;
    int w = getWidth() * scale;
    int h = getHeight() * scale;
    
    program.setUniform("resolution", w, h);
    program.setUniform("gridInterval", this->thin_grids_pixel * scale, this->thick_grids_interval * scale);
    program.setUniform("sw", (float)isEditMode());
    program.setUniform("alpha", this->gridsBackgroundAlpha);
    
    //std::cout << "setUniform " << w << ", " << h << " : scale = " << scale << " : thinGrid = " << this->thin_grids_pixel * scale <<std::endl;
   
      
}

// ==================================================
