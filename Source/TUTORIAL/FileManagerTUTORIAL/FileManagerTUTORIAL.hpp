//
//  FileManagerTUTORIAL.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 11/04/2019.
//


/*
    This source code shows how to use IRFileManager System.
    The system can avoid loading the same file on memory in order to save memory space and also to enhance the performance.
    IRFileManager System also has an important role to realise the file sharing system to make any interactions between different IRNodeObjects.
 */

#ifndef FileManagerTUTORIAL_hpp
#define FileManagerTUTORIAL_hpp

#include "IRUIFoundation.hpp"
#include "IRNodeObject.hpp"
#include "IRFileManager.hpp"

/*
    To use IRFileManager System, you must inherit IRUIFoundation.
    IRUIFoundation contains all necessary members and methods to use IRUIFoundation.
    See cpp file for details.
 */

class FileManagerTUTORIAL : public IRUIFoundation
{
public:
    FileManagerTUTORIAL(IRNodeObject* parent);
    ~FileManagerTUTORIAL();
    
    
    void loadImageData(File file);
    void loadAudioData(File file);
    
    void fileImportCompleted();
    
private:
    
    // we need a pointer of IRNodeObject to register owners pointer.
    // at this moment, only IRNodeObject can use IRFileManager when loading any files
    IRNodeObject* parent;
    
    // This is how to declare data type managed by IRFileManager
    File imgFile;
    DataAllocationManager<IRImage>* imgData = nullptr;
    File audioFile;
    DataAllocationManager<IRAudio>* audioData = nullptr;

};



#endif /* FileManagerTUTORIAL_hpp */
