
#include "IRNodeComponent.hpp"

IRNodeComponent::IRNodeComponent(Component* parent, String name, NodeObjectType objectType) :
resizingArea(25, 25)
{
    this->objectType = objectType;
    //default size
    setSize(100, 100);
    this->name = name;
    this->mixer = new MixerAudioSource();
    
    this->parent = parent;
    
    this->menu.addItem(999, name); // do nothing
    this->menu.addSeparator();
    this->menu.addItem(1, "Bring to Front");
    this->menu.addItem(2, "Send to Back");
    this->menu.addSeparator();
    this->menu.addItem(3, "Cut");
    this->menu.addItem(4, "Copy");
    this->menu.addItem(5, "Paste");
    this->menu.addItem(6, "Duplicate");
    
}


IRNodeComponent::~IRNodeComponent()
{
    delete this->mixer;
    
    std::cout << "~IRNODECOMPONENT DESTRUCTOR CALLED" << std::endl;
    
}


// basics
void IRNodeComponent::resized()
{
    //this->constrainer.setMinimumOnscreenAmounts(getHeight(), getWidth(),
      //                                          getHeight(), getWidth());
}


void IRNodeComponent::setSize(float width, float height)
{
    statusChangedWrapper(IRNodeComponentStatus::WillResizeStatus);

    
    float w = width > this->maxWidth ? this->maxWidth : width;
    float h = height > this->maxHeight ? this->maxHeight : height;
    
    
    w = w >= this->minWidth ? w : this->minWidth;
    h = h >= this->minHeight ? h : this->minHeight;
    
    float x = getX();
    float y = getY();
    
    setBounds(x, y, w, h);
    
    statusChangedWrapper(IRNodeComponentStatus::HasResizedStatus);
}


void IRNodeComponent::setEnableParameters(IRNodeComponentSetUp id...)
{
    
}


void IRNodeComponent::setDisableParameters(IRNodeComponentSetUp id...)
{
    
}


int IRNodeComponent::getPreviousWidth() const
{
    return this->previousWidth;
}


int IRNodeComponent::getPreviousHeight() const
{
    return this->previousHeight;
}


void IRNodeComponent::setPreferenceWindow(PreferenceWindow* preferenceWindow)
{
    this->preferenceWindow = preferenceWindow;
}


PreferenceWindow* IRNodeComponent::getPreferenceWindow()
{
    return this->preferenceWindow;
}

void IRNodeComponent::updateFileManager(IRFileManager* fileManager)
{
    setIRFileManager(fileManager);
    if(this->fileManagerUpdated != nullptr) this->fileManagerUpdated(fileManager);
}

// paint
void IRNodeComponent::paint(Graphics& g)
{
    auto area = getLocalBounds();//.reduced (0);

    if (isEditMode())
    {
        g.setColour (SYSTEMCOLOUR.contents);
        g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
    }
    
    //g.setColour (this->isSelected()? Colours::white : SYSTEMCOLOUR.titleText);
    //g.drawFittedText ("NodeObject", area, Justification::centred, 1);
    
}



// Child Component Management
void IRNodeComponent::childComponentManager(Component* comp)
{
    comp->addMouseListener(this, true);
    if (isEditMode())
    {
        comp->setInterceptsMouseClicks(false, false);
    }
    else
    {
        comp->setInterceptsMouseClicks(true, false);
    }
}


// Audio source
void IRNodeComponent::addAudioComponent(AudioSource *source)
{
    this->mixer->addInputSource(source, false);
    this->containAudioSourceFlag = true;
}


void IRNodeComponent::removeAudioComponent(AudioSource *source) // FD ADDON
{
    this->mixer->removeInputSource(source); // which will be deleted automatically
    this->containAudioSourceFlag = false;

}


MixerAudioSource* IRNodeComponent::getAudioSource() const
{
    return this->mixer;
}


// check if any AudioSources are added in this NodeObject.
bool IRNodeComponent::isContainAudioSource() const
{
    return this->containAudioSourceFlag;
}


// interaction
void IRNodeComponent::mouseDown(const MouseEvent& e)
{
    std::cout << "IRNodeComponent mouseDown\n";
    mouseDownNodeEvent(e);
    mouseDownEvent(e);
}


void IRNodeComponent::mouseMove(const MouseEvent& e)
{
    mouseUpNodeEvent(e);
    mouseMoveEvent(e);
}


void IRNodeComponent::mouseUp(const MouseEvent& e)
{
    mouseUpNodeEvent(e);
    mouseUpEvent(e);
    mouseUpCompleted(e);
}

void IRNodeComponent::mouseDoubleClick(const MouseEvent& e)
{
    mouseDoubleClickNodeEvent(e);
    mouseDoubleClickEvent(e);
}


void IRNodeComponent::mouseDrag(const MouseEvent& e)
{
    mouseDragNodeEvent(e); // defines fundamental behavior
    mouseDragEvent(e); // defined by an unique Node object
}


// resizing area
Point<float> IRNodeComponent::getResizingArea() const
{
    return this->resizingArea;
}


