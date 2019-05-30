
#ifndef IRObjectFactory_hpp
#define IRObjectFactory_hpp


#include "IRObjectCreaterBase.hpp"
#include "IRObjectCreater.hpp"




class IRObjectFactory
{
public:
    
    struct t_object
    {
        std::string id;
        std::string name;
        Image img;
        
        // IRObjectCreaterBase* obj;
        std::shared_ptr<IRObjectCreaterBase> obj;
        
        t_object() {}
        ~t_object(){ printf("t_object deconstructor called\n");}
        
        // t_object(std::string id, std::string name, Image img, IRObjectCreaterBase* obj)
        t_object(std::string id, std::string name, Image img, std::shared_ptr<IRObjectCreaterBase> obj)
        {
            printf("t_object constructor called\n");
            this->id = id;
            this->name = name;
            this->img = img;
            this->obj = obj;
        }
    };
    
    
    IRObjectFactory();
    ~IRObjectFactory() // won't be called as is singleton?
    {
        // std::cout << "destructor of IRObjectFactory called" << std::endl; // I NEED FOR THIS TO HAPPEN...
    };
    
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
    
    // now trying with shared pointer
    
    std::cout<< id << " registering : size is " << this->list.size() << std::endl;

    
    std::shared_ptr<IRObjectCreater<T>> obj = std::make_shared<IRObjectCreater<T>>();
    // IRObjectCreater<T>* obj = new IRObjectCreater<T>();
    // std::cout << "hallo" << std::endl;
    this->list[id] = IRObjectFactory::t_object(id, name, img, obj);
    
    std::cout<< id << " registered : size is " << this->list.size() << std::endl;
    
}

#endif /* IRObjectFactory_hpp */
