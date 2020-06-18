//
//  IRVideoAnnotaterWorkspace.cpp
//  IRiMaS
//
//  Created by Keitaro on 09/05/2020.
//

#include "IRVideoAnnotaterWorkspace.hpp"

IRVideoAnnotaterWorkspace::IRVideoAnnotaterWorkspace(String title, Rectangle<int> draggableMargin, IRStr* str, bool withOpenButton) :
IRNodeObjectWorkspace(title, draggableMargin, str)
{
    enableDrawGrids(false);
    // initially create videoplayer and add
    this->videoPlayerObj.reset( new IRVideoPlayerObject2(this, str, withOpenButton) );
    //createParentObject(this->videoPlayerObj.get(), false);
    addAndMakeVisible(this->videoPlayerObj.get());
    setParentNodeObject(this->videoPlayerObj.get());
    this->videoPlayerObj->addMouseListener(this, true);
    this->videoPlayerObj->addListener(this);
    this->videoPlayerObj->addChangeListener(this);
    this->videoPlayerObj->videoLoadCompletedCallbackFunc = [this] { videoLoadCompletedAction(); };
    this->videoPlayerObj->videoPlayingUpdateCallbackFunc = [this] (double pos) { videoPlayingUpdateAction(pos); };

}

IRVideoAnnotaterWorkspace::~IRVideoAnnotaterWorkspace()
{
    this->videoPlayerObj.reset();
}
// ------------------------------------------------------------------------------------------

void IRVideoAnnotaterWorkspace::onResized()
{
    
    this->videoPlayerObj->setObjectBounds(getLocalBounds());
    
    //auto vs = this->videoPlayerObj->getVideoSize();
    //std::cout << "getVideoSize = " << vs.getX() << ", " << vs.getY() << std::endl;
    //std::cout << "getVideoObjectSize = " << this->videoPlayerObj->getWidth() << " , " << this->videoPlayerObj->getHeight() << std::endl;

}

void IRVideoAnnotaterWorkspace::onPaint(Graphics& g)
{
    g.drawText("IRVideoAnnotaterWorkspace", 0, 0, getWidth(), getHeight()/2, Justification::centred);
    g.fillAll(getStr()->SYSTEMCOLOUR.background);
}

void IRVideoAnnotaterWorkspace::setFixObjectSizeRatioWithOriginalSize(bool flag, Rectangle<float> originalSize)
{
    setFixObjectSizeRatio(flag, originalSize);

}
// ------------------------------------------------------------------------------------------


void IRVideoAnnotaterWorkspace::addAnnotationObject(IRNodeObject* obj, Rectangle<int> bounds)
{
    
}
// ------------------------------------------------------------------------------------------

void IRVideoAnnotaterWorkspace::copyAllDataToWorkspace(IRVideoAnnotaterWorkspace* newWorkspace)
{
    auto w = newWorkspace;
        
    auto p = static_cast<IRVideoPlayerObject2*>(this->videoPlayerObj->copyThis());
    std::cout << "IRVideoAnnotaterWorkspace copy VideoPlayerObj url = " << p->getVideoPlayer()->getMovieFile().getFullPathName() << std::endl;
    
    // replace videoPlayerObject
    //w->replaceVideoPlayerObject(p);
    
    double ratioW =  (double)getWidth() / (double)newWorkspace->getWidth();
    double ratioH = (double)getHeight() / (double)newWorkspace->getHeight();
    
    
    std::cout << "workspace ratio = " << ratioW << ", " << ratioH << std::endl;
    for(auto obj : newWorkspace->getObjects())
    {
        auto o = obj->copyThis();
        auto ob = obj->getBounds().toFloat();
        /*
        o->setObjectBounds(ob.getX() * ratioW,
                           ob.getY() * ratioH,
                           ob.getWidth() * ratioW,
                           ob.getHeight() * ratioH);*/
        
        o->setObjectBoundsRelative(obj->getObjectBoundsRelative());
        o->setBoundType(IRNodeComponentBoundsType::RELATIVE);
        o->setObjectBounds(obj->getBounds());
        o->setEventComponent(obj->getEventComponent());
        
        createObject(o);
        
        std::cout << "copy object on the workspace " << obj << " to " << o << " at " << o->getX() << ", " << o->getY() << " : " << o->getWidth() << ", " << o->getHeight() << std::endl;
    }
    
    
}

