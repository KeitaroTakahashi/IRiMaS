
#include "IRWorkSpace.hpp"

IRWorkSpace::IRWorkSpace(String title, Rectangle<int> frameRect, PreferenceWindow* preferenceWindow)
{
    
    
    this->name = title;
    this->title = this->name + " (EDIT MODE)";
    setBounds(frameRect);
    
    
    setOpaque(true);
    
    
    //setBufferedToImage(false);
    loadBackgroundImageLink(); // for LinkMode
    
    //setSize (frameRect.getWidth(), frameRect.getHeight());
    setWantsKeyboardFocus(true);
    addKeyListener(this);
    
    //object menu
    this->objectMenuComponent = new ObjectListMenu(this, Rectangle<int>(300,400));
    this->objectMenuComponent->addListener(this);
    
    // get a pointer of the preference window
    this->preferenceWindow = preferenceWindow;
    
    //give object lists to selector
    this->selector = new IRNodeObjectSelector(&this->objects, &this->linkModeFlag);
    
    this->selector->setShiftConstrainsDirection(true);
    this->selector->setConstrainBoundsToParent(true, {0,0,10,10});
}


IRWorkSpace::~IRWorkSpace()
{
    std::cout << "workspace destructor called" << std::endl;
    delete this->objectMenuComponent;
    delete this->selector;
  
    // juce::Array free the memory space as well when cleared.
    for(auto obj : this->objects)
    {
        delete obj;
    }
    this->objects.clearQuick();
}


void IRWorkSpace::paint (Graphics& g)
{
    std::cout << " ++++++ workspace repained! ++++++ \n";
    g.fillAll(Colours::white);
    
    // draw shadows for the selected objects
    if(isEditMode()) drawShadows(g);
    
    // paint all of
    /*
    for(auto obj : this->objects)
    {
        obj->initialPaintOnWorkspace(g, this);
    }*/
    
    if(isEditMode()) drawGrids(g);
    
    if(isLinkMode())
    {
        float w = getWidth() / 2;
        float h = w;
        float x = w - w/2;
        float y = getHeight()/2 - h/2;
        g.drawImage(this->background_image_link, Rectangle<float>(x,y,w,h));
    }
    
}


void IRWorkSpace::drawShadows(Graphics& g)
{
   // std::cout << "drawing... shadow of " << this->selector->getSelectedObjectList().size() << " items" << std::endl;
    auto list = this->selector->getSelectedObjectList();
    
    for(auto obj : list)
    {
        DropShadow shadow(SYSTEMCOLOUR.contents, 30, Point<int>(0,0));
        
        Rectangle<int> b = obj->getBounds();
        Rectangle<int> bounds(b.getX(), b.getY(), b.getWidth(), b.getHeight());
        
        shadow.drawForRectangle(g, bounds);
        
        // we need to explicitly repaint the area of shadow, otherwise it won't be erased.
        repaint(b.getX()-50, b.getY()-50, b.getWidth()+100, b.getHeight()+100);
    }
}


void IRWorkSpace::drawGrids(Graphics& g)
{
    int w = getWidth();
    int h = getHeight();

    g.setColour(Colours::lightgrey.brighter().brighter());
    int i = 0;
    
    Path p;
    for(i=0;i<=w;i+=this->thick_grids_interval)
    {
        p.startNewSubPath(i, 0);
        p.lineTo(i, h);
        
        p.startNewSubPath(0, i);
        p.lineTo(w, i);
    }
    p.closeSubPath();
    g.strokePath(p, PathStrokeType(this->grid_thickness));

    for(i=0;i<=w;i+=this->thin_grids_pixel)
    {
        p.startNewSubPath(i, 0);
        p.lineTo(i, h);
        
        p.startNewSubPath(0, i);
        p.lineTo(w, i);
    }
    p.closeSubPath();
    g.strokePath(p, PathStrokeType(this->grid_thickness2));
}

void IRWorkSpace::resized()
{
}


void IRWorkSpace::mouseDown(const MouseEvent& e)
{
    std::cout << "IRWorkSpace mouseDown " << this << std::endl;

    this->selector->mouseDownHandler(e);
    
    if(isEditMode())
    {
        addAndMakeVisible(this->selector);
        this->isMultiSelectMode = true;
        closeObjectListMenu();
    }
    
}


