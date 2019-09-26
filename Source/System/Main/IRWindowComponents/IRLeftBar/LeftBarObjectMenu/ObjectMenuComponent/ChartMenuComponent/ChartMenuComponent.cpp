//
//  ChartMenuComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 27/08/2019.
//

#include "ChartMenuComponent.hpp"


ChartMenuComponent::ChartMenuComponent(IRStr* str, Rectangle<int> frameRect) : ObjectMenuComponent(str, frameRect)
{
    setTitleText("Charts");
    auto objects = getStr()->OBJECTFACTORY.getRegisteredObjectOfCategory(objectCategory::CHARTMENU);
    
    for(auto o : objects)
    {
        std::cout << o->name << std::endl;
        createItem(o);
    }
}

ChartMenuComponent::~ChartMenuComponent()
{
    
}

void ChartMenuComponent::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.contents);
}

void ChartMenuComponent::resized()
{
    ObjectMenuComponent::resized();
}
// ==================================================

void ChartMenuComponent::itemSelected(IRObjectFactory2::t_object* obj)
{
    
}

void ChartMenuComponent::itemReleased(IRObjectFactory2::t_object* obj)
{
    
    
    auto topSpace = static_cast<IRWorkspace* >(getStr()->TopWorkspace);
    
    auto nodeObj = getStr()->OBJECTFACTORY.createObject(obj->id, topSpace);
    nodeObj->setCentrePosition(topSpace->getWidth()/2,
                               topSpace->getHeight()/2);
    topSpace->createObject(nodeObj);
}

// ==================================================

