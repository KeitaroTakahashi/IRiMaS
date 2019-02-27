//
//  IRObjectCreater.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 13/01/2019.
//

#ifndef IRObjectCreater_hpp
#define IRObjectCreater_hpp

#include <iostream>
#include <map>
#include "JuceHeader.h"
#include "IRNodeObject.hpp"

// ===========================================================================
// Object Factory

class IRObjectCreaterBase
{
public:
    IRObjectCreaterBase() {}
    virtual ~IRObjectCreaterBase() {}
    
    virtual IRNodeObject* create(Component* parent) = 0;
    std::string objectName;
    
};


template<class T>
class IRObjectCreater : public IRObjectCreaterBase
{
public:
    IRObjectCreater() {}
    ~IRObjectCreater() {}
    
    // ==================================================
    IRNodeObject* create(Component* parent) override
    {
        return new T(parent);
    }
    
    // ==================================================
private:
    
};


class IRObjectFactory
{
public:
    
    struct t_object
    {
        std::string id;
        std::string name;
        Image img;
        
        IRObjectCreaterBase* obj;
        
        t_object() {}
        
        t_object(std::string id, std::string name, Image img, IRObjectCreaterBase* obj)
        {
            this->id = id;
            this->name = name;
            this->img = img;
            this->obj = obj;
        }
    };
    
    
    IRObjectFactory() {}
    ~IRObjectFactory() {}
    
    // ==================================================
    template<class T>
    void registerObject(std::string id, std::string name, Image img);
    
    IRNodeObject* createObject(std::string id, Component* parent)
    {
        return this->list[id].obj->create(parent);
    }
    
    void showRegisteredObjectList()
    {
        printf("showRegisteredObjectList\n");
        for(auto item : this->list)
        {
            std::cout << item.first << std::endl;
        }
    }
    
    std::map<std::string, t_object> getRegisteredObjectList() const { return this->list; }
private:
    std::map<std::string, t_object> list;
};


// template function should be written outside of the class
template<class T>
void IRObjectFactory::registerObject(std::string id, std::string name, Image img)
{
    IRObjectCreater<T>* obj = new IRObjectCreater<T>();
    this->list[id] = IRObjectFactory::t_object(id,name,img, obj);
    
    std::cout<< id << " registered : size is " << this->list.size() << std::endl;
    
}





#endif /* IRObjectCreater_hpp */
