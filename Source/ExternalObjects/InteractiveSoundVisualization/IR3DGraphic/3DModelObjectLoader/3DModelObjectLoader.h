//
//  3DModelObjectLoader.hpp
//  OpenGLComponentTest - App
//
//  Created by Keitaro on 15/03/2019.
//

#ifndef _DModelObjectLoader_hpp
#define _DModelObjectLoader_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include "DemoUtilities.h"
#include "WavefrontObjParser.h"

#include "ShaderLib.h"


//==============================================================================
/** This loads a 3D model from an OBJ file and converts it into some vertex buffers
 that we can draw.
 */
struct Shape
{
    Shape (OpenGLContext& openGLContext)
    {
        
        File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + "/Contents/Resources/materials/3DModel/sphere64.obj");
        
        if (shapeFile.load (loadEntireAssetIntoString (file.getFullPathName().toStdString().c_str())).wasOk())
            for (auto* s : shapeFile.shapes){
                vertexBuffers.add (new VertexBuffer (openGLContext, *s));
                
                std::cout << "shape : name " << s->name << std::endl;
            }
    }
    
    void draw (OpenGLContext& openGLContext, Attributes& attributes, GLenum mode)
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
        VertexBuffer (OpenGLContext& context, WavefrontObjFile::Shape& shape) : openGLContext (context)
        {
            numIndices = shape.mesh.indices.size();
            
            openGLContext.extensions.glGenBuffers (1, &vertexBuffer);
            openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
            
            Array<Vertex> vertices;
            createVertexListFromMesh (shape.mesh, vertices, Colours::green);
            
            openGLContext.extensions.glBufferData (GL_ARRAY_BUFFER, vertices.size() * (int) sizeof (Vertex),
                                                   vertices.getRawDataPointer(), GL_STATIC_DRAW);
            
            openGLContext.extensions.glGenBuffers (1, &indexBuffer);
            openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
            openGLContext.extensions.glBufferData (GL_ELEMENT_ARRAY_BUFFER, numIndices * (int) sizeof (juce::uint32),
                                                   shape.mesh.indices.getRawDataPointer(), GL_STATIC_DRAW);
        }
        
        ~VertexBuffer()
        {
            openGLContext.extensions.glDeleteBuffers (1, &vertexBuffer);
            openGLContext.extensions.glDeleteBuffers (1, &indexBuffer);
        }
        
        void bind()
        {
            openGLContext.extensions.glBindBuffer (GL_ARRAY_BUFFER, vertexBuffer);
            openGLContext.extensions.glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        }
        
        GLuint vertexBuffer, indexBuffer;
        int numIndices;
        OpenGLContext& openGLContext;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VertexBuffer)
    };
    
    WavefrontObjFile shapeFile;
    OwnedArray<VertexBuffer> vertexBuffers;
    
    static void createVertexListFromMesh (const WavefrontObjFile::Mesh& mesh, Array<Vertex>& list, Colour colour)
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
#endif /* _DModelObjectLoader_hpp */
