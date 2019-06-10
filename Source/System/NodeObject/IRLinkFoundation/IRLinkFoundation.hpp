//
//  IRLinkFoundation.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 25/04/2019.
//

#ifndef IRLinkFoundation_hpp
#define IRLinkFoundation_hpp

#include "JuceHeader.h"
#include "IRLinkMenuObject.hpp"
#include "DataType.h"

// ==================================================
// data structures
// these structures are used for sharing data between objects.
struct IRLinkBase
{
    
};

struct IRAudioLink : IRLinkBase
{
    IRAudio data;
};


struct IRTextLink : IRLinkBase
{
    IRText data;
};

struct IRImageLink : IRLinkBase
{
    IRImage data;
};

struct IRDataLink : IRLinkBase
{
    IRData data;
};

// ==================================================
class IRLinkFoundation
{
public:
    // parent = IRWorkSpace
    IRLinkFoundation(Component* parent, int size);
    virtual ~IRLinkFoundation();
    
    // --------------------------------------------------
    // link mode
    bool isLinkMode() const;
    void setLinkMode(bool flag);
    virtual void setLinkModeEvent() {};
    
    // link activated
    bool isLinkActivated() const;
    void setLinkActivation(bool flag);
    virtual void setLinkActivationEvent() {};
    // linked with other objects
    bool isLinked() const;
    void setLinked(bool flag);
    virtual void setLinkedEvent() {};
    
    virtual void linkModeChangedEvent() {}; // for IRNodeObject
    virtual void linkActivatedChangedEvent() {};
    virtual void linkChangedEvent() {};
    // --------------------------------------------------
    void addLinkParam(IRLinkSystemFlag flag);
    virtual void LinkParamAddedEvent() {};
    void revemoLinkParam(IRLinkSystemFlag flag);
    virtual void LinkParamRemovedEvent() {};
    void clearLinkParam();
    virtual void LinkParamClearedEvent() {};
    std::vector<IRLinkSystemFlag> getLinkParams() const { return this->linkFlags; }
    
    // --------------------------------------------------
    
    void createLinkMenu();
    virtual void LinkMenuCreatedEvent() {};
    IRLinkMenuObject* getLinkMenu();
    void openLinkMenu();
    void closeLinkMenu();
    bool isLinkMenuOpened() const;
    
    void setLinkMenuCentrePosition(int x, int y);
    // receive the selected IRLinkSystemFlag from IRLinkMenuObject
    void receiveSelectedLinkMenuItem(IRLinkSystemFlag flag);
    virtual void receiveSelectedLinkMenuItemEvent() {};
    
    
    IRLinkSystemFlag selectedLinkSystemFlag;
    // --------------------------------------------------

    std::vector<IRLinkSystemFlag> linkFlags;
    std::vector<IRLinkBase*> linkBuffer;
    
    // ==================================================
    
    IRAudio* getAudioLink() { return this->audioLink; }
    IRText* getTextLink() { return this->textLink; }
    IRImage* getImageLink() { return this->imageLink; }
    IRData* getDataLink() { return this->dataLink; }
    IRVideo* getVideoLink() { return this->videoLink; }
    
    void setAudioLink(IRAudio* obj) { this->audioLink = obj; }
    void setTextLink(IRText* obj) { this->textLink = obj; }
    void setImageLink(IRImage* obj) { this->imageLink = obj; }
    void setDataLink(IRData* obj) { this->dataLink = obj; }
    void setVideoLink(IRVideo* obj) { this->videoLink = obj; }

    
    // --------------------------------------------------

private:
    
    Component* parent; // IRWorkSpace
    
    bool linkMenuOpenedFlag = false;
    bool linkModeFlag = false;
    bool linkActivationFlag = false;
    bool linkedFlag = false;
    
    int linkMenuSize = 60;

    std::shared_ptr<IRLinkMenuObject> linkMenu { nullptr };
    
    //link data
    IRAudio* audioLink = nullptr;
    IRText* textLink = nullptr;
    IRImage* imageLink = nullptr;
    IRData* dataLink = nullptr;
    IRVideo* videoLink = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRLinkFoundation)

};

#endif /* IRLinkFoundation_hpp */
