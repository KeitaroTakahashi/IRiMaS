//
//  ShaderLib.hpp
//  OpenGLComponentTest - App
//
//  Created by Keitaro on 15/03/2019.
//

#ifndef ShaderLib_hpp
#define ShaderLib_hpp

#include "../JuceLibraryCode/JuceHeader.h"



struct Vertex
{
    float position[3];
    float normal[3];
    float colour[4];
    float texCoord[2];
};

struct Attributes
{
    Attributes (OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
    {
        position      .reset (createAttribute (openGLContext, shaderProgram, "position"));
        normal        .reset (createAttribute (openGLContext, shaderProgram, "normal"));
        sourceColour  .reset (createAttribute (openGLContext, shaderProgram, "sourceColour"));
        textureCoordIn.reset (createAttribute (openGLContext, shaderProgram, "textureCoordIn"));
    }
    
    void enable (OpenGLContext& glContext)
    {
        if (position.get() != nullptr)
        {
            glContext.extensions.glVertexAttribPointer (position->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), nullptr);
            glContext.extensions.glEnableVertexAttribArray (position->attributeID);
        }
        
        if (normal.get() != nullptr)
        {
            glContext.extensions.glVertexAttribPointer (normal->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*) (sizeof (float) * 3));
            glContext.extensions.glEnableVertexAttribArray (normal->attributeID);
        }
        
        if (sourceColour.get() != nullptr)
        {
            glContext.extensions.glVertexAttribPointer (sourceColour->attributeID, 4, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*) (sizeof (float) * 6));
            glContext.extensions.glEnableVertexAttribArray (sourceColour->attributeID);
        }
        
        if (textureCoordIn.get() != nullptr)
        {
            glContext.extensions.glVertexAttribPointer (textureCoordIn->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof (Vertex), (GLvoid*) (sizeof (float) * 10));
            glContext.extensions.glEnableVertexAttribArray (textureCoordIn->attributeID);
        }
    }
    
    void disable (OpenGLContext& glContext)
    {
        if (position.get() != nullptr)       glContext.extensions.glDisableVertexAttribArray (position->attributeID);
        if (normal.get() != nullptr)         glContext.extensions.glDisableVertexAttribArray (normal->attributeID);
        if (sourceColour.get() != nullptr)   glContext.extensions.glDisableVertexAttribArray (sourceColour->attributeID);
        if (textureCoordIn.get() != nullptr) glContext.extensions.glDisableVertexAttribArray (textureCoordIn->attributeID);
    }
    
    std::unique_ptr<OpenGLShaderProgram::Attribute> position, normal, sourceColour, textureCoordIn;
    
private:
    static OpenGLShaderProgram::Attribute* createAttribute (OpenGLContext& openGLContext,
                                                            OpenGLShaderProgram& shader,
                                                            const char* attributeName)
    {
        if (openGLContext.extensions.glGetAttribLocation (shader.getProgramID(), attributeName) < 0)
            return nullptr;
        
        return new OpenGLShaderProgram::Attribute (shader, attributeName);
    }
};

//==============================================================================
// This class just manages the uniform values that the demo shaders use.
struct Uniforms
{
    Uniforms (OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
    {
        projectionMatrix.reset (createUniform (openGLContext, shader, "projectionMatrix"));
        viewMatrix      .reset (createUniform (openGLContext, shader, "viewMatrix"));
        lightPosition   .reset (createUniform (openGLContext, shader, "lightPosition"));
        materialColour  .reset (createUniform (openGLContext, shader, "materialColour"));
        DiffuseColour  .reset (createUniform (openGLContext, shader, "diffuseColour"));
        SpecularColour  .reset (createUniform (openGLContext, shader, "specularColour"));
        
        positiveStretch .reset (createUniform (openGLContext, shader, "positiveStretch"));
        negativeStretch      .reset (createUniform (openGLContext, shader, "negativeStretch"));
        
        stretch_amount  .reset (createUniform (openGLContext, shader, "stretch_amount"));
        
        positiveTBStretch  .reset (createUniform (openGLContext, shader, "positiveTBStretch"));

        positiveTBStretchPos  .reset (createUniform (openGLContext, shader, "positiveTBStretchPos"));

        negativeTBStretch  .reset (createUniform (openGLContext, shader, "negativeTBStretch"));
        
        negativeTBStretchPos  .reset (createUniform (openGLContext, shader, "negativeTBStretchPos"));

        stretchX  .reset (createUniform (openGLContext, shader, "stretchX"));
        stretchY  .reset (createUniform (openGLContext, shader, "stretchY"));

        
        vertexOffset    .reset (createUniform (openGLContext, shader, "vertexOffset"));
        vertexScaleIn   .reset (createUniform (openGLContext, shader, "vertexScaleIn"));
        vertexScaleOut  .reset (createUniform (openGLContext, shader, "vertexScaleOut"));
        
        vertexXYZAmount  .reset (createUniform (openGLContext, shader, "vertexXYZAmount"));
        
        
        
        
        /*
         texture         .reset (createUniform (openGLContext, shader, "demoTexture"));
         bouncingNumber  .reset (createUniform (openGLContext, shader, "bouncingNumber"));
         */
    }
    
    std::unique_ptr<OpenGLShaderProgram::Uniform> projectionMatrix,
    viewMatrix,
    lightPosition,
    materialColour,
    DiffuseColour,
    SpecularColour,
    positiveStretch,
    negativeStretch,
    stretchX,
    stretchY,
    stretch_amount,
    positiveTBStretch,
    positiveTBStretchPos,
    negativeTBStretch,
    negativeTBStretchPos,
    vertexOffset,
    vertexScaleIn,
    vertexScaleOut,
    vertexXYZAmount;
    
private:
    static OpenGLShaderProgram::Uniform* createUniform (OpenGLContext& openGLContext,
                                                        OpenGLShaderProgram& shader,
                                                        const char* uniformName)
    {
        if (openGLContext.extensions.glGetUniformLocation (shader.getProgramID(), uniformName) < 0)
            return nullptr;
        
        return new OpenGLShaderProgram::Uniform (shader, uniformName);
    }
};


#endif /* ShaderLib_hpp */
