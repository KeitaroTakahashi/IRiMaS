
#include "IRWorkSpace.hpp"




IRWorkSpace::IRWorkSpace(String title, Rectangle<int> frameRect, PreferenceWindow* preferenceWindow)
{
    this->name = title;
    this->title = this->name + " (EDIT MODE)";
    setBounds(frameRect);
    //setSize (frameRect.getWidth(), frameRect.getHeight());
    setWantsKeyboardFocus(true);
    addKeyListener(this);
    
    //object menu
    this->ObjectMenuComponent = new ObjectListMenu(this, Rectangle<int>(300,400));
    this->ObjectMenuComponent->addListener(this);
    
    // get a pointer of the preference window
    this->preferenceWindow = preferenceWindow;
    
    //give object lists to selector
    this->selector = new IRNodeObjectSelector(&this->objects);
    
    this->selector->setShiftConstrainsDirection(true);
    this->selector->setConstrainBoundsToParent(true, {0,0,10,10});
}


IRWorkSpace::~IRWorkSpace()
{
    std::cout << "workspace destructor called" << std::endl;
    delete this->ObjectMenuComponent;
    delete this->selector;
}


void IRWorkSpace::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    //g.fillAll(Colour(60, 7, 36));
    g.fillAll(SYSTEMCOLOUR.background);
    
    // draw shadows for the selected objects
    //drawShadows(g);
    
}


void IRWorkSpace::drawShadows(Graphics& g)
{
    
    printf("drawing... shadow of %d items\n",this->selector->getSelectedObjectList().size());
    auto list = this->selector->getSelectedObjectList();
    
    for(auto obj : list)
    {
        DropShadow shadow(SYSTEMCOLOUR.contents, obj->getBounds().getWidth(), Point<int>(0,0));
        
        Rectangle<int> b = obj->getBounds();
        Rectangle<int> bounds(b.getX() + 20, b.getY() + 30, b.getWidth(), b.getHeight());
        
        shadow.drawForRectangle(g,
                                bounds);
    }
}


void IRWorkSpace::resized()
{
    
}


void IRWorkSpace::mouseDown(const MouseEvent& e)
{
    std::cout << "IRWorkSpace mouseDown\n";
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
        for (auto obj : this->dummy)
        {
            IRNodeObject* o = obj->copySelectedContents();
            Rectangle<int> bounds = o->getBounds();
            //std::cout << "workspace xy = " << this->currentMousePosition.getX() << ", " << this->currentMousePosition.getY() << std::endl;
            
            o->setBounds(this->currentMousePosition.getX(),
                         this->currentMousePosition.getY(),
                         bounds.getWidth(),
                         bounds.getHeight());
            
            createObject(o);
            o->setSelected(true);
            this->selector->addSelectedObjects();
            o->repaint();
        }
        this->dummy.clear();
    }
    
}


void IRWorkSpace::mouseDrag(const MouseEvent& e)
{
    this->currentMousePosition = e.getEventRelativeTo(this).getPosition();
    
    //std::cout << "workspace e = " << e.getPosition().x << ", " << e.getPosition().y << std::endl;
    this->selector->mouseDragHandler(e);
    
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
    
    printf("makeSaveDataOfThis\n");
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



