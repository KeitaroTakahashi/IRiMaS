//
//  IRProjectOwnerBase.h
//  IRiMaS
//
//  Created by Keitaro on 24/10/2019.
//

#ifndef IRProjectOwnerBase_h
#define IRProjectOwnerBase_h

class IRProjectOwnerBase : public AudioAppComponent
{
    
public:
    IRProjectOwnerBase()
    {
        
    }
    
    ~IRProjectOwnerBase() {}
    
    
    virtual void rebindOpenGLContents() {}
    
    virtual void updateAppearance() {}
};

#endif /* IRProjectOwnerBase_h */
