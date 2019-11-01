//
//  IRRightBar.hpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#ifndef IRRightBar_hpp
#define IRRightBar_hpp

#include "JuceHeader.h"
#include "IRWindowHeader.h"
#include "IRStrComponent.hpp"
#include "IRImageButton.hpp"
#include "IRMainSpace.hpp"
#include "IROpenGLManager.hpp"

class IRRightBar : public Component,
public IRStrComponent,
public ChangeBroadcaster,
public ChangeListener,
private KeAnimationComponent,
public IRMainSpace::Listener
{
public:
    IRRightBar(IRStr* str);
    ~IRRightBar();
    //==================================================
    void paint (Graphics&) override;
    void resized() override;
    
    //==================================================
    
    void openSpaceAction(bool flag);
    
    void setInitialPos(Point<int> pos) { this->initialPos = pos; }
    
    //==================================================
    void addComponentAndMakeVisible(Component& comp);
    
    // update Z-Order?
    void bringThisToFront() {
        //this->openGLContext.bringViewToFront();
        IROpenGLManager manager(&this->openGLContext);
        manager.bringOpenGLContextFront(this);
    }
    
private:
    //==================================================
     //OpenGL
     OpenGLContext openGLContext;
     bool isOpenGLComponentClosed = false;
     
    
    //==================================================

    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    void updateAnimationFrame() override;
    
    // ==================================================
    //IRWorkspace
    // getting to know when the selection status of a NodeObject has been changed.
    // This function is called when an object is either selected or unselected.
    // this function is, for instance, used to update the Object Controller in the IRRightBar
    // IRWorkspace and this class is connected in IRWindowComponent
    void nodeObjectSelectionChange(IRNodeObject* obj) override;
    void nodeObjectGetFocused(IRNodeObject* obj) override;

    //==================================================
    // layout
    int buttonSize = 40;
    int space = 250;
    
    Point<int> initialPos;
     
    //==================================================
    bool openSpace = false;
    bool isOpened = false;
    int openSpeed = 24;
    //==================================================
    // actual component
    Component* comp = nullptr;
    
    //==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRRightBar)

};

#endif /* IRRightBar_hpp */
