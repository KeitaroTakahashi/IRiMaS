
#include "IRNodeComponent.hpp"

IRNodeComponent::IRNodeComponent(Component* parent,
                                 String name,
                                 IRStr* str,
                                 NodeObjectType objectType) :
IRStrComponent(str),
IRHeavyWeightComponent(this, name),
resizingSquare(this, parent),
resizingArea(25, 25)
{
    StopWatch bench; bench.start();
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
    bench.result("x x x x x x x xx x : IRNodeComponent");
    
    
    //only resizingSquare and not squares at the corners
    parent->addAndMakeVisible(this->resizingSquare);
    this->resizingSquare.showSquare(false);
    
    this->resizingSquare.resizingSquareClicked = [this](IRResizeSquare2::MovableDirection direction){resizingSquareClicked(direction);};
    
    this->resizingSquare.resizingSquareReleased = [this](IRResizeSquare2::MovableDirection direction){resizingSquareReleased(direction);};

    
    this->resizingSquare.resizingSquareDragged = [this](MouseEvent e){resizingSquareDragged(e);};


    //this->resizingSquare.addListener(this);

}


IRNodeComponent::~IRNodeComponent()
{

    delete this->mixer;
    
    if(this->objectType.componentType == ordinaryIRComponent)
    {
        //this->openGLContext.detach();
        std::cout << "openGL despatched!\n";

    }

    std::cout << "~IRNODECOMPONENT DESTRUCTOR CALLED" << std::endl;
    
}
/*
void IRNodeComponent::initOpenGLContext()
{
    //if this object contains heavy weight component, then connect to OpenGLContext
       if(this->objectType.componentType == ordinaryIRComponent)
       {
           this->openGLContext.setRenderer(this);
           this->openGLContext.setContinuousRepainting(false);
           
           this->openGLContext.attachTo(*this);
           IROpenGLManager manager(&this->openGLContext, this->name);
           manager.setOpenGLContextAlpha(0);
    
       }
}*/


// basics
void IRNodeComponent::resized()
{
    //this->constrainer.setMinimumOnscreenAmounts(getHeight(), getWidth(),
      //                                          getHeight(), getWidth());
}

// ==================================================

void IRNodeComponent::setSize(float width, float height)
{
    statusChangedWrapper(IRNodeComponentStatus::WillResizeStatus);

    
    float w = width > this->maxWidth ? this->maxWidth : width;
    float h = height > this->maxHeight ? this->maxHeight : height;
    
    
    w = w >= this->minWidth ? w : this->minWidth;
    h = h >= this->minHeight ? h : this->minHeight;
    
    float x = getX();
    float y = getY();
    
    setObjectBounds(x, y, w, h);
    
    updateResizingSquare();
    
    statusChangedWrapper(IRNodeComponentStatus::HasResizedStatus);
}

void IRNodeComponent::setObjectCentredPosition(int x, int y)
{
    std::cout << "IRNodeComponent::setObjectCentredPosition\n";
    setCentrePosition(x, y);
    
    updateResizingSquare();
    
    if(this->objectType.componentMode == IRNodeComponentMode::WORKSPACE)
    {
        ObjectPositionChanged(x - getWidth()/2, y - getHeight()/2);
    }
    
    // update initial bounds
    this->setInitialBounds(getBounds().toFloat());
    
}

void IRNodeComponent::setObjectBounds(Rectangle<int> bounds)
{
    //std::cout << "IRNodeComponent::setObjectBounds\n";
    setBounds(bounds);
    
    updateResizingSquare();
    
    if(this->objectType.componentMode == IRNodeComponentMode::WORKSPACE)
    {
        ObjectPositionChanged(bounds.getX(), bounds.getY());
        Rectangle<int> b (bounds.getX(), bounds.getY(),
                          bounds.getWidth(), bounds.getHeight());
        ObjectBoundsChanged(b);
        ObjectBoundsChanged4IRNodeObject(b);
    }
    
    // update initial bounds
    this->setInitialBounds(getBounds().toFloat());
}

void IRNodeComponent::setObjectBounds(int x, int y, int w, int h)
{
    Rectangle<int> bounds(x,y,w,h);
    setObjectBounds(bounds);
}

void IRNodeComponent::setObjectBoundsRelative(Rectangle<float> ratioBounds)
{
    setBoundsRelative(ratioBounds);
}
void IRNodeComponent::setObjectBoundsRelative(float x, float y, float w, float h)
{
    setBoundsRelative(x, y, w, h);
}

