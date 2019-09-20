//
//  IRMainSpace.hpp
//  IRiMaS
//
//  Created by Keitaro on 27/08/2019.
//

#ifndef IRMainSpace_hpp
#define IRMainSpace_hpp

#include "JuceHeader.h"
#include "IRWindowHeader.h"
#include "IRIconBank.hpp"
#include "IRImageButton.hpp"
#include "IRWorkspace2.hpp"
#include "KeAnimationComponent.h"

class IRMainSpace : public Component,
public ChangeBroadcaster,
public ChangeListener,
private KeAnimationComponent
{
public:
    //==================================================
    IRMainSpace(IRStr* str);
    ~IRMainSpace();
    
    void paint(Graphics& g) override;
    void resized() override;
    
    //==================================================
    // IRWorkspace
    void createNewWorkspace();
    
    std::vector<IRWorkspace* > getWorkspaces() const { return this->workspaces; }
    IRWorkspace* getTopWorkspace() const { return this->topWorkspace; }
    //==================================================
    void mouseDown(const MouseEvent& e) override;
    
    //==================================================
    //==================================================
    //==================================================

    
private:
    // ==================================================

    IRStr* ir_str;

    //==================================================
    void changeListenerCallback (ChangeBroadcaster* source) override;
    void updateAnimationFrame() override;

    //==================================================
    // Workspace
    std::vector<IRWorkspace*> workspaces;
    IRWorkspace* topWorkspace = nullptr;
    //==================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRMainSpace)


};


#endif /* IRMainSpace_hpp */
