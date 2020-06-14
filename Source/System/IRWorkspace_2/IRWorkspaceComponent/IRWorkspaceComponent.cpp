//
//  IRWorkspaceComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 18/09/2019.
//

#include "IRWorkspaceComponent.hpp"

IRWorkspaceComponent::IRWorkspaceComponent(String title, Rectangle<int> draggableMargin, IRStr* str) :
IRStrComponent(str),
IRHeavyWeightComponent(this, "IRWorkspaceComponent"),
draggableMargin(draggableMargin)
{
    
    this->name = title;
    this->title = this->name + " (EDIT MODE)";
    
    this->ir_str = str;
    setOpaque(false);
    
    setWantsKeyboardFocus(true);
    //addKeyListener(this);
    if(this->ir_str != nullptr)
    {
        
        // needs to be controlled from outside of this class!
        //addKeyListener(this->irt_str->key);
        //addMouseListener(this->ir_str->mouse, false);
    }
    
    createCover();
    
    //give object lists to selector
    this->selector = new IRNodeObjectSelector(&this->objects, this->draggableMargin);
    
    this->selector->setShiftConstrainsDirection(true);
    this->selector->setConstrainBoundsToParent(true, {0,0,10,10});
    
    //openGLInit();
    
    
}


IRWorkspaceComponent::~IRWorkspaceComponent()
{
    std::cout << "workspace destructor called" << std::endl;
    //delete this->objectMenuComponent;
    delete this->selector;
    
    // juce::Array free the memory space as well when cleared.
    for(auto obj : this->objects)
    {
        delete obj;
    }
    this->objects.clearQuick();
    
    this->cover.reset();
}


void IRWorkspaceComponent::paint (Graphics& g)
{
    g.fillAll(this->backgroundColour);
    //g.fillAll(Colours::purple);
    //if(isDrawGrids() && this->editModeFlag) shaderTask(g);
    
    //g.setColour(Colours::white);
    //g.drawRect(getLocalBounds());
    
    // virtual method
    onPaint(g);
    std::cout << " ++++++ workspace repained! ++++++ " << this->bench.stop() << std::endl;

}

void IRWorkspaceComponent::drawShadows(Graphics& g)
{
    // std::cout << "drawing... shadow of " << this->selector->getSelectedObjectList().size() << " items" << std::endl;
    auto list = this->selector->getSelectedObjectList();
    
    for(auto obj : list)
    {
        DropShadow shadow(getStr()->SYSTEMCOLOUR.contents, 30, juce::Point<int>(0,0));
        
        Rectangle<int> b = obj->getBounds();
        Rectangle<int> bounds(b.getX(), b.getY(), b.getWidth(), b.getHeight());
        
        shadow.drawForRectangle(g, bounds);
        
        // we need to explicitly repaint the area of shadow, otherwise it won't be erased.
        repaint(b.getX()-50, b.getY()-50, b.getWidth()+100, b.getHeight()+100);
    }
}
// ==================================================

void IRWorkspaceComponent::copyAllDataToWorkspace(IRWorkspaceComponent* newWorkspace)
{
    auto w = newWorkspace;
    
    //w->enableDrawGrids(isDrawGrids());
    //w->setGridsBackgroundAlpha(this->gridsBackgroundAlpha);
    
    /*
    if(this->parentNodeObject != nullptr)
    {
        auto p = this->parentNodeObject->copyThis();
        w->setParentNodeObject(p);
    }
    
    for(auto obj : this->objects)
    {
        auto o = obj->copyThis();
        w->createObject(o);
    }*/
    
}

// ==================================================

// ==================================================
// APPEARANCE
// ==================================================

void IRWorkspaceComponent::setBackgroundColour(Colour colour)
{
    this->backgroundColour = colour;
    repaint();
}

// ==================================================

