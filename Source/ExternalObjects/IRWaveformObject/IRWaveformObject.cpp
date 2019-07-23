
#include "IRWaveformObject.hpp"




IRWaveformObject::IRWaveformObject(Component* parent) : IRNodeObject(parent, "IRWaveform")
{
    
    std::cout << "IRWaveformObject " << this << std::endl;
    this->waveform = new IRWaveformObjectUI2(this);
    this->waveform->getWaveformUI()->addChangeListener(this);
    //this->waveform->addKeyListener(this);
    this->waveform->setBounds(this->xMargin,
                              this->yMargin,
                              getWidth()-(this->xMargin*2),
                              getHeight()-(this->yMargin*2));
    this->waveform->setEditMode(isEditMode());
    
    //IRFileManager
    
    //addKeyListener(this->waveform);
    addAndMakeVisible(this->waveform);
    childComponentManager(this->waveform);
    
    this->selector = new IRObjectSelection<Component*>();
    
    // in case this object uses any AudioSources, we must register this to the internal mixer by this method.
    addAudioComponent(this->waveform->getWaveformUI()->getPlayer());
    
    setSize(400,150);
    
    clearLinkParam();
    addLinkParam(AudioLinkFlag);
    addLinkParam(DataLinkFlag);
    addLinkParam(ConsoleLinkFlag);
    
    
}

IRWaveformObject::~IRWaveformObject()
{
    delete this->waveform;
    delete this->selector;
    
    std::cout << "~IRWaveformObject()\n";
   
}

// copy constructor
IRNodeObject* IRWaveformObject::copyThis()
{
    std::cout << "IRWaveformObject copyThis " << this << std::endl;
    return new IRWaveformObject(this->parent);
}

IRNodeObject* IRWaveformObject::copyContents(IRNodeObject* object)
{
    IRWaveformObject* obj = static_cast<IRWaveformObject*>(object);

    // temporary set the same bounds to calculate all other child components at the right positions.
    obj->setBounds(getLocalBounds());
    // open the same audio file
    obj->waveform->getWaveformUI()->openFile(this->waveform->getWaveformUI()->getPath());
    // add all selectionSquares
    for(auto o : this->waveform->getWaveformUI()->selectionSquareObjects)
    {
        Rectangle<int>rect(o->getX(), o->getY(), o->getWidth(), o->getHeight());
        obj->waveform->getWaveformUI()->createSquareObject(rect);
    }
    return obj;
}

IRNodeObject* IRWaveformObject::copyDragDropContents(IRNodeObject* object)
{
    IRWaveformObject* obj = static_cast<IRWaveformObject*>(object);
    
    obj->waveform->getWaveformUI()->openFile(this->waveform->getWaveformUI()->getPath());
    
    for(auto o : this->waveform->getWaveformUI()->selectedSquareObjectList)
    {
        //obj->setBounds(o->getX(), o->getY(), o->getWidth() + (this->xMargin*2), o->getHeight() + (this->yMargin*2));
        
        //obj->setSize(o->getWidth() + (this->xMargin*2), o->getHeight() + (this->yMargin*2));
        obj->setSize(o->getWidth() + // width
                     this->waveform->getGridSize() +
                     this->waveform->getXMargin()*2,
                     o->getHeight() + // height
                     this->waveform->getGridSize() +
                     this->waveform->getYMargin()*2 +
                     this->waveform->getScrollSpace());
        

        
        Rectangle<float> bounds = o->getBoundsInRatio();
        double startms = ((double)this->waveform->getWaveformUI()->getStart() + (float)this->waveform->getWaveformUI()->getDisplayDuration()*bounds.getX());
        double durationms = ((double)this->waveform->getWaveformUI()->getDisplayDuration() * bounds.getWidth());
        obj->waveform->getWaveformUI()->setStart(startms);
        obj->waveform->getWaveformUI()->setDisplayDuration(durationms);
    }
    
    return obj;
}


