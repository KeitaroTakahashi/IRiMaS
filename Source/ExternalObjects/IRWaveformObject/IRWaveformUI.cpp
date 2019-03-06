
#include "IRWaveformUI.hpp"




IRWaveformUI::IRWaveformUI() : IRWaveform()
{
    this->selector = new IRGraphSelector();
    // only horizontal axis is movable and height follows the object height.
    this->selector->setSelectionMode(true, false);
    this->selector->setParentSize(getWidth(), getHeight());
    
    //addKeyListener(this);
}


IRWaveformUI::~IRWaveformUI()
{
    
}


void IRWaveformUI::resized()
{
    IRWaveform::resized();
    this->selector->setParentSize(getWidth(), getHeight());
    
    //double wRatio = (double)getWidth() / (double)this->previousWidth;
    
    for(auto obj : this->selectionSquareObjects)
    {
        obj->renewBounds();
    }
    this->previousWidth = getWidth();
    this->previousHeight = getHeight();
}


bool IRWaveformUI::isSelectMode() const
{
    return this->selectModeFlag;
}


void IRWaveformUI::setEditMode(bool flag)
{
    this->editModeFlag = flag;
    
    // change edit mode for
    for (auto obj : this->selectionSquareObjects)
    {
        obj->setEditMode(flag);
    }
    
    // if not editMode, get keyboardFocus
    if (! flag)
    {
        setWantsKeyboardFocus(true);
        addKeyListener(this);
        
    }
    else
    { // otherwise out of keyboard Focus and initialize dragdropCalled flag
        setWantsKeyboardFocus(false);
        removeKeyListener(this);
        this->dragdropCalled = false;
    }
}


bool IRWaveformUI::isEditMode() const
{
    return this->editModeFlag;
}


void IRWaveformUI::setLooping(bool flag)
{
    this->looping = flag;
}


bool IRWaveformUI::isLooping() const
{
    return this->looping;
}


void IRWaveformUI::setPlayOffset(int offset)
{
    this->playOffset = offset;
}


int IRWaveformUI::getPlayOffset() const
{
    return this->playOffset;
}


void IRWaveformUI::createSquareObject(Rectangle<int> rect)
{
    if(rect.getWidth() > 1)
    {
        IRMultiPurposeObject* obj = new IRMultiPurposeObject(this);
        //obj->addListener(this->parentListener);
        obj->setEditMode(isEditMode());
        obj->setBoundsRatio(true);
        obj->setMovable(false,false,false);
        addAndMakeVisible(obj);
        
        float x = (float)rect.getX() / (float)getWidth();
        float w = (float)rect.getWidth() / (float)getWidth();
        if(x < 0){ // when mouse drag goes to the outside of the parent component, x becomes below 0.
            w += x; // fix
            x = 0.0;
        }
        
        float y = 0.0;
        if((w+x) > 1.0) w = 1.0 - x;
        
        float h = 1.0;
        
        obj->setBoundsInRatio(x, y, w, h);
        obj->renewBounds();
        this->selectionSquareObjects.add(obj);
    }
}


void IRWaveformUI::addSquareObject(IRMultiPurposeObject* obj)
{
    obj->setEditMode(isEditMode());
    obj->setBoundsRatio(true);
    obj->setMovable(false,false,false);
    addAndMakeVisible(obj);
    this->selectionSquareObjects.add(obj);
}


void IRWaveformUI::deleteSquareObject()
{
    std::cout << "delete\n";
    for(auto obj : this->selectedSquareObjectList)
    {
        
        int index = this->selectionSquareObjects.indexOf(obj);
        if(index >= 0) this->selectionSquareObjects.remove(index);
        removeChildComponent(obj);
    }
    this->selectedSquareObjectList.clear();
    
    // stop playing
    stopPlaying();
    
}


void IRWaveformUI::deselectAllSquareObject()
{
    std::cout << "deselected all square object\n";
    for(auto obj : this->selectionSquareObjects)
    {
        if(obj->isSelected()) obj->setSelected(false);
        obj->repaint();
    }
    this->selectedSquareObjectList.clear();
    
}


void IRWaveformUI::addSelectedObjects()
{
    //std::cout << "addSelectedObjects() :: " << std::endl;
    
    this->selectedSquareObjectList.clear();
    for(auto obj : this->selectionSquareObjects)
    {
        //std::cout << obj << " : selected? = " << obj->isSelected() << std::endl;
        if(obj->isSelected())
        {
            this->selectedSquareObjectList.add(obj);
        }
    }
}


void IRWaveformUI::setListener(IRNodeObject::Listener* newListener)
{
    this->parentListener = newListener;
    
    // replace listener
    for (auto obj : this->selectedSquareObjectList)
    {
        obj->getListenerList().clear();
        obj->addListener(newListener);
    }
}


bool IRWaveformUI::keyPressed(const KeyPress& key, Component* originatingComponent)
{
    std::cout << "IRWaveformUI keyPressed : " << key.getKeyCode() << std::endl;
    if(key.getKeyCode() == key.deleteKey || key.getKeyCode() == key.backspaceKey)
    {
        if(! isEditMode())
            deleteSquareObject();
    }
    
    if (key.getTextDescription() == "command + E")
    {
        this->status = EDITMODECHANGE;
        sendChangeMessage();
        
    }
    else if (key.getTextDescription() == "command + S")
    {
        this->status = PROJECTSAVE;
        sendChangeMessage();
    }
    return true;
}