void IRWorkspaceComponent::resized()
{
    // update initial
    this->initialBounds = getBounds().toFloat();
    
    
    resizeNodeObjectsRelativeToWorkspaceSizeChange();
    
    Rectangle<int>   area = getLocalBounds();
    area.setX       (area.getX() + this->draggableMargin.getX());
    area.setY       (area.getY() + this->draggableMargin.getY());
    area.setWidth   (area.getWidth() + this->draggableMargin.getWidth());
    area.setHeight  (area.getHeight() + this->draggableMargin.getHeight());
    
    this->selector->setDraggableArea(area);
    
    if(this->cover.get() != nullptr)
    {
        std::cout << "cover resized : " << getLocalBounds().getWidth() << std::endl;
        this->cover->setBounds(getLocalBounds());
        
    }
    
    onResized();

}

void IRWorkspaceComponent::resizeNodeObjectsRelativeToWorkspaceSizeChange()
{
    if(!this->isFixObjectSizeRatio && this->parentNodeObject != nullptr)
    {
        
        std::cout << "resizeNodeObjectsRelativeToWorkspaceSizeChange\n";
        if(this->previousBounds.getWidth() != 0 &&
           this->previousBounds.getHeight() != 0)
        {
            //auto wb = getBounds().toFloat();
            auto wb = this->previousBounds.toFloat();
            //auto wb = this->parentNodeObject->getBounds();
            float ratioX = this->initialBounds.getX() / wb.getX();
            float ratioY = this->initialBounds.getY() / wb.getY();
            float ratioW = this->initialBounds.getWidth() / wb.getWidth();
            float ratioH = this->initialBounds.getHeight() / wb.getHeight();

            
            std::cout <<"w = " << ratioW << " : " << ratioH << " : initial(w, h) = " << this->initialBounds.getWidth() << ", " <<  this->initialBounds.getHeight() << " : current = " << wb.getWidth() << ", " << wb.getHeight() << std::endl;

            for(auto obj : this->objects)
            {
                
                Rectangle<float> rb((float)obj->getInitialBounds().getX() * ratioW,
                                    (float)obj->getInitialBounds().getY() * ratioH,
                                    (float)obj->getInitialBounds().getWidth() * ratioW,
                                    (float)obj->getInitialBounds().getHeight() * ratioH);
                
                obj->setBoundType(IRNodeComponentBoundsType::RELATIVE);
                obj->setObjectBounds(rb.toNearestInt());
                obj->setBoundType(IRNodeComponentBoundsType::ORDINARY);

                
            }
        }
    }
    setPreviousBounds(getBounds().toFloat());
}
// ==================================================


void IRWorkspaceComponent::setPreviousBounds(Rectangle<float> previousBounds)
{
    this->previousBounds = previousBounds;
}

bool IRWorkspaceComponent::isPreviousBoundsAlreadyGiven() const
{
    if(this->previousBounds.getWidth() == 0 &&
       this->previousBounds.getHeight() == 0)
    {
        return false;
    }else return true;
}

// ==================================================

void IRWorkspaceComponent::setFixObjectSizeRatio(bool flag, Rectangle<float> initialBounds)
{
    this->isFixObjectSizeRatio = flag;
    this->initialBounds = initialBounds;
    std::cout << "setFixObjectSizeRatio IRWorkspace updated\n";
}


// ==================================================

void IRWorkspaceComponent::setDraggableMargin(Rectangle<int> newMargin)
{
    this->draggableMargin = newMargin;
}


void IRWorkspaceComponent::mouseDown(const MouseEvent& e)
{
    std::cout << "IRWorkspaceComponent mouseDown " << e.getEventRelativeTo(this).getPosition().getX() << ", " << e.getEventRelativeTo(this).getPosition().getY() << std::endl;
    
    this->selector->mouseDownHandler(e.getEventRelativeTo(this));
    
    if(isEditMode())
    {
        addAndMakeVisible(this->selector);
        this->isMultiSelectMode = true;
        //closeObjectListMenu();
    }
    
    // if workspace empty place is clicked, then send a "nothing selected" signal.
    if(this->selector->getSelectedObjectList().size() == 0)
    {
        callNothingSelected();
    }
    
}


void IRWorkspaceComponent::mouseMove(const MouseEvent& e)
{
    
    this->currentMousePosition = e.getEventRelativeTo(this).getPosition();
    
    //std::cout << currentMousePosition.getX() << " , " << currentMousePosition.getY() << std::endl;
    
}


