//
//  DataAllocationManager.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 22/02/2019.
//

#include "DataAllocationManager.hpp"

template<class T>
DataAllocationManager<T>::DataAllocationManager():
referencedCount(0)
{
    
}
// --------------------------------------------------
template<class T>
DataAllocationManager<T>::~DataAllocationManager()
{
    
}
// --------------------------------------------------
template<class T>
T* DataAllocationManager<T>::allocate()
{
    
}
// --------------------------------------------------
template<class T>
T* DataAllocationManager<T>::deallocate()
{
    
}
// --------------------------------------------------
template<class T>
T* DataAllocationManager<T>::ptr()
{
    
}
// --------------------------------------------------
template<class T>
void DataAllocationManager<T>::addReferencingObject()
{
    
    // if reference count is 0 then allocate new object
    if(this->referencedCount == 0)
        allocate();

    // add referencing object count
    this->referencedCount ++;
    
}

// --------------------------------------------------
template<class T>
void DataAllocationManager<T>::removeReferencingObject()
{
    
    if(this->referencedCount > 0)
    {
        this->referencedCount --;
        // if referencedCount becomes 0, then deallocate this object
        deallocate();
    }
    
}

// --------------------------------------------------