void IRNodeComponent::setResizingArea(Point<float> area)
{
    this->resizingArea = area;
}


// min max setter
void IRNodeComponent::setMinimumWidth(const float newMin)
{
    this->minWidth = newMin;
}


void IRNodeComponent::setMinimumHeight(const float newMin)
{
    this->minHeight = newMin;
}


void IRNodeComponent::setMaximumWidth(const float newMax)
{
    this->maxWidth = newMax;
}


void IRNodeComponent::setMaximumHeight(const float newMax)
{
    this->maxHeight = newMax;
}


// EVENTS FOR OVERRIDE
void IRNodeComponent::mouseDownEvent(const MouseEvent& e)
{
    
}


void IRNodeComponent::mouseMoveEvent(const MouseEvent& e)
{
    
}


void IRNodeComponent::mouseDoubleClickEvent(const MouseEvent& e)
{
    
}


void IRNodeComponent::mouseUpEvent(const MouseEvent& e)
{
    
}


void IRNodeComponent::mouseDragEvent(const MouseEvent& e)
{
    
}


// CALLBACK FUNCTIONS
void IRNodeComponent::statusChangedCallback(IRNodeComponentStatus status)
{
   
}


// Change status

bool IRNodeComponent::isMovable() const
{
    return this->isMovableFlag;
}


bool IRNodeComponent::isVerticalMovable() const
{
    return this->isVerticalMovableFlag;
}


bool IRNodeComponent::isHorizontalMovable() const
{
    return this->isHorizontalMovableFlag;
}


// all setter
void IRNodeComponent::setMovable(bool movable, bool verticalMovable, bool horizontalMovable)
{
    this->isMovableFlag = movable;
    this->isVerticalMovableFlag = verticalMovable;
    this->isHorizontalMovableFlag = horizontalMovable;
}


bool IRNodeComponent::isMoving() const
{
    return this->movingFlag;
}


bool IRNodeComponent::isDragging() const
{
    return this->draggingFlag;
}


// return a flag shows whether this Component is resizable or not. Default is true.
bool IRNodeComponent::isResizable() const
{
    return this->isResizableFlag;
}


// return a flag shows whether this Component is being resized or not. Default is false.
bool IRNodeComponent::isResizing() const
{
    return this->resizingFlag;
}


// return a flag shows whether this Component is being selected or not. Default is false.
bool IRNodeComponent::isSelected() const
{
    return this->selectedFlag;
}


// return a flag shows whether the belonging workspace is on edit mode or not.
bool IRNodeComponent::isEditMode() const
{
    return this->editModeFlag;
}


// if edit mode is true, this object does not receive any Mouse/Keyboard events
// if false, this object receive Mouse/Keyboard events
void IRNodeComponent::setEditMode(bool flag)
{
    this->editModeFlag = flag;
    statusChangedWrapper(IRNodeComponentStatus::EditModeStatus);
    editModeChangedEvent();
    repaint();
}

bool IRNodeComponent::isCopied() const
{
    return this->copiedFlag;
}


bool IRNodeComponent::isCopiable() const
{
    return this->isCopiableFlag;
}

bool IRNodeComponent::isMouseListenerFlag() const
{
    return this->mouseListenerFlag;
}


void IRNodeComponent::setMouseListenerFlag(bool flag)
{
    this->mouseListenerFlag = flag;
}

//unique ID
void IRNodeComponent::setUniqueID(String id)
{
    this->uniqueID = id;
}


void IRNodeComponent::setUniqueID(std::string id)
{
    this->uniqueID = String(id);
}


String IRNodeComponent::getUniqueID() const
{
    return this->uniqueID;
}


// object type
NodeObjectType IRNodeComponent::getObjectType() const
{
    return this->objectType;
}


// user defined popup menu events
void IRNodeComponent::popupMenuEvents()
{
    
}

// **** BELOW ALREDY DONE BY K

void IRNodeComponent::statusChangedWrapper(IRNodeComponentStatus status)
{
    switch (status)
    {
        case EditModeStatus:
            // cancel selection
            setSelected(false);
            if(isEditMode())
            {
                if(! this->mouseListenerFlag)
                {
                    addMouseListener(this->parent, true);
                    this->mouseListenerFlag = true;
                }
                
                for(auto comp : getChildren())
                {
                    comp->setInterceptsMouseClicks(false, false);
                }
            }else{
                
                if(this->mouseListenerFlag)
                {
                    removeMouseListener(this->parent);
                    this->mouseListenerFlag = false;
                }
                
                for(auto comp : getChildren())
                {
                    comp->setInterceptsMouseClicks(true, true);
                }
            }
            
            break;
        default:
            break;
    }
    
    // callback function :: used in IRUIFoundation to inform any of IRNodeObject's status change
    if(this->statusChangeCompleted != nullptr)
        statusChangeCompleted(status);
    
    // call statusChangeCallback
    statusChangedCallback(status);
}


void param_register(std::string id, int data)
{
    t_json save = t_json::object({
        {id, data}
    });
}