void IRVideoAnnotaterWorkspace::replaceVideoPlayerObject(IRVideoPlayerObject2* newVideoPlayer)
{
    if(this->videoPlayerObj.get() != nullptr)
    {
        //removeChildComponent(this->videoPlayerObj.get());
    }
    std::cout << "newVideoPlayer = " << newVideoPlayer << std::endl;
    this->videoPlayerObj.reset( newVideoPlayer );
    addAndMakeVisible(this->videoPlayerObj.get());
    setParentNodeObject(this->videoPlayerObj.get());
    this->videoPlayerObj->addMouseListener(this, true);
    this->videoPlayerObj->addListener(this);
    this->videoPlayerObj->addChangeListener(this);
    this->videoPlayerObj->videoLoadCompletedCallbackFunc = [this] { videoLoadCompletedAction(); };
    this->videoPlayerObj->videoPlayingUpdateCallbackFunc = [this] (double pos) { videoPlayingUpdateAction(pos); };
 
    //this->videoPlayerObj->setObjectBounds(0, 0, newVideoPlayer->getWidth(), newVideoPlayer->getHeight());
    //setBounds(newVideoPlayer->getLocalBounds());
    
    manageHeavyWeightComponents(true);
    
    std::cout << "IRVideoAnnotaterWorkspace::replaceVideoPlayerObject bounds = " << getWidth() << ", " << getHeight() << " : " << this->videoPlayerObj->getWidth() << ", " << this->videoPlayerObj->getHeight()<< std::endl;
    
}



// ------------------------------------------------------------------------------------------
void IRVideoAnnotaterWorkspace::videoLoadCompletedAction()
{
    manageHeavyWeightComponents(true);
    if(this->videoLoadCompletedCallback != nullptr)
        this->videoLoadCompletedCallback();
}
// ------------------------------------------------------------------------------------------


IRVideoPlayerObject2* IRVideoAnnotaterWorkspace::getVideoPlayerObject()
{
    if(this->videoPlayerObj.get() != nullptr)
        return this->videoPlayerObj.get();
    else return nullptr;
}

// ------------------------------------------------------------------------------------------

void IRVideoAnnotaterWorkspace::createNodeObjectOnWorkspace(IRNodeObject* obj)
{
    obj->setObjectBounds(getWidth()/2,
                         getHeight()/2,
                         getWidth()/4,
                         60);
    
    obj->setObjectBoundsRelative(0.5f, 0.5f, 0.25f, 0.25f);
    createObject(obj);
    obj->bringToFront();
    deselectAllObjects();
    //obj->setEventComponent(event);
    //event->setNodeObject(obj);
    obj->setSelected(true);
    // initially the object is shown
    //obj->setAnimated(true);
    //obj->setCurrentTimeCode(this->videoTransport.getPlayPosition());
}

void IRVideoAnnotaterWorkspace::createNodeObjectOnworkspaceAsSubtitle(IRNodeObject* obj)
{
    obj->setObjectBounds(0,
                         getHeight()-60,
                         getWidth(),
                         60);
    
    obj->setObjectBoundsRelative(0.0f, 0.8f, 1.0f, 0.2f);
    createObject(obj);
    obj->bringToFront();
    deselectAllObjects();
    obj->setSelected(true);
}

// ------------------------------------------------------------------------------------------
void IRVideoAnnotaterWorkspace::createAnnotationObject(IRNodeObject* obj, Component* event)
{
    // here, bounds coordination is set in relative to the parent component bounds
    obj->setBoundType(IRNodeComponentBoundsType::ORDINARY);
    
    obj->setEventComponent(event);
    createNodeObjectOnWorkspace(obj);
}

IRNodeObject* IRVideoAnnotaterWorkspace::createTextObject(Component* event)
{
    IRVATextEditorObject* nodeObj = static_cast<IRVATextEditorObject*>(IRObjectCreater<IRVATextEditorObject>().create(this,
                                                                getStr()));
    
    // here, bounds coordination is set in relative to the parent component bounds
    nodeObj->setBoundType(IRNodeComponentBoundsType::ORDINARY);
    nodeObj->setEventComponent(event);
    createNodeObjectOnWorkspace(nodeObj);

    return nodeObj;
}

IRNodeObject* IRVideoAnnotaterWorkspace::createShapeObject(Component* event)
{
    IRVAShapeObject* nodeObj = static_cast<IRVAShapeObject*>(IRObjectCreater<IRVAShapeObject>().create(this,
                                                                getStr()));
    nodeObj->setBoundType(IRNodeComponentBoundsType::ORDINARY);

    nodeObj->setEventComponent(event);
    createNodeObjectOnWorkspace(nodeObj);

    return nodeObj;
}

