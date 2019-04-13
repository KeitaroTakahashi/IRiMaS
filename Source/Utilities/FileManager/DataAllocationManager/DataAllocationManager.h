//
//  DataAllocationManager.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 22/02/2019.
//

#ifndef DataAllocationManager_hpp
#define DataAllocationManager_hpp
#include "JuceHeader.h"
#include "DataType.h"

enum IRFileType
{
    IRIMAGE,
    IRVIDEO,
    IRAUDIO,
    IRAUDIO_THREADSAFE // IRAUDIO thread safe
};

// DO NOT SEPARATE THIS CLASS TO hpp AND cpp because THIS IS Template!!
template<class T>
class DataAllocationManager
{
public:
    
    DataAllocationManager()
    {
        this->owners.clear();
    }
    
    ~DataAllocationManager()
    {
        if(this->owners.size() > 0)
            deallocate();
    }
    
 
    // ==================================================
    // getter
    void allocate()
    {
        this->data = new T();
    }
    // getter nullptr
    void deallocate()
    {
        delete this->data;
        this->owners.clear();
        this->data = nullptr;
    }
    // --------------------------------------------------
    
    void addReferencingObject(IRObjectPtr owner)
    {
        if(owner != nullptr)
        {
            int index = this->owners.indexOf(owner);
            if(index == -1) // if the given owner is not yet added
            {
                // if not yet then allocate
                if(this->owners.size() == 0) allocate();
                this->owners.add(owner);
            }
        
            std::cout << "DataAllocationManager : addReferencer count = " << this->owners.size() << std::endl;
            for(auto ref : this->owners)
            {
                std::cout<< ref << std::endl;
            }
        }
    }
    
    // return if any owner holding the object remains
    bool removeReferencingObject(IRObjectPtr owner)
    {
        
        // remove from the list
        int index = this->owners.indexOf(owner);
        std::cout << "removing owner " << owner << " of index " << index << std::endl;

        if(index >= 0) // in case the owner is found
        {
            this->owners.remove(index);
            // if referencedCount becomes 0, then deallocate this object
            if(owners.size() == 0)
            {
                deallocate();
                std::cout << "removeReferencingObject() deleted object\n";
                return false;
            }
        }
        
        std::cout << "DataAllocationManager : remove Referencer count = " << this->owners.size() << std::endl;
        for(auto ref : this->owners)
        {
            std::cout<< ref << std::endl;
        }
        
        return true;
    }
    
    // --------------------------------------------------
    // get pointer of the object
    T* getData()
    {
        return this->data;
    }
    
    // get a number of the owners
    int getOwnerCount() const
    {
        return this->owners.size();
    }
    // get a list of the owners
    Array<IRObjectPtr> getOwnerList() const
    {
        return this->owners;
    }
    
    // ==================================================

    
     
private:
    
    T* data;
    
    Array<IRObjectPtr> owners;
    
};
#endif /* DataAllocationManager_hpp */
