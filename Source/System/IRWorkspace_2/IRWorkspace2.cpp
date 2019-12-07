//
//  IRWorkspace.cpp
//  IRiMaS
//
//  Created by Keitaro on 18/09/2019.
//

#include "IRWorkspace2.hpp"

IRWorkspace::IRWorkspace(String title, IRStr* str, PreferenceWindow* preferenceWindow) :
IRStrComponent(str), message(20, 30, this, str),
IRHeavyWeightComponent(this)
{
    
    this->name = title;
    this->title = this->name + " (EDIT MODE)";
    
    this->ir_str = str;
    setOpaque(true);
    
    //setBufferedToImage(false);
    //loadBackgroundImageLink(); // for LinkMode
    
    setWantsKeyboardFocus(true);
    addKeyListener(this);
    if(this->ir_str != nullptr)
    {
        addKeyListener(this->ir_str->key);
        addMouseListener(this->ir_str->mouse, false);
    }
    
    //object menu
    this->objectMenuComponent = new ObjectListMenu(this, Rectangle<int>(300,400));
    this->objectMenuComponent->addListener(this);
    
    // get a pointer of the preference window
    if(this->preferenceWindow != nullptr)
        this->preferenceWindow = preferenceWindow;
    
    //give object lists to selector
    this->selector = new IRNodeObjectSelector(&this->objects, &this->linkModeFlag);
    
    this->selector->setShiftConstrainsDirection(true);
    this->selector->setConstrainBoundsToParent(true, {0,0,10,10});
    
    openGLInit();
    
    
}


IRWorkspace::~IRWorkspace()
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


void IRWorkspace::paint (Graphics& g)
{
    shaderTask(g);
    //g.fillAll(Colours::white);
    
    // draw shadows for the selected objects
    //if(isEditMode()) drawShadows(g);
    
    // paint all of
    /*
     for(auto obj : this->objects)
     {
     obj->initialPaintOnWorkspace(g, this);
     }*/
    
    //if(isEditMode()) drawGrids(g);
    
    /*
    if(isLinkMode())
    {
        float w = getWidth() / 2;
        float h = w;
        float x = w - w/2;
        float y = getHeight()/2 - h/2;
        g.drawImage(this->background_image_link, Rectangle<float>(x,y,w,h));
    }*/
    
    std::cout << " ++++++ workspace repained! ++++++ " << this->bench.stop() << std::endl;

}

void IRWorkspace::drawShadows(Graphics& g)
{
    // std::cout << "drawing... shadow of " << this->selector->getSelectedObjectList().size() << " items" << std::endl;
    auto list = this->selector->getSelectedObjectList();
    
    for(auto obj : list)
    {
        DropShadow shadow(getStr()->SYSTEMCOLOUR.contents, 30, Point<int>(0,0));
        
        Rectangle<int> b = obj->getBounds();
        Rectangle<int> bounds(b.getX(), b.getY(), b.getWidth(), b.getHeight());
        
        shadow.drawForRectangle(g, bounds);
        
        // we need to explicitly repaint the area of shadow, otherwise it won't be erased.
        repaint(b.getX()-50, b.getY()-50, b.getWidth()+100, b.getHeight()+100);
    }
}


void IRWorkspace::drawGrids(Graphics& g)
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
    }
    
    for(i=0;i<=h;i+=this->thick_grids_interval)
    {
        p.startNewSubPath(0, i);
        p.lineTo(w, i);
    }
    p.closeSubPath();
    g.strokePath(p, PathStrokeType(this->grid_thickness));
    
    for(i=0;i<=w;i+=this->thin_grids_pixel)
    {
        p.startNewSubPath(i, 0);
        p.lineTo(i, h);
    }
    
    for(i=0;i<=h;i+=this->thin_grids_pixel)
       {
           p.startNewSubPath(0, i);
           p.lineTo(w, i);
       }
    p.closeSubPath();
    g.strokePath(p, PathStrokeType(this->grid_thickness2));
}

void IRWorkspace::resized()
{
    
    this->message.setCentrePosition(getWidth()/2,
                                    getHeight()/2);

}


void IRWorkspace::mouseDown(const MouseEvent& e)
{
    //std::cout << "IRWorkspace mouseDown " << e.getPosition().getX() << ", " << e.getPosition().getY() << std::endl;
    
    this->selector->mouseDownHandler(e);
    
    if(isEditMode())
    {
        addAndMakeVisible(this->selector);
        this->isMultiSelectMode = true;
        closeObjectListMenu();
    }
    
}


void IRWorkspace::mouseMove(const MouseEvent& e)
{
    
    this->currentMousePosition = e.getEventRelativeTo(this).getPosition();
    
    //std::cout << currentMousePosition.getX() << " , " << currentMousePosition.getY() << std::endl;
    
}


