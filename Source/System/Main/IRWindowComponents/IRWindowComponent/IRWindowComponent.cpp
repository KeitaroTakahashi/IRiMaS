//
//  IRWindowComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#include "IRWindowComponent.hpp"

// ==================================================
// FUNDAMENTAL
// ==================================================

IRWindowComponent::IRWindowComponent(String projectName,
                                     Rectangle<int> frameRect) :
projectName(projectName), frameRect(frameRect)
{
    initialize();
    createComponents();
    createNewWorkspace();
    
    
    //initially set object menu on t
    this->leftBar->toObjectMenuAction();
    
    audioSetup();
}
// ----------------------------------------

IRWindowComponent::~IRWindowComponent()
{
    closeAudioSetup();
    
    this->mainSpace.reset();
    this->bar.reset();
    this->leftBar.reset();
    this->rightBar.reset();
    
    this->ir_str.reset();
    
}

// ----------------------------------------

void IRWindowComponent::paint(Graphics& g)
{
    g.fillAll(this->ir_str->SYSTEMCOLOUR.fundamental);
    
    
    
}
// ----------------------------------------

void IRWindowComponent::resized() 
{
    int w = getWidth();
    int h = getHeight();
    
    this->mainComponentHeight = h - this->barHeight;
    
    this->bar->setBounds(0, 0, w, this->barHeight);
    this->mainSpace->setBounds(this->leftBarWidth, this->barHeight,
                               w - this->leftBarWidth, this->mainComponentHeight);
    
    
    int lw = (this->leftBar->getWidth() >= this->leftBarWidth) ? this->leftBar->getWidth() : this->leftBarWidth;
    this->leftBar->setBounds(0, this->barHeight,
                             lw, 
                             h - this->barHeight);
    this->leftBar->workspaceSelectedCallback = [this](IRWorkspace* space) { workspaceSelected(space); };
    
    int rw = this->rightBar->getWidth();
    this->rightBar->setBounds(getWidth() - rw, this->barHeight,
                              rw, getHeight() - this->barHeight);
    this->rightBar->setInitialPos(Point<int>(getWidth(), this->barHeight));
    
    
    
}
// ----------------------------------------

void IRWindowComponent::initialize()
{
    
    //keyListener setup
    //setWantsKeyboardFocus(true);
    //addKeyListener(this);
    
    // create IRStr
    this->ir_str.reset(new IRStr());
    this->ir_str->projectOwner = this;
    this->ir_str->setKeyListener(this);
    this->ir_str->setMouseListener(this);
    this->ir_str->projectName = this->projectName;
    this->ir_str->SYSTEMCOLOUR = IR::darkBlueSet();
    this->ir_str->ICONBANK = IRIconBank();
    
    //lookandfeelcolour
    
    updateAppearance();
    
}
// ----------------------------------------
// should be called after rightBar created
void IRWindowComponent::createBarComponent()
{
    if(this->bar.get() != nullptr)
    {
        if(this->bar->isVisible())
        {
            removeChildComponent(this->bar.get());
        }
    }
    
    
    this->bar.reset(new IRTitleBar(this->ir_str.get(), "Untitled"));
    this->bar->addChangeListener(this);
    addAndMakeVisible(this->bar.get());
    // title bar and right bar (link)
    this->bar->comp.rightBarButtonCallback = [this](bool flag){
        this->rightBar->openSpaceAction(flag);
    };

    // connect to the title component callbacks
    this->bar->comp.closeButtonCallback = [this] { closeButtonClicked(); };
    this->bar->comp.editModeButtonCallback = [this] { editModeButtonClicked(); };
    this->bar->comp.newSlideButtonCallback = [this] { newSlideButtonClicked(); };
    this->bar->comp.newProjectButtonCallback = [this] { newProjectButtonClicked(); };
    this->bar->comp.saveButtonCallback = [this] { saveButtonClicked(); };
    this->bar->comp.saveasButtonCallback = [this] { saveasButtonClicked(); };
    this->bar->comp.openButtonCallback = [this] { openButtonClicked(); };
    
    //currently this is not active
    this->bar->titleDoubleClickedCallback = [this] { titleDoubleClicked(); };
    

}

