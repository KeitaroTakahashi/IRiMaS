//
//  DataAllocationManager.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 22/02/2019.
//

#ifndef DataAllocationManager_hpp
#define DataAllocationManager_hpp
#include <vector>
#include "DataType.h"

enum IRFileType
{
    JUCEIMAGE,
    JUCEVIDEOCOMPONENT,
    IRAUDIO
};


template<class T>
class DataAllocationManager
{
public:
    
    DataAllocationManager();
    
    ~DataAllocationManager();
    
    // ==================================================
    // getter
    T* allocate();
    // getter nullptr
    T* deallocate();
    // --------------------------------------------------
    
    void addReferencingObject();
    
    void removeReferencingObject();
    
    // --------------------------------------------------

    T* ptr();
    
    // ==================================================
private:
    
    T* data;
    
    // count a number of referencing objects of this pointer
    unsigned int referencedCount;
    
    std::vector<IRObjectPtr> list;
    
    
};
#endif /* DataAllocationManager_hpp */