void IRWorkspace::mouseUp(const MouseEvent& e)
{
    
    //std::cout << "IRWorkspace mouseUp " << e.getPosition().getX() << ", " << e.getPosition().getY() << std::endl;
    this->selector->mouseUpHandler(e);

    if(isEditMode())
    {
    
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
}


void IRWorkspace::mouseDrag(const MouseEvent& e)
{
    if(isEditMode())
    {
        
        this->currentMousePosition = e.getEventRelativeTo(this).getPosition();
        
        if(!this->isResizingFlag){
            this->selector->mouseDragHandler(e);
        }
        
        if(this->dummy.size() > 0)
        {
            for(auto obj : this->dummy)
            {
                obj->setCentrePosition(this->currentMousePosition.getX(),
                                       this->currentMousePosition.getY());
            }
        }
    }
}


void IRWorkspace::mouseDoubleClick(const MouseEvent& e)
{
    this->currentMousePosition = e.getEventRelativeTo(this).getPosition();
    
    if(isEditMode())
    {
        std::cout << "Open menu window\n";
        
        // we need the screen position
        //openObjectListMenu(e.getScreenPosition());
    }
}


void IRWorkspace::modifierKeysChanged(const ModifierKeys &mod)
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


void IRWorkspace::changeListenerCallback (ChangeBroadcaster* source)
{
    std::cout << "changeListenerCallback : " << source << std::endl;
    IRNodeObject* obj = dynamic_cast<IRNodeObject* >(source);

    if(obj != nullptr)
    {

        if(obj->isSelected())
        {
            
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


void IRWorkspace::AudioSetup()
{
    setAudioChannels(0, 2);
}


void IRWorkspace::closeAudioSetup()
{
    shutdownAudio();
}


void IRWorkspace::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    this->mixer.getAudioSource().prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void IRWorkspace::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    this->mixer.getAudioSource().getNextAudioBlock(bufferToFill);
}


void IRWorkspace::releaseResources()
{
    this->mixer.getAudioSource().releaseResources();
}


AudioSource& IRWorkspace::getMixer()
{
    return this->mixer.getAudioSource();
}


json11::Json IRWorkspace::makeSaveDataOfThis()
{
    
    std::cout << "makeSaveDataOfThis" << std::endl;
    std::vector<json11::Json::object> objectList;
    
    // to save objects, we need to reverse the order of ObjectZorder
    // The top object is stored the begining of the vector but it needs to be at the end in order to be created at last.
    std::vector<IRNodeObject*> reversedZorder = this->ObjectZorder;
    std::reverse(std::begin(reversedZorder), std::end(reversedZorder));

    int index = 0;
    for(auto item : reversedZorder)
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

    
    return obj;
}


bool IRWorkspace::isEditMode() const
{
    return this->editModeFlag;
}


void IRWorkspace::setEditMode(bool flag, bool notification)
{
    this->editModeFlag = flag;
    
    
    // reset all selected objects
    this->selector->deselectAllObjects();
    
    
    
    /*
    if (flag)
    {
        
        this->message.setText("EDIT MODE");
        this->message.toFront(true);
        this->message.run();
        this->message.addKeyListener(this);
        
        this->title = this->name + " (EDIT MODE)";
        this->setInterceptsMouseClicks(true, true);
        setWantsKeyboardFocus(true);
        
    }
    else
    {
        
        this->message.setText("CONTROL MODE");
        this->message.toFront(true);
        this->message.run();
        this->message.addKeyListener(this);

        setWantsKeyboardFocus(true);

        this->title = this->name;
        this->setInterceptsMouseClicks(true, false);

        
    }*/
    
    //std::cout << "edit mode changed " << flag << " : " << this->title << std::endl;
    
   
    for (auto obj : this->objects)
    {
        obj->setEditMode(flag);
    }
  
    // notify to IRMainSpace
    if(notification) callEditModeChanged();
    
    
    // send change message to IRProject
    sendChangeMessage();
    
    repaint();
}
/*
bool IRWorkspace::isLinkMode() const
{
    return this->linkModeFlag;
}
void IRWorkspace::setLinkMode(bool flag)
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
}*/

Array<IRNodeObject*> IRWorkspace::getObjectList()
{
    return this->objects;
}

Image IRWorkspace::getSnap()
{
    this->snap = createComponentSnapshot(Rectangle<int>(0, 0, this->getWidth(), this->getHeight()),
                                         false, 0.4);
    return this->snap;
}


void IRWorkspace::addListener(Listener* newListener)
{
    this->listeners.add(newListener);
}


void IRWorkspace::removeListener(Listener* listener)
{
    this->listeners.remove(listener);
}

// ==================================================
// Link Menu
/*
void IRWorkspace::openLinkMenuOfSelectedObject()
{
    for (auto obj : this->selector->getActivatedLinkingObjectList())
    {
        obj->openLinkMenu();
    }
}
// ----------------------------------------
void IRWorkspace::openAllLinkMenu()
{
    for(auto obj : this->objects)
    {
        obj->openLinkMenu();
    }
}
// ----------------------------------------

void IRWorkspace::closeLinkMenu()
{
    for(auto obj : this->objects)
    {
        obj->closeLinkMenu();
    }
}
// ----------------------------------------

void IRWorkspace::closeLinkMenu(IRNodeObject* obj)
{
    for (auto obj : this->selector->getActivatedLinkingObjectList())
    {
        obj->closeLinkMenu();
    }
}*/

void IRWorkspace::getSelectedLinkSystemFlag(IRNodeObject* obj)
{
    //std::cout << obj << " : ws getSelected flag = " <<  obj->selectedLinkSystemFlag << std::endl;
    
    // if shift is pressed, then link this flag with the previously selected Flag
    
    /*
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
    */
}

// ==================================================

void IRWorkspace::loadBackgroundImageLink()
{
    String url = "/materials/Images/icon/link/link.png";
#if JUCE_MAC
    this->background_image_link = loadImage("/Contents/Resources" + url);
#elif JUCE_IOS
    this->background_image_link = loadImage(url);
#endif
    
}

Image IRWorkspace::loadImage(String url)
{
    File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + url);
    return ImageFileFormat::loadFrom(file);
}

// ==================================================
// OpenGL

void IRWorkspace::openGLInit()
{
    
    if (auto* peer = getPeer())
            peer->setCurrentRenderingEngine (0);
                
        String url = File::getSpecialLocation(File::currentApplicationFile).getFullPathName();
    
    #if JUCE_MAC
        url += "/Contents/Resources/materials/Sources/GLSL/grid/KGrid.frag";
    #elif JUCE_IOS
        url += "/materials/Sources/GLSL/grid/KGrid.frag";
    #endif
        
        File f(url);
        if(!f.exists())
        {
            std::cout << "Error : " << url << " does not exist!!\n";
            AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon, "Fatal Error", "IRSpectrogram : frag file " + url + "not found! Please contact a developer with this error message.");
        }
        

        
        this->fragURL = url;
        this->fragmentText.load (url.toStdString());
        this->fragmentCode = fragmentText.getStdString();
        this->fragmentRefreshed = true;
    
    //std::cout << this->fragmentCode << std::endl;
    

        
}

void IRWorkspace::shaderTask(Graphics& g)
{
    
    if (shader.get() == nullptr || shader->getFragmentShaderCode() != fragmentCode)
    {
       
        //shader.reset();
        if (fragmentCode.isNotEmpty() && this->fragmentRefreshed)
        {
            shader.reset (new OpenGLGraphicsContextCustomShader (fragmentCode));

            shader->onShaderActivated = [this](OpenGLShaderProgram& program){setUniform(program);};
            /*
            if(!this->isTextureCreated)
            {
                createTexture();
                this->isTextureCreated = true;
            }*/
            std::cout << "shader compiling...\n";

            auto result = shader->checkCompilation (g.getInternalContext());
            if (result.failed()){ shader.reset();
                std::cout << "Error : shader compile error\n";
                std::cout << result.getErrorMessage() << std::endl;
            }
            
            this->fragmentRefreshed = false;
        }else{
           // std::cout << "fragmentCode empty and not refreshed\n";
        }
    }else{
       // std::cout << "shader null or shaderCode not loaded\n";
    }
    
    if (shader.get() != nullptr)
    {
        shader->fillRect (g.getInternalContext(),
                          getLocalBounds()
                          );
    }
}

void IRWorkspace::setUniform(OpenGLShaderProgram& program)
{
    
    // IMPORTANT : get the actual scale of the screen because MacOSX uses Retina display which has double piexles
    int scale = Desktop::getInstance().getDisplays().getMainDisplay().scale;
    int w = getWidth() * scale;
    int h = getHeight() * scale;
    
    program.setUniform("resolution", w, h);
    program.setUniform("gridInterval", this->thin_grids_pixel * scale, this->thick_grids_interval * scale);
    program.setUniform("sw", (float)isEditMode());
    
    //std::cout << "setUniform " << w << ", " << h << " : scale = " << scale << " : thinGrid = " << this->thin_grids_pixel * scale <<std::endl;
   
      
}

void IRWorkspace::update()
{
    
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->sp_w, this->sp_h, GL_DEPTH_COMPONENT, GL_FLOAT, this->buffer);
    this->updateTexture = false;
}

void IRWorkspace::createTexture()
{
    glDeleteTextures(1, &this->textureID);

    this->textureID = 0;
    glGenTextures(1, &this->textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->sp_w, this->sp_h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, this->buffer);
    
}