void IRWorkspaceComponent::mouseUp(const MouseEvent& e)
{
    
    //std::cout << "IRWorkspaceComponent mouseUp " << e.getPosition().getX() << ", " << e.getPosition().getY() << std::endl;
    this->selector->mouseUpHandler(e.getEventRelativeTo(this));

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


void IRWorkspaceComponent::mouseDrag(const MouseEvent& e)
{
    
    //std::cout << "IRWorkspaceComponent mouseDrag\n";
    if(isEditMode())
    {
        this->currentMousePosition = e.getEventRelativeTo(this).getPosition();
        
        if(!this->isResizingFlag){
            this->selector->mouseDragHandler(e.getEventRelativeTo(this));
        }
        
        if(this->dummy.size() > 0)
        {
            for(auto obj : this->dummy)
            {
                obj->setObjectCentredPosition(this->currentMousePosition.getX(),
                                              this->currentMousePosition.getY());
            }
        }
    }
}


void IRWorkspaceComponent::mouseDoubleClick(const MouseEvent& e)
{
    this->currentMousePosition = e.getEventRelativeTo(this).getPosition();
    
    if(isEditMode())
    {
        
        // we need the screen position
        //openObjectListMenu(e.getScreenPosition());
    }
}


void IRWorkspaceComponent::modifierKeysChanged(const ModifierKeys &mod)
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


void IRWorkspaceComponent::changeListenerCallback (ChangeBroadcaster* source)
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
            this->selector->deselectOtherObejcts(obj);
            //this->selector->addSelectedObjects();
        }
    }else{ std::cout << "changeListenerCallback object nullptr\n"; }
}


void IRWorkspaceComponent::AudioSetup()
{
    setAudioChannels(0, 2);
}


void IRWorkspaceComponent::closeAudioSetup()
{
    shutdownAudio();
}


void IRWorkspaceComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    this->mixer.getAudioSource().prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void IRWorkspaceComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    this->mixer.getAudioSource().getNextAudioBlock(bufferToFill);
}


void IRWorkspaceComponent::releaseResources()
{
    this->mixer.getAudioSource().releaseResources();
}


AudioSource& IRWorkspaceComponent::getMixer()
{
    return this->mixer.getAudioSource();
}


