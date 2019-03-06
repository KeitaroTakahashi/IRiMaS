
#include "IRWaveformObject.hpp"



/*
IRWaveformObject::IRWaveformObject(Component* parent) : IRNodeObject(parent, "IRWaveform")
{
    
    this->waveform = new IRWaveformUI();
    this->waveform->addChangeListener(this);
    //this->waveform->addKeyListener(this);
    this->waveform->setBounds(this->xMargin,
                              this->yMargin,
                              getWidth()-(this->xMargin*2),
                              getHeight()-(this->yMargin*2));
    this->waveform->setEditMode(isEditMode());
    //addKeyListener(this->waveform);
    addAndMakeVisible(this->waveform);
    childComponentManager(this->waveform);
    
    this->selector = new IRObjectSelection<Component*>();
    
    // in case this object uses any AudioSources, we must register this to the internal mixer by this method.
    addAudioComponent(this->waveform->getPlayer());
    
    setSize(300,100);
    
    
}


IRWaveformObject::~IRWaveformObject()
{
    
}


// copy constructor
IRNodeObject* IRWaveformObject::copyThis()
{
    return new IRWaveformObject(this->parent);
}


IRNodeObject* IRWaveformObject::copyThisWithContents()
{
    IRWaveformObject* obj = new IRWaveformObject(this->parent);
    // temporary set the same bounds to calculate all other child components at the right positions.
    obj->setBounds(getLocalBounds());
    // open the same audio file
    obj->waveform->audioFile.openFileFromPath(this->waveform->audioFile.getPath());
    // add all selectionSquares
    for(auto o : this->waveform->selectionSquareObjects)
    {
        Rectangle<int>rect(o->getX(), o->getY(), o->getWidth(), o->getHeight());
        obj->waveform->createSquareObject(rect);
    }
    return obj;
}


IRNodeObject* IRWaveformObject::copySelectedContents()
{
    IRWaveformObject* obj = new IRWaveformObject(this->parent);
    // temporary set the same bounds to calculate all other child components at the right positions.
    //obj->setBounds(this->getX(), this->getY(), this->getWidth(), this->getHeight());
    // open the same audio file
    obj->waveform->audioFile.openFileFromPath(this->waveform->audioFile.getPath());
    
    for(auto o : this->waveform->selectedSquareObjectList)
    {
        //Rectangle<int>rect(o->getX(), o->getY(), o->getWidth(), o->getHeight());
        //obj->waveform->createSquareObject(rect);
        obj->setBounds(o->getX(), o->getY(), o->getWidth() + (this->xMargin*2), o->getHeight() + (this->yMargin*2));
        
        Rectangle<float> bounds = o->getBoundsInRatio();
        double startms = ((double)this->waveform->getStart() + (float)this->waveform->getDisplayDuration()*bounds.getX());
        double durationms = ((double)this->waveform->getDisplayDuration() * bounds.getWidth());
        obj->waveform->setStart(startms);
        obj->waveform->setDisplayDuration(durationms);
    }
    
    return obj;
}


t_json IRWaveformObject::saveThisToSaveData()
{
     //start making Json for Selections
 
    std::vector<t_json> selectionData;
    int index = 0;
    for(auto o : this->waveform->selectionSquareObjects)
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
        {"start_ms",this->waveform->getStart()},
        {"duration_ms", this->waveform->getDisplayDuration()},
        {"filePath", this->waveform->audioFile.getPath().toStdString()},
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
    this->waveform->setStart(w["start_ms"].number_value());
    this->waveform->setDisplayDuration(w["duration_ms"].number_value());
    this->waveform->audioFile.openFileFromPath(w["filePath"].string_value());
    t_json::array selectionData = w["selectionObj"].array_items();
    std::cout << "selectionData count = " << selectionData.size() << std::endl;
    int index = 0;
    for(auto item : selectionData)
    {
        t_json s = item["rect_"+std::to_string(index)];
        
        Rectangle<int>rect(s[0].int_value(), s[1].int_value(), s[2].int_value(), s[3].int_value());
        
        this->waveform->createSquareObject(rect);
        
        index++;
    }
    this->waveform->repaint();
    
}


void IRWaveformObject::resized()
{
    this->waveform->setSize(getWidth()-10, getHeight()-10);
}


void IRWaveformObject::paint(Graphics& g)
{
    if(isEditMode())
    {
        auto area = getLocalBounds();//.reduced (2);
        
        g.setColour (SYSTEMCOLOUR.contents);
        g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
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
    if(this->waveform->isWaveformPainted())
    {
        double displayedDuration = this->waveform->getDisplayDuration();
        double mouseDownProportion = (double)e.getMouseDownX() / (double) getWidth();
        double startTime = displayedDuration * mouseDownProportion;
        //this->waveform->setStart(startTime);
    }
    
}


void IRWaveformObject::changeListenerCallback(ChangeBroadcaster* source)
{
    if(source == this->waveform)
    {
        switch(this->waveform->status)
        {
            case IRWaveformUI::DRAGOUT:
                this->callDragOutNodeObjectFromParent();
                break;
            case IRWaveformUI::DROPOUT:
                this->callDropOutNodeObjectFromParent();
                break;
            case IRWaveformUI::EDITMODECHANGE:
                // first change its EditMode status
                setEditMode(! isEditMode());
                // then inform the new edit mode status by calling this
                this->callEditModeChangedInNodeObject();
                break;
                
            case IRWaveformUI::PROJECTSAVE:
                // request IRMAIN to save this project.
                // IRNodeObject -> IRWorkspace -> IRProject -> IRMAIN
                this->callSaveProject();
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


bool IRWaveformObject::keyPressed(const KeyPress& key, Component* originatingComponent)
{
    
    IRNodeObject::keyPressed(key, originatingComponent);
    return true;
}


*/
