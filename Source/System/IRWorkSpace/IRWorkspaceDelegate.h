//
//  IRWorkspaceDelegate.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 30/08/2018.
//

#ifndef IRWorkspaceDelegate_h
#define IRWorkspaceDelegate_h

#include "JuceHeader.h"

class IRWorkspaceDelegate : public ChangeBroadcaster,
                            public ChangeListener
{
    
};

class IRWorkspaceBroadcaster : public ChangeBroadcaster
{
public:
    ~IRWorkspaceBroadcaster() {}
    
    
};


#endif /* IRWorkspaceDelegate_h */
