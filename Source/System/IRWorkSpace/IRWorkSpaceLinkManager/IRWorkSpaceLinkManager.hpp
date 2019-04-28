//
//  IRWorkSpaceLinkManager.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/04/2019.
//

#ifndef IRWorkSpaceLinkManager_hpp
#define IRWorkSpaceLinkManager_hpp

#include "JuceHeader.h"
#include "IRNodeObject.hpp"

class IRWorkSpaceLinkManager
{
public:
    
    IRWorkSpaceLinkManager();
    ~IRWorkSpaceLinkManager();
    

    bool addLinkingObject(IRNodeObject* obj);
    std::vector<IRNodeObject*> getLinkingObjects() const { return this->linkingObjects; }
    void removeAllLinkingObjects() { this->linkingObjects.clear(); }
    
    
    void showLinkingObjects();
    
private:
    
    void addLinkObjectExecute(IRNodeObject* obj);
    std::vector<IRNodeObject*> linkingObjects;
    
};
#endif /* IRWorkSpaceLinkManager_hpp */