IRNodeObject* IRVideoAnnotaterWorkspace::createImageObject(Component* event)
{

    IRVAImageViewerObject* nodeObj = static_cast<IRVAImageViewerObject*>(IRObjectCreater<IRVAImageViewerObject>().create(this,
                                                                getStr()));
    nodeObj->setBoundType(IRNodeComponentBoundsType::ORDINARY);
    nodeObj->setEventComponent(event);
    createNodeObjectOnWorkspace(nodeObj);
    
    return nodeObj;

}
// --------------------------------------------------

void IRVideoAnnotaterWorkspace::updateVideoPlayingPos(double pos)
{
    videoPlayingUpdateAction(pos);
}



void IRVideoAnnotaterWorkspace::videoPlayingUpdateAction(double pos)
{
      //std::cout << "pos = " << pos << std::endl;
    // first reset the list of the visible annotation components

    bool shouldUpdateZOrder = false;
    
    using t = VideoAnnotationEventComponent::VideoAnnotationType;
    for(auto obj : getObjects())
    {
        auto event = static_cast<VideoAnnotationEventComponent*>(obj->getEventComponent());
        
        if(event == nullptr)
        {
            std:: cout << "ERROR : object " << obj->name << " does not have event!!\n";
            return;
        }
        
        std::cout << "event component begin = " << event->getBeginTimeCode() << " : " << event->getEndTimeCode() << std::endl;
        
        if(pos >= event->getBeginTimeCode() && event->getEndTimeCode() > pos)
        {
            std::cout << "in event activate? " << obj << " : " << obj->isActive() << std::endl;
            if(event->isActive())
            {
                if(!obj->isActive())
                {
                    event->setSelected(true);
                    event->selectedAction();
                    // show object
                    obj->setActive(true);
                    //obj->callReorderZIndex();
                    
                    shouldUpdateZOrder = true;
                    
                }
            }else{
                obj->setActive(false);
                shouldUpdateZOrder = true;

            }
        }else{
            //std::cout << "out event activate? " << obj << " : " << obj->isActive() << std::endl;
            if(event->isActive())
            {
                if(obj->isActive())
                {
                    event->setSelected(false);
                    // hide object
                    obj->setActive(false);
                    
                    //obj->callReorderZIndex();
                    
                    shouldUpdateZOrder = true;
                }
            }else{
                obj->setActive(false);
                shouldUpdateZOrder = true;

            }
            
        }

    }

    if(shouldUpdateZOrder)
    {
        manageHeavyWeightComponents(true);
    }
            
      
    if(this->videoPlayingUpdateCallback != nullptr)
        this->videoPlayingUpdateCallback(pos);
}

// ==============================

void IRVideoAnnotaterWorkspace::loadAndApplyIRSRT(t_json data)
{
    
    std::cout << "laoding...\n";
    auto va = data["IRVideoAnnotaterSaveData"];
    if(va.dump().length() == 0){
        std::cout << "Error : IRVideoAnnotater loadAndApplySRTs : save data empty!\n";
        return;
    }
    
    t_json objects = va["Objects"];
    if(objects.dump().length() == 0){
        std::cout << "Error : IRVideoAnnotater loadAndApplySRTs : There is no object.\n";
        return;
    }
    
    json11::Json::array objectArray = objects.array_items();
    
    std::cout << "objectArray num = " << objectArray.size() << std::endl;
    for(int i = 0; i < objectArray.size(); i ++)
    {
        for (auto it = objectArray[i].object_items().cbegin(); it != objectArray[i].object_items().cend(); ++it)
        {
            
            std::cout << " ===== " << it->first << " ===== " << std::endl;
            std::cout << "object type= " << it->second["objectType"].string_value() << std::endl;
            std::cout << "object uniqueID= " << it->second["objectUniqueID"].string_value() << std::endl;
            std::cout << "object status= " << it->second["status"].string_value() << std::endl;
            
            
            // ===== create object =====
            std::string objectTypeId = it->second["objectType"].string_value();
            //auto* obj = factory.createObject(objectTypeId, this, getStr());
            std::cout << "object created\n";


            json11::Json arrangeCtl = it->second["ArrangeController"];
            
            std::cout << "loadArrangeControllerSaveData\n";
            //loadArrangeControllerSaveData(obj, arrangeCtl);
            std::cout << "loadArrangeControllerSaveData done\n";

            //createObject(obj);
            
            // load save dada
            //obj->loadThisFromSaveData(it->second["ObjectDefined"]);
            
            // ===== END =====
        }
    }
}

// ==============================
