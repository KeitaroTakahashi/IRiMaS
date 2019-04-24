//
//  FileManagerTUTORIAL.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 11/04/2019.
//

#include "FileManagerTUTORIAL.hpp"

FileManagerTUTORIAL::FileManagerTUTORIAL(IRNodeObject* parent) :
IRUIFoundation(parent)
{
    
}

FileManagerTUTORIAL::~FileManagerTUTORIAL()
{
    if(this->imgData != nullptr)
        getFileManager()->discardFilePtr(IRFileType::IRIMAGE, this->imgData, this, this->imgFile);
    
    if(this->audioData != nullptr)
        getFileManager()->discardFilePtr(IRFileType::IRAUDIO, this->audioData, this, this->audioFile);
        
}


void FileManagerTUTORIAL::loadImageData(File file)
{
    // this is how to get a pointer of the requested image data
    // if the image data is not yet loaded, then this method will load it
    // otherwise, it will retrieves a pointer of the data from IRFileManager
    
    // it requires IRFileType, File, and a pointer of the owner (IRNodeObject)
    this->imgFile = file;
    this->imgData = static_cast<DataAllocationManager<IRImage>*>(getFileManager()->getFilePtr(
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
    getFileManager()->getFilePtrWithCallBack(IRFileType::IRAUDIO,
                                             this->audioFile,
                                             this->parent,
                                             callback);
    
    
}

// thread locked.
void FileManagerTUTORIAL::fileImportCompleted()
{
    this->audioData = static_cast<DataAllocationManager<IRAudio>*>(getFileManager()->getFileObject());
    
    //this->audioData->getData()->getAudioBuffer()
}