json11::Json IRWorkspaceComponent::makeSaveDataOfThis()
{
    
    std::cout << "makeSaveDataOfThis" << std::endl;
    std::vector<json11::Json::object> objectList;
    
    // to save objects, we need to reverse the order of ObjectZorder
    // The top object is stored the begining of the vector but it needs to be at the end in order to be created at last.
    std::vector<IRNodeObject*> reversedZorder = this->ObjectZorder;
    std::reverse(std::begin(reversedZorder), std::end(reversedZorder));

    KLib k;
    int index = 0;
    for(auto item : reversedZorder)
    {
        std::string s = k.GetNextNumber("object-", index, 5);
        
        /*
        json11::Json::object ob = json11::Json::object({
            {s, json11::Json::object({

                {"objectType",          item->name.toStdString()},
                {"objectUniqueID",      item->getUniqueID().toStdString()},
                {"bounds",              json11::Json::array({item->getX(), item->getY(), item->getWidth(), item->getHeight()})},
                {"status",              "Normal"},
                {"ObjectDefined",       item->saveThisToSaveData()}
                
            })},
        });*/
        
        json11::Json::object ob = json11::Json::object({
            {s, json11::Json::object({

                {"objectType",          item->name.toStdString()},
                {"objectUniqueID",      item->getUniqueID().toStdString()},
                {"ArrangeController",   item->getArrangeControllerSaveData()},
                {"ObjectDefined",       item->saveThisToSaveData()}
                
            })},
        });
        
        objectList.push_back(ob);
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


bool IRWorkspaceComponent::isEditMode() const
{
    return this->editModeFlag;
}


void IRWorkspaceComponent::setEditMode(bool flag, bool notification)
{
    
    std::cout << "IRWorkspaceComponent::setEditMode : " << flag << std::endl;
    this->editModeFlag = flag;
    
    // reset all selected objects
    this->selector->deselectAllObjects();
    
    // change cover as well
    setCoverEditMode(flag);
    
    for (auto obj : this->objects)
    {
        obj->setEditMode(flag);
    }
    
    if(this->parentNodeObject != nullptr)
    {
        //this->parentNodeObject->setEditMode(flag);
    }
  
    // notify to IRMainSpace
    if(notification) callEditModeChanged();
    
    
    // send change message to IRProject
    sendChangeMessage();
    
    repaint();
}

Array<IRNodeObject*> IRWorkspaceComponent::getObjectList()
{
    return this->objects;
}

Image IRWorkspaceComponent::getSnap()
{
    this->snap = createComponentSnapshot(Rectangle<int>(0, 0, this->getWidth(), this->getHeight()),
                                         false, 0.4);
    return this->snap;
}


void IRWorkspaceComponent::addListener(Listener* newListener)
{
    this->listeners.add(newListener);
}


void IRWorkspaceComponent::removeListener(Listener* listener)
{
    this->listeners.remove(listener);
}

// ==================================================

Image IRWorkspaceComponent::loadImage(String url)
{
    File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + url);
    return ImageFileFormat::loadFrom(file);
}

// ==================================================
// OpenGL

void IRWorkspaceComponent::openGLInit()
{
    
    if (auto* peer = getPeer())
            peer->setCurrentRenderingEngine (0);
                
        String url = File::getSpecialLocation(File::currentApplicationFile).getFullPathName();
    
    #if JUCE_MAC
        url += "/Contents/Resources/materials/Sources/GLSL/grid/KGrid2.frag";
    #elif JUCE_IOS
        url += "/materials/Sources/GLSL/grid/KGrid2.frag";
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

void IRWorkspaceComponent::shaderTask(Graphics& g)
{
    
    if (shader.get() == nullptr || shader->getFragmentShaderCode() != fragmentCode)
    {
       
        //shader.reset();
        if (fragmentCode.isNotEmpty() && this->fragmentRefreshed)
        {
            
            glEnable (GL_BLEND);
            glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            
            shader.reset (new OpenGLGraphicsContextCustomShader (fragmentCode));

            shader->onShaderActivated = [this](OpenGLShaderProgram& program){setUniform(program);};
           
            auto result = shader->checkCompilation (g.getInternalContext());
            if (result.failed()){ shader.reset();
                std::cout << "Error : shader compile error\n";
                std::cout << result.getErrorMessage() << std::endl;
            }
            this->fragmentRefreshed = false;
        }
    }
    
    if (shader.get() != nullptr)
    {
  
        shader->fillRect (g.getInternalContext(),
                          getLocalBounds()
                          );
         
    }
}

void IRWorkspaceComponent::setUniform(OpenGLShaderProgram& program)
{
    
    // IMPORTANT : get the actual scale of the screen because MacOSX uses Retina display which has double piexles
    int scale = Desktop::getInstance().getDisplays().getMainDisplay().scale;
    int w = getWidth() * scale;
    int h = getHeight() * scale;
    
    program.setUniform("resolution", w, h);
    program.setUniform("gridInterval", this->thin_grids_pixel * scale, this->thick_grids_interval * scale);
    program.setUniform("sw", (float)isEditMode());
    program.setUniform("alpha", this->gridsBackgroundAlpha);
    
    //std::cout << "setUniform " << w << ", " << h << " : scale = " << scale << " : thinGrid = " << this->thin_grids_pixel * scale <<std::endl;
   
      
}

void IRWorkspaceComponent::update()
{
    
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->sp_w, this->sp_h, GL_DEPTH_COMPONENT, GL_FLOAT, this->buffer);
    this->updateTexture = false;
}

void IRWorkspaceComponent::createTexture()
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
// ==================================================


void IRWorkspaceComponent::bringToFrontCompleted()
{
    if(this->cover.get() != nullptr)
    {
        bringCoverToFront();
    }
}

// ==================================================

void IRWorkspaceComponent::enableDrawGrids(bool flag)
{
    this->isdrawGridsFlag = flag;
    
    this->cover->enableDrawGrids(flag);
}


// ==================================================
