//
//  ObjectMenuComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 11/08/2019.
//

#include "ObjectMenuComponent.hpp"

ObjectMenuComponent::ObjectMenuComponent(Rectangle<int> frameRect) :
frameRect(frameRect)
{
    this->title.setColour(Label::textColourId, SYSTEMCOLOUR.text);
    this->title.setJustificationType(Justification::centred);
    addAndMakeVisible(&this->title);
}

ObjectMenuComponent::~ObjectMenuComponent()
{
    
}

// ==================================================
void ObjectMenuComponent::paint(Graphics& g)
{
    g.fillAll(SYSTEMCOLOUR.contents);
    
}

void ObjectMenuComponent::resized()
{
    int y = 30;
    int x = getWidth() / 2;
    this->title.setBounds(0, 0, 100, 30);
    this->title.setCentrePosition(x, y);
    
    for(auto item : this->items)
    {
        int s = this->buttonSize + yMarge*2;
        y += s;
        item->setSize(getWidth(), s);
        item->setCentrePosition(x, y);
    }
}

// ==================================================
void ObjectMenuComponent::updateAnimationFrame()
{
   
}
// ==================================================
void ObjectMenuComponent::setTitleText(String text)
{
    this->title.setText(text, dontSendNotification);
}
// ==================================================

void ObjectMenuComponent::createItem(IRObjectFactory::t_object* obj)
{
    this->objects.push_back(obj);
    this->items.push_back(new ObjectMenuItem(this, obj, this->buttonSize));
    
    addAndMakeVisible(this->items[this->items.size() - 1]);
    
}
// ==================================================

void ObjectMenuComponent::ObjectMenuItemClicked(ObjectMenuItem* item)
{
    //deselect
    for(auto i : this->items)
    {
        if(i != item) i->setSelected(false);
    }
    
    //virtual function
    itemSelected(item->getObject());
}