void IRNodeComponent::setBounds(int x, int y, int w, int h)
{
    Component::setBounds(x, y, w, h);
}

void IRNodeComponent::setBounds(Rectangle<int>bounds)
{
    Component::setBounds(bounds);
}

void IRNodeComponent::setObjectSize(int w, int h)
{
    Rectangle<int> bounds(getX(),getY(),w,h);
    setObjectBounds(bounds);
}

void IRNodeComponent::setDraggableArea(Rectangle<int> area)
{
    this->draggableArea = area;
}


// ==================================================

void IRNodeComponent::updateResizingSquare()
{
    //std::cout << "updateResizingSquare\n";
    int s = this->resizingSquare.getSquareSize() / 2;
    this->resizingSquare.setBounds(getBounds().expanded(s));
}

void IRNodeComponent::setResizingSquareColour(Colour colour)
{
    this->resizingSquare.setSquareColour(colour);
}

void IRNodeComponent::setEnableResizingSquare(bool flag)
{
    this->enableResizingSquare = flag;
    
    if(!flag)
    {
        if(this->resizingSquare.isVisible())
        {
            this->resizingSquare.showSquare(false);
        }
    }
    
}

// ==================================================

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


void IRNodeComponent::updateFileManager(IRFileManager& fileManager)
{
    FILEMANAGER = &fileManager;
    //setIRFileManager(fileManager);
    if(this->fileManagerUpdated != nullptr) this->fileManagerUpdated(&fileManager);
}

// paint
void IRNodeComponent::paint(Graphics& g)
{
    g.fillAll(Colours::transparentBlack);

    auto area = getLocalBounds();//.reduced (0);

    if (isEditMode())
    {
        g.setColour (getStr()->SYSTEMCOLOUR.contents);
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
    //std::cout << "IRNodeComponent mouseDown\n";
    mouseDownNodeEvent(e);
    mouseDownEvent(e);
}


void IRNodeComponent::mouseMove(const MouseEvent& e)
{
    //mouseUpNodeEvent(e);
    mouseMoveNodeEvent(e);
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
juce::Point<float> IRNodeComponent::getResizingArea() const
{
    return this->resizingArea;
}


void IRNodeComponent::setResizingArea(juce::Point<float> area)
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

void IRNodeComponent::setResizable(bool flag, bool isWidthResizableFlag, bool isHeightResizableFlag)
{
    this->isResizableFlag = flag;
    this->isWidthResizableFlag = isWidthResizableFlag;
    this->isHeightResizableFlag = isHeightResizableFlag;
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

void IRNodeComponent::setResizingStart(bool flag)
{
    if(flag)
    {
        this->isMovableFlag = false;
        this->resizingFlag = true;
        // store the current size
        this->previousWidth = getWidth();
        this->previousHeight = getHeight();
        this->previousX = getX();
        this->previousY = getY();
        sendChangeMessage();
    }else{
        recoverEventStatus();
    }
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
    
    // disable resizingSquare in Control mode
    if(!flag) this->resizingSquare.showSquare(flag);

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

void IRNodeComponent::setActive(bool active)
{
    
    std::cout << "IRNodeComponent::setActive = " << active << std::endl;
    this->isActiveFlag = active;
    
    //heavyComponentCreatedFunc();
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


// =============================================


void param_register(std::string id, int data)
{
    t_json save = t_json::object({
        {id, data}
    });
}

// =============================================

void IRNodeComponent::bringToFront(bool isRefreshHeavyComponent, bool registerZIndex)
{
    //IRHeavyWeightComponent
    bringThisToFront();
    
    if(isSelected()) this->resizingSquare.bringThisToFront();
    
    // inform IRWorkspaceComponent 
    moveToFrontEvent(isRefreshHeavyComponent, registerZIndex);
}
void IRNodeComponent::bringToBack()
{
    
    std::cout << "IRNodeComponent::bringToBack\n";
    //toBack(); // dont need it!
    
    moveToBackEvent();
    // after all others are to front
    if(isSelected()) this->resizingSquare.toBack();

}


void IRNodeComponent::heavyComponentRefreshed()
{
    //IROpenGLManager manager(&this->openGLContext, this->name);
   // manager.setOpenGLContextAlpha(0);

}

// =============================================