void IRWindowComponent::createLeftComponent()
{
    if(this->leftBar.get() != nullptr)
    {
        if(this->leftBar->isVisible())
        {
            removeChildComponent(this->leftBar.get());
        }
    }
    
    this->leftBar.reset(new IRLeftBar(this->ir_str.get()));
    this->leftBar->addChangeListener(this);
    addAndMakeVisible(this->leftBar.get());
    this->rightBar->addMouseListener(this, true);

}

void IRWindowComponent::createComponents()
{
    this->rightBar.reset(new IRRightBar(this->ir_str.get()));

    this->mainSpace.reset(new IRMainSpace(this->ir_str.get()));
    this->mixer.addAudioSource(&this->mainSpace->getMixer());
    
    
    this->mainSpace->addListener(this);
    this->mainSpace->addListener(this->rightBar.get());
    
    // main space is back
    addAndMakeVisible(this->mainSpace.get());
    addAndMakeVisible(this->rightBar.get());

    
    //this should be called after rightBar created
    createBarComponent();
    createLeftComponent();
        
}
// ----------------------------------------

void IRWindowComponent::changeListenerCallback (ChangeBroadcaster* source)
{
    if(source == this->bar.get())
    {
        if(this->bar->getStatus() == IRWindowBarActionStatus::MoveWindow)
        {
            if(this->windowMoveAction != nullptr)
                this->windowMoveAction(this->bar->pos);
        }
    }
    else if(source == this->leftBar.get())
    {
        if(this->leftBar->getStatus() == IRWindowBarActionStatus::MoveWindow)
        {
            if(this->windowMoveAction != nullptr)
            {
                this->windowMoveAction(this->leftBar->pos);
            }
        }
    }
}
// ----------------------------------------

// ==================================================
// KeyEvent
// ==================================================

bool IRWindowComponent::keyPressed(const KeyPress& key, Component* originatingComponent)
{
    std::cout << "IRWindowComponent keyPressed() : " << key.getKeyCode() << " : " << key.getTextDescription() << ", " << key.getTextCharacter() <<   std::endl;
    
   
    
    if(key.getTextDescription() == "command + E")
    {
        
        
        // we need to get TopWorkspace first and get its EditMode status
       // bool EditModeOfTopWorkspace = getTopWorkspace()->isEditMode();
        // and then,
        setEditMode(!this->isEditMode());
        
        /*
        // notify it to IRProject
        if(this->notifyEditModeChanged != nullptr)
        {
            this->notifyEditModeChanged();
        }
        */
        return true;
    }
    
    // in case no key
    /*
    if(! this->isEditMode())
    {
        std::cout << "IRWworkspace : The workspace can not receive any key event because it is in Control Mode, if you want to control Object interface, please click an object you want to control and give a KeyEventFocus on it." << std::endl;
    }
    else {
        std::cout << "IRWorkspace : Unknown KeyEvent received." << std::endl;
    }
    */
    return false;
}
// ----------------------------------------

void IRWindowComponent::modifierKeysChanged(const ModifierKeys &mod)
{
    
}

// ----------------------------------------

// ==================================================
// MOUSE EVENT
// ==================================================

void IRWindowComponent::mouseDrag(const MouseEvent& e)
{
    auto pos = e.getEventRelativeTo(this).getPosition();
    
    if(this->isResizable)
    {
        Point<int> delta = pos - this->prevPos;
        
        int newW = this->prevSize.getX() + delta.getX();
        int newH = this->prevSize.getY() + delta.getY();
        if(newW < this->minWidth) newW = this->minWidth;
        if(newH < this->minHeight) newH = this->minHeight;
        
        setSize(newW, newH);
    }
    
}
// ----------------------------------------

void IRWindowComponent::mouseUp(const MouseEvent& e)
{
    if(this->isResizable)
    {
        this->isResizable = false;
        if(this->mainSpace->getTopWorkspace() != nullptr)
            this->mainSpace->getTopWorkspace()->setResizing(true);
    }
}
// ----------------------------------------

