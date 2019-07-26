
#include "IRWaveformObjectUI.hpp"




IRWaveformObjectUI::IRWaveformObjectUI(IRNodeObject* parent) : IRWaveform(parent)
{
    this->selector = new IRGraphSelector();
    // only horizontal axis is movable and height follows the object height.
    this->selector->setSelectionMode(true, false);
    this->selector->setParentSize(getWidth(), getHeight());
    
    // controller
    
    this->controller.setZoomInEvent([this]{zoomInClicked();});
    this->controller.setZoomOutEvent([this]{zoomOutClicked();});
    this->controller.setCommentEvent([this]{commentClicked();});

    //addKeyListener(this);
}


IRWaveformObjectUI::~IRWaveformObjectUI()
{
    delete this->selector;
    
    this->selectionSquareObjects.clear();
}


void IRWaveformObjectUI::resized()
{
    IRWaveform::resized();
    
    if(getHeight() < 50) setBounds(getX(), getY(), getWidth(), 50);

    this->selector->setParentSize(getWidth(), getHeight());
    
    //double wRatio = (double)getWidth() / (double)this->previousWidth;
    
    for(auto obj : this->selectionSquareObjects)
    {
        obj->renewBounds();
    }
    this->previousWidth = getWidth();
    this->previousHeight = getHeight();
    
    
    int s = getHeight();
    if(s > 50) s = 50;
    int y = getHeight() / 2 - s / 2;
    
    this->controller.setBounds(this->visibleArea.getX(), y, getWidth(), s);

    
}


bool IRWaveformObjectUI::isSelectMode() const
{
    return this->selectModeFlag;
}


void IRWaveformObjectUI::setEditMode(bool flag)
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


bool IRWaveformObjectUI::isEditMode() const
{
    return this->editModeFlag;
}


void IRWaveformObjectUI::setLooping(bool flag)
{
    this->looping = flag;
}


bool IRWaveformObjectUI::isLooping() const
{
    return this->looping;
}


void IRWaveformObjectUI::setPlayOffset(int offset)
{
    this->playOffset = offset;
}


int IRWaveformObjectUI::getPlayOffset() const
{
    return this->playOffset;
}


void IRWaveformObjectUI::createSquareObject(Rectangle<int> rect)
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


void IRWaveformObjectUI::addSquareObject(IRMultiPurposeObject* obj)
{
    obj->setEditMode(isEditMode());
    obj->setBoundsRatio(true);
    obj->setMovable(false,false,false);
    addAndMakeVisible(obj);
    this->selectionSquareObjects.add(obj);
}


void IRWaveformObjectUI::deleteSquareObject()
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


void IRWaveformObjectUI::deselectAllSquareObject()
{
    std::cout << "deselected all square object\n";
    for(auto obj : this->selectionSquareObjects)
    {
        if(obj->isSelected()) obj->setSelected(false);
        obj->repaint();
    }
    this->selectedSquareObjectList.clear();
    
}


void IRWaveformObjectUI::addSelectedObjects()
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


void IRWaveformObjectUI::setListener(IRNodeObject::Listener* newListener)
{
    this->parentListener = newListener;
    
    // replace listener
    for (auto obj : this->selectedSquareObjectList)
    {
        obj->getListenerList().clear();
        obj->addListener(newListener);
    }
}


void IRWaveformObjectUI::IRKeyPressed(int keyCode, String keyText)
{
    KeyPress key;
    
    std::cout << "IRWaveformObjectUI keyPressed : " << keyCode << std::endl;
    if(keyCode == key.deleteKey || keyCode == key.backspaceKey)
    {
        if(! isEditMode())
            deleteSquareObject();
    }
}



void IRWaveformObjectUI::audioPtrDelivery(IRAudio *obj)
{
    std::cout << "IRWaveformObjectUI : audioPtrDelivery filename = " << obj->getFile().getFullPathName() << std::endl;
    //makeThumbnail(obj->getFile().getFullPathName());
    
    // init
    stopPlaying();
    deleteSquareObject();
    deinitializeAudioData();
    
    

    openFile(obj->getFile().getFullPathName());
}
// ==================================================
// controller
void IRWaveformObjectUI::zoomInClicked()
{
    // go to IRWaveformObjectUI2
    if(this->zoomInClickedCallback != nullptr) this->zoomInClickedCallback();
}
void IRWaveformObjectUI::zoomOutClicked()
{
    if(this->zoomOutClickedCallback != nullptr) this->zoomOutClickedCallback();
}
void IRWaveformObjectUI::commentClicked()
{
    //show comment
}

// ==================================================
void IRWaveformObjectUI::setVisibleArea(Rectangle<int> area)
{
    this->visibleArea = area;
    
    int s = getHeight();
    if(s > 50) s = 50;
    int y = getHeight() / 2 - s / 2;
    this->visibleArea = area;
    this->controller.setBounds(this->visibleArea.getX(), y, getWidth(), s);
    
    
    setVisiblePos(area.getPosition());
    linkViewPosition(nodeObject);
    this->previousOffsetX = this->visibleArea.getX();
    
    
    std::cout << "waveform w, h " << getWidth() << std::endl;
    
    std::cout << "current pos " << area.getX() << std::endl;
    
}

// ==================================================
