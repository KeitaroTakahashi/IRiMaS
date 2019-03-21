
#ifndef IRObjectFactory_hpp
#define IRObjectFactory_hpp


#include "JuceHeader.h"

#include "IRObjectCreaterBase.hpp"
#include "IRObjectCreater.hpp"



class IRObjectFactory
{
public:
    
    struct t_object
    {
        std::string id;
        std::string name;
        // Image img; // if I leave this as is, I get the leak...
        IRObjectCreaterBase* obj;
        
        t_object() {}
        
        // DUFEU TESTING IMAGE LEAK
        t_object(std::string id, std::string name, IRObjectCreaterBase* obj)
        {
            this->id = id;
            this->name = name;
            this->obj = obj;
        }
        
        /*
        t_object(std::string id, std::string name, Image img, IRObjectCreaterBase* obj)
        {
            this->id = id;
            this->name = name;
            this->img = img;
            this->obj = obj;
        }
         */
    };
    
    
    IRObjectFactory();
    ~IRObjectFactory();
    
    template<class T>
    void registerObject(std::string id, std::string name, Image img);
    
    IRNodeObject* createObject(std::string id, Component* parent);
    
    void showRegisteredObjectList();
    
    std::map<std::string, t_object> getRegisteredObjectList() const // FD leaving this here because of scope resolution design to check with t_object
    {
        return this->list;
    }
    
    
private:
    
    std::map<std::string, t_object> list;
    
};


// template function should be written outside of the class - FD could not put this in .cpp - to investigate further once global file separation done
template<class T>
void IRObjectFactory::registerObject(std::string id, std::string name, Image img)
{
    
    // IRObjectCreater<T>* obj = std::unique_ptr<IRObjectCreater<T>
    // std::unique_ptr<IRObjectCreater<T>> obj(new IRObjectCreater<T>);
    
    IRObjectCreater<T>* obj = new IRObjectCreater<T>();
    // this->list[id] = IRObjectFactory::t_object(id, name, img, obj);
    this->list[id] = IRObjectFactory::t_object(id, name, obj); // THIS WORKS ALL RIGHT
    
    std::cout<< id << " registered : size is " << this->list.size() << std::endl;
    // showRegisteredObjectList();
}

#endif /* IRObjectFactory_hpp */
