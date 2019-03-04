//
//  IRFileManager.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 05/02/2019.
//

#include "IRFileManager.hpp"

// -------------------------------------------------

IRObjectPtr IRFileManager::createFileData(IRFileType type, File file)
{
    
    switch(type)
    {
        case IRIMAGE:
            return createImageFileData(file);
            break;
        case IRVIDEO:
            break;
        case IRAUDIO:
            break;
        default:
            break;
    }
    return nullptr;
}

IRObjectPtr IRFileManager::createImageFileData(File file)
{
    IRImage* img = new IRImage(file);
    return static_cast<IRObjectPtr>(img);
}
// -------------------------------------------------

bool IRFileManager::isFileAlreadyRegistered(File newFile)
{
    return this->list.isFile(newFile);
}
// -------------------------------------------------

bool IRFileManager::isObjectAlreadyRegistered(IRObjectPtr obj)
{
    IRObjectPtr p = this->list.findPtr(obj);
    if(p != nullptr) return true;
    else return false;
}
// -------------------------------------------------

IRObjectPtr IRFileManager::retrievePtrByFile(File file)
{
    IRObjectPtr p = this->list.findPtrByFile(file);
    if(p != nullptr) return p;
    else return nullptr;
}
// -------------------------------------------------

File IRFileManager::retrieveFileByPtr(IRObjectPtr obj)
{
    return this->list.findFileByPtr(obj);
}
// -------------------------------------------------

void IRFileManager::registerNewFile(File file, IRObjectPtr obj)
{
    if(isFileAlreadyRegistered(file))
    {
        std::cout << "Error : IRFileManager::registerNewFile() : file already registered!" << file.getFullPathName() << " has been registered with obj " << this->list.findPtrByFile(file) << std::endl;
        return;
    }
    if(isObjectAlreadyRegistered(obj))
    {
        std::cout << "Error : IRFileManager::registerNewFile() : obj already registered!" << obj << " has been registered with File path "<< this->list.findFileByPtr(obj).getFullPathName() << std::endl;
        return;
    }
    
    std::cout << "register new file of " << file.getFullPathName() << " within " << obj << std::endl;
    if(!this->list.add(file, obj))
    {
        KLib().showConnectionErrorMessage("Error : FileManager List size conflicts!! for " + file.getFullPathName() + "\n");
    }
}
// -------------------------------------------------
//==================================================
IRObjectPtr IRFileManager::getFilePtr(IRFileType type, File file)
{
    
    std::cout << "IRFileManager ptr = " << this << std::endl;
    
    if(isFileAlreadyRegistered(file))
    {
        printf("file already imported to the project!\n");
        return retrievePtrByFile(file);
    }
    else
    {
        printf("create new object in getFilePtr()\n");
        IRObjectPtr newObj = createFileData(type, file);
        registerNewFile(file, newObj);
        return newObj;
    }
}
// -------------------------------------------------
IRObjectPtr IRFileManager::discardFilePtr(IRObjectPtr owner, File file)
{
    return nullptr;
}
//==================================================
