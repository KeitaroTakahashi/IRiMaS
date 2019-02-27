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
    
    void addReferencingObject(IRObjectPtr owner);
    
    void removeReferencingObject(IRObjectPtr owner);
    
    // --------------------------------------------------
    // get pointer of the object
    T* ptr();
    
    // get a number of the owners
    int getOwnerCount() const;
    // get a list of the owners
    Array<IRObjectPtr> getOwnerList() const;
    
    // ==================================================
private:
    
    T* data;
    
    Array<IRObjectPtr> owners;
    
    
};
#endif /* DataAllocationManager_hpp */
