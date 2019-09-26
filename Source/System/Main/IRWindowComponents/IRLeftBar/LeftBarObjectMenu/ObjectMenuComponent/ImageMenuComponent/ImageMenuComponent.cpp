//
//  ImageMenuComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 12/08/2019.
//

#include "ImageMenuComponent.hpp"


ImageMenuComponent::ImageMenuComponent(IRStr* str, Rectangle<int> frameRect) :
ObjectMenuComponent(str, frameRect)
{
    setTitleText("Graphics");
    
    auto objects = getStr()->OBJECTFACTORY.getRegisteredObjectOfCategory(objectCategory::IMAGEMENU);
    
    for(auto o : objects)
    {
        std::cout << o->name << std::endl;
        createItem(o);
    }
}

ImageMenuComponent::~ImageMenuComponent()
{
    
}


// ==================================================
void ImageMenuComponent::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.contents);
}

void ImageMenuComponent::resized()
{
    ObjectMenuComponent::resized();
}
// ==================================================

void ImageMenuComponent::itemSelected(IRObjectFactory2::t_object* obj)
{
    
}

void ImageMenuComponent::itemReleased(IRObjectFactory2::t_object* obj)
{
    
    
    auto topSpace = static_cast<IRWorkspace* >(getStr()->TopWorkspace);
    
    auto nodeObj = getStr()->OBJECTFACTORY.createObject(obj->id, topSpace);
    nodeObj->setCentrePosition(topSpace->getWidth()/2,
                               topSpace->getHeight()/2);
    topSpace->createObject(nodeObj);
    
    
}

// ==================================================
// ==================================================
// ==================================================
