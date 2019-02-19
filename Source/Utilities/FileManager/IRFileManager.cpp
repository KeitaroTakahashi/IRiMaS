//
//  IRFileManager.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 05/02/2019.
//

#include "IRFileManager.hpp"


// *** MODIFICATION BY FREDERIC *** //


bool IRFileManager::isFileAlreadyHeld(FILE* newFile)
{
    FILE* f = this->list.find(newFile);
    if(f != nullptr) return true;
    else return false;
}

bool IRFileManager::isObjectAlreadyExist(IRObjectPtr obj)
{
    IRObjectPtr p = this->list.find(obj);
    if(p != nullptr) return true;
    else return false;
}

IRObjectPtr IRFileManager::retrieveAlreadyHeldFile(FILE* file)
{
    IRObjectPtr p = this->list.find(file);
    if(p != nullptr) return p;
    else return nullptr;
}

FILE* IRFileManager::retrieveAlreadyHeldFILE(IRObjectPtr obj)
{
    //FILE* f  = this->list.find(obj);
    //if(f != nullptr) return f;
    //else return nullptr;
}

void IRFileManager::registerNewFile(FILE file, IRObjectPtr obj)
{
    
}