void IRWindowComponent::mouseDown(const MouseEvent& e)
{
    auto pos = e.getEventRelativeTo(this).getPosition();
    this->prevPos = pos;
    
    //std::cout << "IRWindowComponent : " << pos.getX() << ", " << pos.getY() << std::endl;
    //std::cout << "resizable area x " << getWidth() - this->resizableMargin << " : " << getHeight() - this->resizableMargin << std::endl;
 
    // store current window size
    this->prevSize = Point<int> (getWidth(), getHeight());
    
    if(pos.getX() > getWidth() - this->resizableMargin &&
       pos.getY() > getHeight() - this->resizableMargin)
    {
        this->isResizable = true;
        if(this->mainSpace->getTopWorkspace() != nullptr)
            this->mainSpace->getTopWorkspace()->setResizing(true);
    }else
    {
        this->isResizable = false;
        if(this->mainSpace->getTopWorkspace() != nullptr)
            this->mainSpace->getTopWorkspace()->setResizing(false);
    }
    
    //close menu if it is opened
    this->leftBar->closeMenu();
}
// ----------------------------------------

void IRWindowComponent::mouseMove(const MouseEvent& e)
{
    
}
// ----------------------------------------

// ==================================================
// CHILD COMPONENTS
// ==================================================

void IRWindowComponent::setComponentsHeight(int barHeight, int mainHeight)
{
    this->barHeight = barHeight;
    this->mainComponentHeight = mainHeight;
}
// ----------------------------------------


// ==================================================
// WORKSPACE
// ==================================================

void IRWindowComponent::createNewWorkspace()
{
    
    this->mainSpace->createNewWorkspace();
}
// ----------------------------------------

// ----------------------------------------
// ----------------------------------------
// ----------------------------------------
// ----------------------------------------

// ==================================================
// WORKSPACE
// ==================================================

void IRWindowComponent::setEditMode(bool flag)
{
    this->isEditModeFlag = flag;
    
    this->bar->comp.setEditMode(flag);
    
    for(auto space : this->mainSpace->getWorkspaces())
    {
        space->setEditMode(flag);
    }
}

// ==================================================
// PROJECT
// ==================================================
void IRWindowComponent::openProject()
{
   
}
void IRWindowComponent::closeProject(DocumentWindow* closingWindow)
{

}

void IRWindowComponent::createNewProject()
{
    
}
void IRWindowComponent::loadProjectFromSavedData(std::string path)
{
    
}
// ==================================================
//IRMainSpace Listener
void IRWindowComponent::nodeObjectSelectionChange(IRNodeObject* obj)
{
    

}

void IRWindowComponent::nodeObjectGetFocused(IRNodeObject* obj)
{
    
}

void IRWindowComponent::newWorkspaceCreated(IRWorkspace* space)
{
    
    std::cout << "IRWindowComponent::newWorkspaceCreated\n";
    this->leftBar->addNewWorkspaceSlide(space);
    
}

void IRWindowComponent::workspaceEditModeChanged(IRWorkspace* changedSpace)
{
    std::cout << "IRWindowComponent::workspaceEditModeChanged\n";
    this->isEditModeFlag = changedSpace->isEditMode();
        
    for(auto space : this->mainSpace->getWorkspaces())
    {
        if(space != changedSpace)
            space->setEditMode(this->isEditModeFlag);
    }
}

void IRWindowComponent::heavyObjectCreated(IRNodeObject* obj)
{
    rebindOpenGLContents();
}


// ==================================================



// ==================================================
// SYSTEM
// ==================================================
// Audio
void IRWindowComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    this->mixer.getAudioSource().prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void IRWindowComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    this->mixer.getAudioSource().getNextAudioBlock(bufferToFill);
}

void IRWindowComponent::releaseResources()
{
    this->mixer.getAudioSource().releaseResources();
}

// AudioAppComponent
void IRWindowComponent::audioSetup()
{
    setAudioChannels(0, 2);
}


void IRWindowComponent::closeAudioSetup()
{
    shutdownAudio();
}

// project
void IRWindowComponent::closeButtonClicked()
{
    // if not yet saved after any changes
    if(!this->ir_str->isProjectSaved)
    {
        
    }else // saved!
    {
        closeProject();
    }
}

void IRWindowComponent::closeProject()
{
    if(this->closeProjectCallback != nullptr)
        this->closeProjectCallback();
}

void IRWindowComponent::editModeButtonClicked()
{
    bool em = this->mainSpace->getTopWorkspace()->isEditMode();
    this->mainSpace->getTopWorkspace()->setEditMode(!em);
    this->bar->comp.setEditMode(!em);
}