void IRWorkSpace::mouseMove(const MouseEvent& e)
{
    this->currentMousePosition = e.getEventRelativeTo(this).getPosition();
    
    //std::cout << currentMousePosition.getX() << " , " << currentMousePosition.getY() << std::endl;
    
}


void IRWorkSpace::mouseUp(const MouseEvent& e)
{
    this->selector->mouseUpHandler(e);
    if (this->isMultiSelectMode)
    {
        removeChildComponent(this->selector);
        this->isMultiSelectMode = false;
    }
    
    if (this->dummy.size() > 0)
    {
        this->dummy.clear();
    }
    //request updating the workspaceList
    if(requestWorkspaceListUpdate != nullptr) requestWorkspaceListUpdate();
}


void IRWorkSpace::mouseDrag(const MouseEvent& e)
{
    this->currentMousePosition = e.getEventRelativeTo(this).getPosition();
    
    //std::cout << "workspace e = " << e.getPosition().x << ", " << e.getPosition().y << std::endl;
    this->selector->mouseDragHandler(e);
    
    if(this->dummy.size() > 0)
    {
        for(auto obj : this->dummy)
        {
            obj->setCentrePosition(this->currentMousePosition.getX(),
                                   this->currentMousePosition.getY());
        }
    }
    
    
}


void IRWorkSpace::mouseDoubleClick(const MouseEvent& e)
{
    this->currentMousePosition = e.getEventRelativeTo(this).getPosition();
    
    if(isEditMode())
    {
        std::cout << "Open menu window\n";
        
        // we need the screen position
        openObjectListMenu(e.getScreenPosition());
    }
}


void IRWorkSpace::modifierKeysChanged(const ModifierKeys &mod)
{
    this->isShiftPressed = false;
    this->isCommandPressed = false;
    this->isControlPressed = false;
    this->isAltPressed = false;
    this->isOptionPressed = false;
    
    if(mod.isShiftDown()) this->isShiftPressed = true;
    if(mod.isCommandDown()) this->isCommandPressed = true;
    if(mod.isCtrlDown()) this->isControlPressed = true;
    if(mod.isAltDown()) this->isAltPressed = true;
    
}


void IRWorkSpace::changeListenerCallback (ChangeBroadcaster* source)
{
    IRNodeObject* obj = dynamic_cast<IRNodeObject* >(source);
    if(obj != nullptr)
    {
        if(obj->isSelected())
        {
            //std::cout << "selected reported!\n";
        }
        
        // if resizing, deselect all objects and setSelect true only the resized object.
        if(obj->isResizing())
        {
            this->selector->deselectAllObjects();
            obj->setSelected(true);
            this->selector->addSelectedObjects();
        }
    }
}


void IRWorkSpace::AudioSetup()
{
    setAudioChannels(0, 2);
}


void IRWorkSpace::closeAudioSetup()
{
    shutdownAudio();
}


void IRWorkSpace::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    this->mixer.getAudioSource().prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void IRWorkSpace::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    this->mixer.getAudioSource().getNextAudioBlock(bufferToFill);
}


void IRWorkSpace::releaseResources()
{
    this->mixer.getAudioSource().releaseResources();
}


AudioSource& IRWorkSpace::getMixer()
{
    return this->mixer.getAudioSource();
}


json11::Json IRWorkSpace::makeSaveDataOfThis()
{
    
    std::cout << "makeSaveDataOfThis" << std::endl;
    std::vector<json11::Json::object> objectList;
    
    int index = 0;
    for(auto item : this->objects)
    {
        json11::Json::object ob = json11::Json::object({
            {"object-" + std::to_string(index), json11::Json::object({
                
                {"objectType",          item->name.toStdString()},
                {"objectUniqueID",      item->getUniqueID().toStdString()},
                {"bounds",              json11::Json::array({item->getX(), item->getY(), item->getWidth(), item->getHeight()})},
                {"status",              "Normal"},
                {"ObjectDefined",       item->saveThisToSaveData()}
                
            })},
        });
        
        objectList.push_back(ob);
        index++;
    }
    
    json11::Json obList = json11::Json { objectList };
    
    json11::Json obj = json11::Json::object({
        {"Appearance", json11::Json::object({
            {"backgroundColour", "white"}
        })},
        {"Objects", obList},
    });
    
    //test
    
    std::string jsonString =  obj.dump();
    std::ofstream myfile2;
    myfile2.open("/Users/keitaro/Desktop/IRiMaS2018.txt");
    myfile2 << jsonString;
    myfile2.close();
    
    
    return obj;
}


