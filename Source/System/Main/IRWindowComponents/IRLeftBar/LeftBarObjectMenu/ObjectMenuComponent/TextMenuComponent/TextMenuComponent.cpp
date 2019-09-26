//
//  TextMenuComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 12/08/2019.
//

#include "TextMenuComponent.hpp"

TextMenuComponent::TextMenuComponent(IRStr* str,Rectangle<int> frameRect) :
ObjectMenuComponent(str, frameRect)
{
    setTitleText("Texts");
    
    auto objects = getStr()->OBJECTFACTORY.getRegisteredObjectOfCategory(objectCategory::TEXTMENU);
    
    for(auto o : objects)
    {
        std::cout << o->name << std::endl;
        createItem(o);
    }
}

TextMenuComponent::~TextMenuComponent()
{
    
}


// ==================================================
void TextMenuComponent::paint(Graphics& g)
{
    g.fillAll(getStr()->SYSTEMCOLOUR.contents);
}

void TextMenuComponent::resized()
{
    ObjectMenuComponent::resized();
}
// ==================================================
void TextMenuComponent::itemSelected(IRObjectFactory2::t_object* obj)
{
    
}

void TextMenuComponent::itemReleased(IRObjectFactory2::t_object* obj)
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
