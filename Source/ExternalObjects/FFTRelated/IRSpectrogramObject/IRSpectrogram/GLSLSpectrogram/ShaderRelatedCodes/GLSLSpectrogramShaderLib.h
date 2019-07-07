//
//  GLGLSpectrogramShaderLib.h
//  IRiMaS
//
//  Created by Keitaro on 26/06/2019.
//

#ifndef GLGLSpectrogramShaderLib_h
#define GLGLSpectrogramShaderLib_h

#include "DemoUtilities.h"
#include "WavefrontObjParser.h"


namespace GLSLSpectrogramSpace
{
    // ==== vertex
    struct VertexSp
    {
        float position[3];
        float normal[3];
        float colour[4];
        float texCoord[2];
    };

    struct AttributesSp
    {
        AttributesSp (OpenGLContext& openGLContext, OpenGLShaderProgram& shaderProgram)
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
                glContext.extensions.glVertexAttribPointer (position->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof (VertexSp), nullptr);
                glContext.extensions.glEnableVertexAttribArray (position->attributeID);
            }
            
            if (normal.get() != nullptr)
            {
                glContext.extensions.glVertexAttribPointer (normal->attributeID, 3, GL_FLOAT, GL_FALSE, sizeof (VertexSp), (GLvoid*) (sizeof (float) * 3));
                glContext.extensions.glEnableVertexAttribArray (normal->attributeID);
            }
            
            if (sourceColour.get() != nullptr)
            {
                glContext.extensions.glVertexAttribPointer (sourceColour->attributeID, 4, GL_FLOAT, GL_FALSE, sizeof (VertexSp), (GLvoid*) (sizeof (float) * 6));
                glContext.extensions.glEnableVertexAttribArray (sourceColour->attributeID);
            }
            
            if (textureCoordIn.get() != nullptr)
            {
                glContext.extensions.glVertexAttribPointer (textureCoordIn->attributeID, 2, GL_FLOAT, GL_FALSE, sizeof (VertexSp), (GLvoid*) (sizeof (float) * 10));
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
    struct UniformsSp
    {
        UniformsSp (OpenGLContext& openGLContext, OpenGLShaderProgram& shader)
        {
            projectionMatrix.reset (createUniform (openGLContext, shader, "projectionMatrix"));
            viewMatrix      .reset (createUniform (openGLContext, shader, "viewMatrix"));
            lightPosition   .reset (createUniform (openGLContext, shader, "lightPosition"));
            materialColour  .reset (createUniform (openGLContext, shader, "materialColour"));
            DiffuseColour  .reset (createUniform (openGLContext, shader, "diffuseColour"));
            SpecularColour  .reset (createUniform (openGLContext, shader, "specularColour"));

        }
        
        std::unique_ptr<OpenGLShaderProgram::Uniform> projectionMatrix,
        viewMatrix,
        lightPosition,
        materialColour,
        DiffuseColour,
        SpecularColour;
        
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

   
        // ==================================================
    
    struct sp_vertexStr
    {
        float x,y,z;
        int index = 0;
        sp_vertexStr(float x, float y, int index)
        {
            this->x = x; this->y = y;
            this->z = -2.0f;
            this->index = index;
        }
    };
    
    struct sp_normalStr
    {
        int x, y, z;
        sp_normalStr(int x, int y, int z)
        {
            this->x = x; this->y = y; this->z = z;
        }
    };
        
    struct ShapeSp
    {
        ShapeSp (OpenGLContext& openGLContext, WavefrontObjFile::Mesh mesh)
        {
            
            if(vertexBuffers.size() > 0)
            {
                for(auto obj : vertexBuffers)
                {
                    delete obj;
                }
                vertexBuffers.clear();
            }
            std::unique_ptr<WavefrontObjFile::Shape> sh (new WavefrontObjFile::Shape());
            sh->name = "test";
            sh->mesh = mesh;
            vertexBuffers.add (new VertexBuffer (openGLContext, sh.release()));
            
                        
        }
        
        void draw (OpenGLContext& openGLContext, AttributesSp& attributes, GLenum mode)
        {
            for (auto* vertexBuffer : vertexBuffers)
            {
                vertexBuffer->bind();
                attributes.enable (openGLContext);
                glDrawElements (mode, vertexBuffer->numIndices, GL_UNSIGNED_INT, nullptr);
                attributes.disable (openGLContext);
            }
        }
        
    private:
        struct VertexBuffer
        {
            VertexBuffer (OpenGLContext& context, WavefrontObjFile::Shape* shape) : openGLContext (context)
            {
                this->shape = shape;
                numIndices = shape->mesh.indices.size();
                
                openGLContext.extensions.glGenBuffers (1, &vertexBuffer);
                openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
                
                Array<VertexSp> vertices;
                createVertexListFromMesh (shape->mesh, vertices, Colours::green);
                
                openGLContext.extensions.glBufferData (GL_ARRAY_BUFFER, vertices.size() * (int) sizeof (VertexSp),
                                                       vertices.getRawDataPointer(), GL_STATIC_DRAW);
                
                openGLContext.extensions.glGenBuffers (1, &indexBuffer);
                openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
                openGLContext.extensions.glBufferData (GL_ELEMENT_ARRAY_BUFFER, numIndices * (int) sizeof (juce::uint32),
                                                       shape->mesh.indices.getRawDataPointer(), GL_STATIC_DRAW);
            }
            
            ~VertexBuffer()
            {
                openGLContext.extensions.glDeleteBuffers (1, &vertexBuffer);
                openGLContext.extensions.glDeleteBuffers (1, &indexBuffer);
                
                delete this->shape;
            }
            
            void bind()
            {
                openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
                openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
            }
            
            GLuint vertexBuffer, indexBuffer;
            WavefrontObjFile::Shape* shape;
            int numIndices;
            OpenGLContext& openGLContext;
            
            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VertexBuffer)
        };
        
        WavefrontObjFile shapeFile;
        OwnedArray<VertexBuffer> vertexBuffers;
        
        static void createVertexListFromMesh (const WavefrontObjFile::Mesh& mesh, Array<VertexSp>& list, Colour colour)
        {
            auto scale = 0.2f;
            WavefrontObjFile::TextureCoord defaultTexCoord = { 0.5f, 0.5f };
            WavefrontObjFile::Vertex defaultNormal = { 0.5f, 0.5f, 0.5f };
            
            for (int i = 0; i < mesh.vertices.size(); ++i)
            {
                auto& v = mesh.vertices.getReference (i);
                
                auto& n = (i < mesh.normals.size() ? mesh.normals.getReference (i)
                           : defaultNormal);
                
                auto& tc = (i < mesh.textureCoords.size() ? mesh.textureCoords.getReference (i)
                            : defaultTexCoord);
                
                list.add ({ { scale * v.x, scale * v.y, scale * v.z, },
                    { scale * n.x, scale * n.y, scale * n.z, },
                    { colour.getFloatRed(), colour.getFloatGreen(), colour.getFloatBlue(), colour.getFloatAlpha() },
                    { tc.x, tc.y } });
            }
        }
    };
}

#endif /* GLGLSpectrogramShaderLib_h */
