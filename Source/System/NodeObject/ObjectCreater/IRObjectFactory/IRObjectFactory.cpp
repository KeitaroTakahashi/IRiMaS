
#include "IRObjectFactory.hpp"




IRObjectFactory::IRObjectFactory()
{
    
}

/*
IRObjectFactory::~IRObjectFactory()
{
    
}
 */


IRNodeObject* IRObjectFactory::createObject(std::string id, Component* parent)
{
    return this->list[id].obj->create(parent);
}


void IRObjectFactory::showRegisteredObjectList()
{
    printf("showRegisteredObjectList\n");
    for (auto item : this->list)
    {
        std::cout << item.first << std::endl;
    }
}



