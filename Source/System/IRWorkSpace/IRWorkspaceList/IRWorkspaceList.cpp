
#include "IRWorkspaceList.hpp"




IRWorkspaceList::IRWorkspaceList(Rectangle<int> frameRect)
{
    this->frameRect = frameRect;
    
    addKeyListener(this);
    
    setBounds(frameRect);
}


IRWorkspaceList::~IRWorkspaceList()
{
    std::cout << "workspace list deleted" << std::endl;

    removeAllChildren();
    for(auto snap : this->snapComponents)
    {
        delete snap;
    }
    this->snapComponents.clearQuick();

    this->workspaces.clear();
    
    this->previouslySelectedSnap = nullptr;
    this->currentlySelectedSnap = nullptr;
    
    
    
}


void IRWorkspaceList::resized()
{
    
}


void IRWorkspaceList::paint(Graphics& g)
{
    g.fillAll(SYSTEMCOLOUR.contents.brighter());
}


void IRWorkspaceList::updateAnimationFrame()
{
    updateList();
}


void IRWorkspaceList::updateList()
{
  
    // get index of the selected workspace
    int selectedIndex = -1;
    if (this->currentlySelectedSnap != nullptr)
    {
        selectedIndex = this->currentlySelectedSnap->getIndex() - 1;
    }else
    {
        selectedIndex = 0;
        std::cout << "Error IRWorkspaceList::updateList() : currentlySelectedSnap is NULL\n";
        //return;
    }
    removeAllChildren();
   
    for(auto snap : this->snapComponents)
    {
        delete snap;
    }
    this->snapComponents.clearQuick();
    
    this->previouslySelectedSnap = nullptr;
    this->currentlySelectedSnap = nullptr;
    
    
    int index = 1;
    int doubleX = this->marginX_left + this->marginX_right;
    
    int w = getWidth() - doubleX;
    int h = w * 4 / 6 + this->marginY * 2; // 6:4
    
    std::cout << "workspace size = " << this->workspaces.size() << std::endl;
    
    // -------
  
    for (auto space : this->workspaces)
    {
        
        int ht = (index - 1) * h;
        
        Rectangle<int> rect(0, ht, getWidth(), h);
        
        ShowSnap* s = new ShowSnap(rect, index, space);
        
        s->updateImage();
        s->addChangeListener(this);
        this->snapComponents.add(s);
        addAndMakeVisible(s);
        
        index++;
    }

    if (selectedIndex >= 0)
    {
        this->snapComponents[selectedIndex]->listEventSelectedAction();
    }

    // IMPORTANT!!
    // give back keyboard focus on this object.
    //workspaceListFocused();
    

    repaint();
   
    // ------

}


void IRWorkspaceList::mouseDown(const MouseEvent& e)
{
    std::cout << "WorkspaceList mouse down\n";
    
    workspaceListFocused();
}

    
// this method is called when this object is focused by mouse down or other behaviors.
// it is importnat to give Keyboard focus only on this obejct otherwise the keyboard event will be confused.
void IRWorkspaceList::workspaceListFocused()
{
    setWantsKeyboardFocus(true);
    grabKeyboardFocus();
}


void IRWorkspaceList::changeListenerCallback(ChangeBroadcaster* source)
{
    for (auto snap : this->snapComponents)
    {
        if(source == snap){
            
            switch (snap->getEventStatus())
            {
                case listEventSelected:
                    std::cout << "IRWorkspaceList : listEventSelected ChangeCallback\n";
                    
                    snapSelectionChange(snap);
                    
                    
                    
                    break;
                case listEventDeleted:
                    break;
                default:
                    break;
            }
            
            
        }
    }
}


// return selected workspace component otherwise return nullptr
Component* IRWorkspaceList::getSelectedComponent()
{
    return this->selectedComponent;
}


void IRWorkspaceList::setSelectedComponentIndex(int index)
{
    if(index < this->snapComponents.size())
    {
        snapSelectionChange(this->snapComponents[index]);
    }
    else
    {
        std::cout << "Error : setSelectedComponentIndex() could not set index of " << index << "to snapComponents. Out of range.\n";
    }
}


