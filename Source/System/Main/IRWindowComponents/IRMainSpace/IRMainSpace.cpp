//
//  IRMainSpace.cpp
//  IRiMaS
//
//  Created by Keitaro on 27/08/2019.
//

#include "IRMainSpace.hpp"

IRMainSpace::IRMainSpace(IRStr* str)
{
    this->ir_str = str;
}

IRMainSpace::~IRMainSpace()
{
    for(auto space : this->workspaces )
    {
        delete space;
    }
}
//==================================================

void IRMainSpace::paint(Graphics& g)
{
}

void IRMainSpace::resized()
{
    
    for(auto space : this->workspaces)
    {
        space->setBounds(getLocalBounds());
    }
}
//==================================================

void IRMainSpace::changeListenerCallback (ChangeBroadcaster* source)
{
    
    
}

//==================================================

void IRMainSpace::updateAnimationFrame()
{
    
}

// ==================================================
// WORKSPACE
// ==================================================

void IRMainSpace::createNewWorkspace()
{
    std::cout << "create IRWorkspace\n";
    String title = this->ir_str->projectName + "_" + String(this->workspaces.size() + 1);    
    IRWorkspace* space = new IRWorkspace(title, this->ir_str, nullptr);
    space->addListener(this);
    this->workspaces.push_back(space);
    this->mixer.addAudioSource(&space->getMixer());
    
    addAndMakeVisible(space);
    
    if(this->topWorkspace != nullptr)
    {
        // hide heavy components first
        this->topWorkspace->manageHeavyWeightComponents(false);
    }
    // update a top workspace
    this->topWorkspace = space;
    // store a pointer of the current top workspace
    this->ir_str->TopWorkspace = this->topWorkspace;

    space->toFront(true);
    
    // call listener's function
    callnewWorkspaceCreated(space);
    
    resized();

}

void IRMainSpace::setTopWorkspace(IRWorkspace* topSpace)
{
    
    // search if workspaces has the received IRWorkspace.
    auto it = std::find(this->workspaces.begin(), this->workspaces.end(), topSpace);
    if(it != this->workspaces.end())
    {
        if(this->topWorkspace != nullptr)
        {
            // hide heavy components first
            this->topWorkspace->manageHeavyWeightComponents(false);
        }
        this->topWorkspace = topSpace;
        this->topWorkspace->toFront(true);
        //show heavy components if they are hidden
        this->topWorkspace->manageHeavyWeightComponents(true);
        this->ir_str->TopWorkspace = this->topWorkspace;
        
    }else{
        std::cout << "ERROR : IRMainSpace setTopWorkspace : Could not find received topSpace of " << topSpace << std::endl;
    }
}


void IRMainSpace::nodeObjectSelectionChange(IRNodeObject* obj)
{
    
    callNodeObjectSelectionChange(obj);
  
}

void IRMainSpace::nodeObjectGetFocused(IRNodeObject* obj)
{
    callNodeObjectGetFocused(obj);
}

void IRMainSpace::editModeChanged(IRWorkspace* changedSpace)
{
    callWorkspaceEditModeChanged(changedSpace);
}

void IRMainSpace::heavyObjectCreated(IRNodeObject* obj)
{
    callHeavyObjectCreated(obj);
}


// ==================================================
// MOUSE EVENT
// ==================================================
void IRMainSpace::mouseDown(const MouseEvent& e)
{
    std::cout << "MainSpace mouseDown\n";
}

// ==================================================
// SYSTEM
// ==================================================

// Audio
void IRMainSpace::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    this->mixer.getAudioSource().prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void IRMainSpace::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    this->mixer.getAudioSource().getNextAudioBlock(bufferToFill);
}


void IRMainSpace::releaseResources()
{
    this->mixer.getAudioSource().releaseResources();
}
// ==================================================
//Listener

void IRMainSpace::callNodeObjectSelectionChange(IRNodeObject* obj)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [obj](Listener& l){ l.nodeObjectSelectionChange(obj); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRMainSpace::callNodeObjectGetFocused(IRNodeObject* obj)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [obj](Listener& l){ l.nodeObjectGetFocused(obj); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRMainSpace::callnewWorkspaceCreated(IRWorkspace* space)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [space](Listener& l){ l.newWorkspaceCreated(space); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRMainSpace::callWorkspaceEditModeChanged(IRWorkspace* space)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [space](Listener& l){ l.workspaceEditModeChanged(space); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRMainSpace::callHeavyObjectCreated(IRNodeObject* obj)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [obj](Listener& l){ l.heavyObjectCreated(obj); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRMainSpace::addListener(Listener* newListener)
{
    this->listeners.add(newListener);
}


void IRMainSpace::removeListener(Listener* listener)
{
    this->listeners.remove(listener);
}

// ==================================================
// ==================================================