void IRWindowComponent::titleDoubleClicked()
{
    /*
    Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
    
    if(r.getWidth() == getWidth() && r.getHeight() == getHeight())
    {
        setSize(this->previousSize.getX(), this->previousSize.getY());
    }else
    {
        this->previousSize = Point<int>(getWidth(), getHeight());
        setSize(r.getWidth(), r.getHeight());
        if(this->windowMoveAction != nullptr)
            this->windowMoveAction(Point<int>(0,0));
    }
     */
}

void IRWindowComponent::newSlideButtonClicked()
{
    std::cout << "IRWindowComponent::newSlideButtonClicked\n";
    this->mainSpace->createNewWorkspace();
}
void IRWindowComponent::newProjectButtonClicked()
{
    if(this->newProjectCallback != nullptr)
        this->newProjectCallback();
}
void IRWindowComponent::saveButtonClicked()
{
    
}
void IRWindowComponent::saveasButtonClicked()
{
    
}
void IRWindowComponent::openButtonClicked()
{
    
}
// ==================================================

// called from IRLeftBar
void IRWindowComponent::workspaceSelected(IRWorkspace* space)
{
    this->mainSpace->setTopWorkspace(space);
    heavyObjectCreated(nullptr);
    
}
// ==================================================

void IRWindowComponent::rebindOpenGLContents()
{
    
    if(this->leftBar->getWidth() > 0 && this->leftBar->getHeight() > 0)
           this->leftBar->bringThisToFront();
       if(this->rightBar->getWidth() > 0 && this->rightBar->getHeight() > 0)
           this->rightBar->bringThisToFront();
       if(this->bar->getWidth() > 0 && this->bar->getHeight() > 0)
           this->bar->bringThisToFront();
    
    // make sure to update
    resized();

}

void IRWindowComponent::updateAppearance()
{
    getLookAndFeel().setColour(ComboBox::backgroundColourId, this->ir_str->SYSTEMCOLOUR.fundamental);
    getLookAndFeel().setColour(ComboBox::textColourId, this->ir_str->SYSTEMCOLOUR.text);
    getLookAndFeel().setColour(PopupMenu::backgroundColourId, this->ir_str->SYSTEMCOLOUR.contents);
    getLookAndFeel().setColour(PopupMenu::highlightedBackgroundColourId, this->ir_str->SYSTEMCOLOUR.fundamental);

    getLookAndFeel().setColour(ColourSelector::backgroundColourId, this->ir_str->SYSTEMCOLOUR.fundamental);
    getLookAndFeel().setColour(Slider::backgroundColourId, this->ir_str->SYSTEMCOLOUR.fundamental);
    getLookAndFeel().setColour(Slider::thumbColourId, this->ir_str->SYSTEMCOLOUR.contents.brighter());
    getLookAndFeel().setColour(Slider::trackColourId, this->ir_str->SYSTEMCOLOUR.text);
    
    getLookAndFeel().setColour(TextButton::buttonColourId, this->ir_str->SYSTEMCOLOUR.fundamental);
    getLookAndFeel().setColour(TextButton::buttonOnColourId, this->ir_str->SYSTEMCOLOUR.contents);
    getLookAndFeel().setColour(TextButton::textColourOnId, this->ir_str->SYSTEMCOLOUR.text);
    //getLookAndFeel().setColour(Slider::backgroundColourId, this->ir_str->SYSTEMCOLOUR.fundamental);
    
    getLookAndFeel().setColour(Label::textColourId, this->ir_str->SYSTEMCOLOUR.text);

    /*
    getLookAndFeel().setColour(ComboBox::focusedOutlineColourId, this->ir_str->SYSTEMCOLOUR.contents);
    getLookAndFeel().setColour(ComboBox::arrowColourId, this->ir_str->SYSTEMCOLOUR.text);
    getLookAndFeel().setColour(ComboBox::buttonColourId, this->ir_str->SYSTEMCOLOUR.fundamental);
    getLookAndFeel().setColour(ComboBox::outlineColourId, this->ir_str->SYSTEMCOLOUR.contents);
     */
    
    if(this->leftBar.get() != nullptr)
    {
        this->leftBar->repaint();
    }
    if(this->rightBar.get() != nullptr)
    {
        this->rightBar->repaint();
    }
    if(this->bar.get() != nullptr)
    {
        this->bar->repaint();
    }
    
    repaint();
    
}