bool IRWorkSpace::isEditMode() const
{
    return this->editModeFlag;
}


void IRWorkSpace::setEditMode(bool flag)
{
    this->editModeFlag = flag;
    
    if (flag)
    {
        this->title = this->name + " (EDIT MODE)";
        this->setInterceptsMouseClicks(true, true);
        setWantsKeyboardFocus(true);
    }
    else
    {
        this->title = this->name;
        this->setInterceptsMouseClicks(true, false);
    }
    
    //std::cout << "edit mode changed " << flag << " : " << this->title << std::endl;
    
    for (auto obj : this->objects)
    {
        obj->setEditMode(flag);
    }
    // send change message to IRProject
    sendChangeMessage();
    
    repaint();
}

bool IRWorkSpace::isLinkMode() const
{
    return this->linkModeFlag;
}
void IRWorkSpace::setLinkMode(bool flag)
{
    this->linkModeFlag = flag;
    
    for(auto obj : this->objects)
    {
        obj->setLinkMode(this->linkModeFlag);
    }
    
    if(this->linkModeFlag){
        
        openLinkMenuOfSelectedObject();
    }
    else{
        closeLinkMenu();
    }
    
    repaint();
}

Array<IRNodeObject*> IRWorkSpace::getObjectList()
{
    return this->objects;
}

Image IRWorkSpace::getSnap()
{
    this->snap = createComponentSnapshot(Rectangle<int>(0, 0, this->getWidth(), this->getHeight()),
                                         false, 0.4);
    return this->snap;
}


void IRWorkSpace::addListener(Listener* newListener)
{
    this->listeners.add(newListener);
}


void IRWorkSpace::removeListener(Listener* listener)
{
    this->listeners.remove(listener);
}

// ==================================================
// Link Menu
void IRWorkSpace::openLinkMenuOfSelectedObject()
{
    for (auto obj : this->selector->getActivatedLinkingObjectList())
    {
        obj->openLinkMenu();
    }
}
// ----------------------------------------
void IRWorkSpace::openAllLinkMenu()
{
    for(auto obj : this->objects)
    {
        obj->openLinkMenu();
    }
}
// ----------------------------------------

void IRWorkSpace::closeLinkMenu()
{
    for(auto obj : this->objects)
    {
        obj->closeLinkMenu();
    }
}
// ----------------------------------------

void IRWorkSpace::closeLinkMenu(IRNodeObject* obj)
{
    for (auto obj : this->selector->getActivatedLinkingObjectList())
    {
        obj->closeLinkMenu();
    }
}

void IRWorkSpace::getSelectedLinkSystemFlag(IRNodeObject* obj)
{
    std::cout << obj << " : ws getSelected flag = " <<  obj->selectedLinkSystemFlag << std::endl;

    // if shift is pressed, then link this flag with the previously selected Flag

    if(this->isShiftPressed)
    {
        addLinkingObject(obj);
    }
    else{
        // if no shift pressed, then deselect other flag, and re-select the new one.
        for(auto o : this->selector->getActivatedLinkingObjectList())
        {
            o->getLinkMenu()->deSelectAll();
            removeAllLinkingObjects();
        }
        addLinkingObject(obj);
    }
    
    showLinkingObjects();
  
}

// ==================================================

void IRWorkSpace::loadBackgroundImageLink()
{
    String url = "/materials/Images/icon/link/link.png";
#if JUCE_MAC
    this->background_image_link = loadImage("/Contents/Resources" + url);
#elif JUCE_IOS
    this->background_image_link = loadImage(url);
#endif
    
    std::cout << "background_image_link " << this->background_image_link.getWidth() << std::endl;
}

Image IRWorkSpace::loadImage(String url)
{
    File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + url);
    return ImageFileFormat::loadFrom(file);
}