t_json IRWaveformObject::saveThisToSaveData()
{
     //start making Json for Selections
 
    std::vector<t_json> selectionData;
    int index = 0;
    for(auto o : this->waveform->getWaveformUI()->selectionSquareObjects)
    {
        t_json d = t_json::object({
            {"rect_"+std::to_string(index), t_json::array({o->getX(), o->getY(), o->getWidth(), o->getHeight()})}
        });
        selectionData.push_back(d);
        
        index++;
    }
    
     //waveform
    Rectangle<int> wb = this->waveform->getBounds();
    t_json save_waveform = t_json::object({
        {"bounds", t_json::array({wb.getX(), wb.getY(), wb.getWidth(), wb.getHeight()})},
        {"start_ms",this->waveform->getWaveformUI()->getStart()},
        {"duration_ms", this->waveform->getWaveformUI()->getDisplayDuration()},
        {"filePath", this->waveform->getWaveformUI()->getPath().toStdString()},
        {"selectionObj", selectionData}
    });
    
    
    t_json save = t_json::object({
        {"waveform", save_waveform}
    });
    
    return save;
}


void IRWaveformObject::loadThisFromSaveData(t_json data)
{
    t_json w = data["waveform"];
    
    std::cout << "waveform : start ms = " << w["start_ms"].number_value() << std::endl;
    this->waveform->getWaveformUI()->setStart(w["start_ms"].number_value());
    this->waveform->getWaveformUI()->setDisplayDuration(w["duration_ms"].number_value());
    this->waveform->getWaveformUI()->openFile(w["filePath"].string_value());
    t_json::array selectionData = w["selectionObj"].array_items();
    std::cout << "selectionData count = " << selectionData.size() << std::endl;
    int index = 0;
    for(auto item : selectionData)
    {
        t_json s = item["rect_"+std::to_string(index)];
        
        Rectangle<int>rect(s[0].int_value(), s[1].int_value(), s[2].int_value(), s[3].int_value());
        
        this->waveform->getWaveformUI()->createSquareObject(rect);
        
        index++;
    }
    this->waveform->repaint();
    
}


void IRWaveformObject::resized()
{
    this->waveform->setSize(getWidth(), getHeight());
}


void IRWaveformObject::paint(Graphics& g)
{
    if(isEditMode())
    {
        auto area = getLocalBounds();//.reduced (2);
        
        g.setColour (SYSTEMCOLOUR.contents);
        //g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
        g.drawRect(area.toFloat(), 1.0);
    }
}


// call back function automatically called when the status of this object changed by others.
// write some tasks here
void IRWaveformObject::statusChangedCallback(IRNodeComponentStatus status)
{
    switch (status)
    {
        case EditModeStatus:
            this->waveform->setEditMode(isEditMode());
            break;
        case SelectableStatus:
            break;
        case HasResizedStatus:
            break;
        default:
            break;
    }
}


// events
void IRWaveformObject::mouseDownEvent(const MouseEvent& e)
{
    std::cout << "IRWaveform Object mouse down\n";
    
    IRNodeObject::mouseDownEvent(e);
    if(this->waveform->getWaveformUI()->isWaveformPainted())
    {
        double displayedDuration = this->waveform->getWaveformUI()->getDisplayDuration();
        double mouseDownProportion = (double)e.getMouseDownX() / (double) getWidth();
        double startTime = displayedDuration * mouseDownProportion;
        //this->waveform->setStart(startTime);
    }
    
}


void IRWaveformObject::changeListenerCallback(ChangeBroadcaster* source)
{
    if(source == this->waveform->getWaveformUI())
    {
        switch(this->waveform->getWaveformUI()->status)
        {
            case IRWaveformObjectUI::DRAGOUT:
                this->callDragOutNodeObjectFromParent();
                break;
            case IRWaveformObjectUI::DROPOUT:
                this->callDropOutNodeObjectFromParent();
                break;
                
                /*
            case IRWaveformObjectUI::EDITMODECHANGE:
                // first change its EditMode status
                setEditMode(! isEditMode());
                // then inform the new edit mode status by calling this
                this->callEditModeChangedInNodeObject();
                break;
                
            case IRWaveformObjectUI::PROJECTSAVE:
                // request IRMAIN to save this project.
                // IRNodeObject -> IRWorkspace -> IRProject -> IRMAIN
                this->callSaveProject();
                 */
            default:
                break;
        }
    }
}


int IRWaveformObject::getXMargin() const
{
    return this->xMargin;
}


int IRWaveformObject::getYMargin() const
{
    return this->yMargin;
}


/*
bool IRWaveformObject::keyPressed(const KeyPress& key, Component* originatingComponent)
{
    
    IRNodeObject::keyPressed(key, originatingComponent);
    return true;
}
*/


