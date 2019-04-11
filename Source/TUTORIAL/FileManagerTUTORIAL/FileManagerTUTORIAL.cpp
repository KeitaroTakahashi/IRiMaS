//
//  FileManagerTUTORIAL.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 11/04/2019.
//

#include "FileManagerTUTORIAL.hpp"

FileManagerTUTORIAL::FileManagerTUTORIAL(Component* parent) :
IRNodeObject(parent, "FileManagerTutorial")
{
    
   

}

FileManagerTUTORIAL::~FileManagerTUTORIAL()
{
    if(this->imgData != nullptr)
        FILEMANAGER.discardFilePtr(IRFileType::IRIMAGE, this->imgData, this, this->imgFile);
    
    if(this->audioData != nullptr)
        FILEMANAGER.discardFilePtr(IRFileType::IRAUDIO, this->audioData, this, this->audioFile);
        
}


void FileManagerTUTORIAL::loadImageData(File file)
{
    // this is how to get a pointer of the requested image data
    // if the image data is not yet loaded, then this method will load it
    // otherwise, it will retrieves a pointer of the data from IRFileManager
    
    // it requires IRFileType, File, and a pointer of the owner (IRNodeObject)
    this->imgFile = file;
    this->imgData = static_cast<DataAllocationManager<IRImage>*>(FILEMANAGER.getFilePtr(
                                                                                        IRFileType::IRIMAGE,
                                                                                        this->imgFile,
                                                                                        this
                                                                                        ));
}

void FileManagerTUTORIAL::loadAudioData(File file)
{
    // this is how to get a pointer of the requested image data
    // if the image data is not yet loaded, then this method will load it
    // otherwise, it will retrieves a pointer of the data from IRFileManager
    
    // it requires IRFileType, File, and a pointer of the owner (IRNodeObject)
    this->audioFile = file;
    
    std::function<void()> callback = [this]{ fileImportCompleted();};
    FILEMANAGER.getFilePtrWithCallBack(IRFileType::IRAUDIO,
                                       this->audioFile,
                                       this->parent,
                                       callback);
    
    
}

// thread locked.
void FileManagerTUTORIAL::fileImportCompleted()
{
    this->audioData = static_cast<DataAllocationManager<IRAudio>*>(FILEMANAGER.getFileObject());
    
    //this->audioData->getData()->getAudioBuffer()
}
