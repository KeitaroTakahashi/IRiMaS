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
    setTitleText("Text");
    
    auto objects = IRFactory.getRegisteredObjectOfCategory(objectCategory::TEXTMENU);
    
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
    g.fillAll(SYSTEMCOLOUR.contents);
}

void TextMenuComponent::resized()
{
    ObjectMenuComponent::resized();
}
// ==================================================



// ==================================================
// ==================================================
// ==================================================
