//
//  IRFileManager.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 05/02/2019.
//

#include "IRFileManager.hpp"


// *** MODIFICATION BY FREDERIC *** //


bool IRFileManager::isFileAlreadyRegistered(File* newFile)
{
    File* f = this->list.findFile(newFile);
    if(f != nullptr) return true;
    else return false;
}

bool IRFileManager::isObjectAlreadyRegistered(IRObjectPtr obj)
{
    IRObjectPtr p = this->list.findPtr(obj);
    if(p != nullptr) return true;
    else return false;
}

IRObjectPtr IRFileManager::retrievePtrByFile(File* file)
{
    IRObjectPtr p = this->list.findPtrByFile(file);
    if(p != nullptr) return p;
    else return nullptr;
}

File* IRFileManager::retrieveFileByPtr(IRObjectPtr obj)
{
    File* f  = this->list.findFileByPtr(obj);
    if(f != nullptr) return f;
    else return nullptr;
}

void IRFileManager::registerNewFile(File* file, IRObjectPtr obj)
{
    if(isFileAlreadyRegistered(file))
    {
        std::cout << "Error : IRFileManager::registerNewFile() : file already registered!" << file->getFullPathName() << " has been registered with obj " << this->list.findPtrByFile(file) << std::endl;
        return;
    }
    if(isObjectAlreadyRegistered(obj))
    {
        std::cout << "Error : IRFileManager::registerNewFile() : obj already registered!" << obj << " has been registered with File path "<< this->list.findFileByPtr(obj)->getFullPathName() << std::endl;
        return;
    }
    
    if(!this->list.add(file, obj))
    {
        KLib().showConnectionErrorMessage("Error : FileManager List size conflicts!! for " + file->getFullPathName() + "\n");
    }
}
