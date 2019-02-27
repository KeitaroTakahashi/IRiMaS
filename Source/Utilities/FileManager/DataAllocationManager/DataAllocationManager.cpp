//
//  DataAllocationManager.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 22/02/2019.
//

#include "DataAllocationManager.hpp"

template<class T>
DataAllocationManager<T>::DataAllocationManager()
{
    this->owners.clear();
}
// --------------------------------------------------
template<class T>
DataAllocationManager<T>::~DataAllocationManager()
{
    if(this->owners.size() > 0)
    {
        this->owners.clear();
        delete this->data;
    }
}
// --------------------------------------------------
template<class T>
T* DataAllocationManager<T>::allocate()
{
    this->data = new T();
}
// --------------------------------------------------
template<class T>
T* DataAllocationManager<T>::deallocate()
{
    delete this->data;
    this->owners.clear();
}
// --------------------------------------------------
template<class T>
T* DataAllocationManager<T>::ptr()
{
    return this->data;
}
// --------------------------------------------------
template<class T>
void DataAllocationManager<T>::addReferencingObject(IRObjectPtr owner)
{
    
    int index = this->owners.indexOf(owner);
    if(index == -1) // if the given owner is not yet added
    {
        this->owners.add(owner);
        
        // if reference count is 0 then allocate new object
        if(this->referencedCount == 0)
            allocate();
    }
}

// --------------------------------------------------
template<class T>
void DataAllocationManager<T>::removeReferencingObject(IRObjectPtr owner)
{
    
    if(this->referencedCount > 0)
    {
        // remove from the list
        int index = this->owners.indexOf(owner);
        if(index >= 0) // in case the owner is found
        {
            this->owners.remove(index);
            
            // if referencedCount becomes 0, then deallocate this object
            if(this->referencedCount == 0)
                deallocate();
            
        }
    }
    
}

// --------------------------------------------------
template<class T>
int DataAllocationManager<T>::getOwnerCount() const
{
    return this->owners.size();
}
// --------------------------------------------------

template<class T>
Array<IRObjectPtr> DataAllocationManager<T>::getOwnerList() const
{
    return this->owners;
}
