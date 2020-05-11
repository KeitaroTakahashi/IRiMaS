//
//  IRWorkspaceCover.hpp
//  IRiMaS
//
//  Created by Keitaro on 11/05/2020.
//

#ifndef IRWorkspaceCover_hpp
#define IRWorkspaceCover_hpp


#include "IRHeavyWeightComponent.h"
#include "IRStrComponent.hpp"
#include "KTextLoader.h"

class IRWorkspaceCover : public Component,
public IRHeavyWeightComponent,
public IRStrComponent
{
public:
    IRWorkspaceCover(IRStr* str);
    
    ~IRWorkspaceCover();
    
    
    // ==================================================
    void setEditMode(bool flag);
    
    bool isEditMode() const { return this->isEditModeFlag; }
    
    bool isDrawGrids() const { return this->isdrawGridsFlag; }
    void enableDrawGrids(bool flag) { this->isdrawGridsFlag = flag; }

    void setBackgroundColour(Colour colour);
    

    // ==================================================
    // ==================================================

    
private:
    // ==================================================

    void paint(Graphics& g) override;
    
    
    void resized() override;
    
    // ==================================================

    bool isEditModeFlag = true;
    bool isdrawGridsFlag = true;
    // ==================================================
    Colour backgroundColour = Colours::white;


    // ==================================================
       //OpenGL
       //OpenGLContext openGLContext;
       std::unique_ptr<OpenGLGraphicsContextCustomShader> shader;
       String fragmentCode;
       GLuint textureID;
       IRTextLoader fragmentText;
       bool isTextureCreated = false;
       bool updateTexture = false;
       float* buffer = nullptr;
       
       bool isOpenGLComponentClosed = false;
       
       String fragURL;
       
       bool fragmentRefreshed = false;
        
        
        int sp_w = 100;
        int sp_h = 100;
        // max size of w and h of Spectrogram
        // 1024
        int MAX_TEXTURE_SIZE = 512;
        
        float ratio_x = 1.0;
        float ratio_y = 1.0;
    
    // grids
    int thin_grids_pixel = 10;
    int thick_grids_interval = 50;
    float grid_thickness = 1.0;
    float grid_thickness2 = 0.5;
    
    float gridsBackgroundAlpha = 0.1;
    // ==================================================

    void openGLInit();
    


    void shaderTask(Graphics& g);
    

    void setUniform(OpenGLShaderProgram& program);
    

    // ==================================================

    
};
#endif /* IRWorkspaceCover_hpp */
