//
//  IRWorkSpaceLinkManager.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/04/2019.
//

#include "IRWorkSpaceLinkManager.hpp"

IRWorkSpaceLinkManager::IRWorkSpaceLinkManager()
{
    this->linkingObjects.clear();
}

IRWorkSpaceLinkManager::~IRWorkSpaceLinkManager()
{
    this->linkingObjects.clear();
}

bool IRWorkSpaceLinkManager::addLinkingObject(IRNodeObject* obj)
{
    
    bool flag = true;
    
    for(auto o : this->linkingObjects)
    {
        if(o->selectedLinkSystemFlag != obj->selectedLinkSystemFlag)
            flag = false;
    }
    
    if(flag){
        // do not add obj if already exists
        auto it = std::find(this->linkingObjects.begin(), this->linkingObjects.end(), obj);
        if(it == this->linkingObjects.end())
        {
            addLinkObjectExecute(obj);
            return true;
        }else return false;
    }
    else{
        std::cout << "The same LinkFlag is required to link each other!!\n ";
        auto it = std::find(this->linkingObjects.begin(), this->linkingObjects.end(), obj);
        if(it != this->linkingObjects.end())
            this->linkingObjects.erase(it);

        return false;
    }
}

void IRWorkSpaceLinkManager::addLinkObjectExecute(IRNodeObject* obj)
{
    
    this->linkingObjects.push_back(obj);
    obj->getLinkMenu()->setSelectedItem(obj->selectedLinkSystemFlag);
    if(this->linkingObjects.size() > 1)
    {
        printf("addLinkObjectExecute\n");
        // the first registered obj is the parent
        obj->callReceiveAudioLink(this->linkingObjects[0]->getAudioLink());
    }

    
}

void IRWorkSpaceLinkManager::showLinkingObjects()
{
    int index = 0;
    for(auto obj : this->linkingObjects)
    {
        std::cout << index << " : " << obj << " : " << obj->selectedLinkSystemFlag << std::endl;
    }
}