void IRWorkspaceList::callRemoveWorkspaceAction(IRWorkSpace* workspace)
{

    Component::BailOutChecker checker(this);
    //==========
    // check if the objects are not deleted, if deleted, return
    if (checker.shouldBailOut()) return;
    this->listeners.callChecked(checker, [workspace](Listener& l){ l.removeWorkspace(workspace); });
    //check again
    if (checker.shouldBailOut()) return;
    //std::function
    if (this->removeWorkspaceCompleted != nullptr) this->removeWorkspaceCompleted();
    
}


void IRWorkspaceList::addWorkspace(IRWorkSpace* space)
{
    this->workspaces.add(space);
        
    std::cout << "addWorkspace : " << this->workspaces.size() << std::endl;

    updateList();

    auto snap = this->snapComponents[this->snapComponents.size()-1];
    snapSelectionChange(snap);
}


Array<IRWorkSpace*> IRWorkspaceList::getWorkspaceList()
{
    return this->workspaces;
}


void IRWorkspaceList::removeWorkspace(ShowSnap* snap)
{
    if (snap == nullptr)
        return;
    
    int removeIndex = snap->getIndex() - 1;
    
    // You can not delete the last workspace and the project empty!±
    if (removeIndex > 0)
    {
        callRemoveWorkspaceAction(this->workspaces[removeIndex]);
        this->workspaces.remove(removeIndex);
    }
    
    //delete snap;
    
    updateList();
}


void IRWorkspaceList::snapSelectionChange(ShowSnap* snap)
{
    
    std::cout << "snapSelectionChange() : " << snap << " : "<< this->currentlySelectedSnap << std::endl;
    if (this->currentlySelectedSnap != nullptr)
    {
        this->previouslySelectedSnap = this->currentlySelectedSnap;
        this->previouslySelectedSnap->updateImage();
        this->previouslySelectedSnap->setSelected(false);
    }
    
    this->currentlySelectedSnap = snap;
    this->currentlySelectedSnap->setSelected(true);
    
    this->selectedComponent = snap->getParent();
    
    this->eventStatus = listEventSelected;
    
    sendChangeMessage();
}


IRWorkspaceList::listEventStatus IRWorkspaceList::getEventStatus() const
{
    return this->eventStatus;
}



// **** **** PRIVATE METHODS **** **** //

bool IRWorkspaceList::keyPressed (const KeyPress& key,
                 Component* originatingComponent)
{
    std::cout << "IRWorkspaceList keyPressed : " << key.getKeyCode() << std::endl;
    if (key.getKeyCode() == key.deleteKey || key.getKeyCode() == key.backspaceKey)
    {
        
        IRWorkSpace* space = dynamic_cast<IRWorkSpace*>(this->selectedComponent);
        
        if(space != nullptr){
            
            ShowSnap* removeSnap = this->currentlySelectedSnap;
            int removeIndex = removeSnap->getIndex();
            
            // set new selectedSnap
            if (this->workspaces.size() == 1)
            {
                this->currentlySelectedSnap = nullptr;
            }
            else
            {
                if (removeIndex == 1)
                {
                    // set one higher index
                    this->currentlySelectedSnap = this->snapComponents[removeIndex];
                    std::cout << "currentlySelectedSnap = " << this->currentlySelectedSnap << std::endl;
                }
                else
                {
                    // set one lower index
                    this->currentlySelectedSnap = this->snapComponents[removeIndex-2];
                    std::cout << "currentlySelectedSnap = " << this->currentlySelectedSnap << std::endl;

                }
            }
            
            removeWorkspace(removeSnap);
            
        }
        
        return true;
    }
    // select neighbor slides
    else if (key.getKeyCode() == key.upKey)
    {
        ShowSnap* snap = this->currentlySelectedSnap;
        int snapIndex = snap->getIndex() - 1;
        // if selected snap is not the first one, then move to the forward
        if (snapIndex > 0)
        {
            snapSelectionChange(this->snapComponents[snapIndex-1]);
        }
        return true;
    }
    else if (key.getKeyCode() == key.downKey)
    {
        ShowSnap* snap = this->currentlySelectedSnap;
        int snapIndex = snap->getIndex();

        // if selected snap is not the last one, then move to the backward
        if (snapIndex < this->snapComponents.size()){
            snapSelectionChange(this->snapComponents[snapIndex]);
        }
        return true;
    }
    
    if (key.getTextDescription() == "command + E")
    {
        
    }
    
    return false;
}




