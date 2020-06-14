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
    this->referenceImage = getReferenceOfIndex(1);
    
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
    if(this->workspaces.size() == 0)
    {
        g.drawImage(this->referenceImage, getLocalBounds().toFloat());
    }
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
    Rectangle<int> r (-1000, -1000, 1000, 1000);
    
    IRWorkspace* space = new IRWorkspace(title, r, this->ir_str);
    space->addListener(this);
    // register key and mouse listeners if necessary
    space->registerKeyListener(this->ir_str->key);
    space->registerMouseListener(this->ir_str->mouse);
    addKeyListener(space);
    
    this->workspaces.push_back(space);
    this->mixer.addAudioSource(&space->getMixer());
    
    addAndMakeVisible(space);

    // update a top workspace
    this->topWorkspace = space;
    // store a pointer of the current top workspace
    this->ir_str->TopWorkspace = this->topWorkspace;
    // --------------------------------------------------
    // resize before bring OopenGLContext to front
    resized();
    // --------------------------------------------------
    space->toFront(true);
    space->bringThisToFront("IRWorkspace");
    space->manageHeavyWeightComponents(true);
    // --------------------------------------------------

    // call listener's function
    callnewWorkspaceCreated(space);
    
}

void IRMainSpace::deleteWorkspace(IRWorkspace* space)
{
    callWorkspaceWillDeleted(space);
    
    this->mixer.removeSource(&space->getMixer());
    removeChildComponent(space);
    
    this->topWorkspace = nullptr;
    auto it = std::find(this->workspaces.begin(), this->workspaces.end(), space);
    if(it != this->workspaces.end())
    {
        this->workspaces.erase(it);
        delete space;
    }
    
    callWorkspaceHasDeleted();
}

void IRMainSpace::setTopWorkspace(IRWorkspace* topSpace)
{
    
    // search if workspaces has the received IRWorkspace.
    auto it = std::find(this->workspaces.begin(), this->workspaces.end(), topSpace);
    if(it != this->workspaces.end())
    {
        this->topWorkspace = topSpace;
        this->topWorkspace->toFront(true);
        this->topWorkspace->bringThisToFront();
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
    
    std::cout << "IRMainSpace::nodeObjectGetFocused\n";
    callNodeObjectGetFocused(obj);
}

void IRMainSpace::nothingSelected()
{
    callNothingSelected();
}

void IRMainSpace::editModeChanged(IRWorkspaceComponent* changedSpace)
{
    auto space = static_cast<IRWorkspace*> (changedSpace);
    callWorkspaceEditModeChanged(space);
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

void IRMainSpace::callWorkspaceWillDeleted(IRWorkspace* space)
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [space](Listener& l){ l.workspaceWillDeleted(space); });
    //check again
    if(checker.shouldBailOut()) return;
}

void IRMainSpace::callWorkspaceHasDeleted()
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [](Listener& l){ l.workspaceHasDeleted(); });
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


void IRMainSpace::callNothingSelected()
{
    Component::BailOutChecker checker(this);
    // check if the objects are not deleted, if deleted, return
    if(checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [this](Listener& l){ l.nothingSelected(); });
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

Image IRMainSpace::loadImage(String url)
{
    File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + url);
    return ImageFileFormat::loadFrom(file);
}

Image IRMainSpace::getReferenceOfIndex(int index)
{
#if JUCE_MAC
    String url = "/Contents/Resources/materials/Images/Reference/TiAALS_Reference" + String(index) + ".png";
    return loadImage(url);
#elif JUCE_IOS
    String url = "/materials/Images/Reference/TiAALS_Reference" + String(index) + ".png";
    return loadImage(url);
#endif
    
    
}

// ==================================================
