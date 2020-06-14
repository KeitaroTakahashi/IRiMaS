//
//  IRSpectrogramObject.cpp
//  IRiMaS
//
//  Created by Keitaro on 26/06/2019.
//

#include "IRSpectrogramObject.hpp"

IRSpectrogramObject::IRSpectrogramObject(Component* parent, IRStr* str) :
IRNodeObject(parent, "IRSpectrogram", str, NodeObjectType(ordinaryIRComponent))
{
    
    setOpaque(false);
    this->UI = std::make_shared<IRSpectrogramWithPreference>(this, str);
    addAndMakeVisible(this->UI.get());
    this->UI->audioFileImportCompletedCallback = [this]{ audioFileImportCompleted(); };
    this->UI->setEditMode(isEditMode());
    this->UI->setBounds(0, 0, getWidth(), getHeight());
    childComponentManager(this->UI.get());
    
    this->controller.reset(new IRSpectrogramController2(str));
    this->controller->audioController.addChangeListener(this);
    setObjController(this->controller.get());
    
    setObjectSize(500,500);

}

IRSpectrogramObject::~IRSpectrogramObject()
{
    
}


// copy constructor
IRNodeObject* IRSpectrogramObject::copyThis()
{
    std::cout << "IRSpectrogramObject copyThis " << this << std::endl;
    return new IRSpectrogramObject(this->parent, getStr());
}


IRNodeObject* IRSpectrogramObject::copyContents(IRNodeObject* object)
{
    IRSpectrogramObject* obj = static_cast<IRSpectrogramObject*>(object);
    return obj;
}




t_json IRSpectrogramObject::saveThisToSaveData()
{
    
    std::string p = this->UI->getSpectrogram()->getFilePath().toStdString();
    t_json sp = t_json::object({
        {"bounds", t_json::array({getX(), getY(), getWidth(), getHeight()})},
        //{"start_ms",this->waveform->getWaveformUI()->getStart()},
        //{"duration_ms", this->waveform->getWaveformUI()->getDisplayDuration()},
        {"filePath", p}
        //{"selectionObj", selectionData}
    });
    
    t_json save = t_json::object({
        {"spectrogram", sp}
    });
    

    return save;
}


void IRSpectrogramObject::loadThisFromSaveData(t_json data)
{
    t_json s = data["spectrogram"];
    String path = s["filePath"].string_value();
    if(path.length() > 0)
    {
        this->UI->getSpectrogram()->openFile(path);
    }
    
    this->UI->getSpectrogram()->repaint();
}

// ==================================================

void IRSpectrogramObject::resized()
{
    this->UI->setBounds(0, 0, getWidth(), getHeight());
}


void IRSpectrogramObject::paint(Graphics& g)
{
    
    g.fillAll(getStr()->SYSTEMCOLOUR.background);
    if(isEditMode())
    {
        auto area = getLocalBounds();//.reduced (2);
        
        g.setColour (getStr()->SYSTEMCOLOUR.contents);
        //g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
        g.drawRect(area.toFloat(), 1.0);
    }
}
// ==================================================

void IRSpectrogramObject::heavyComponentRefreshed()
{
    
    std::cout << "Spectrogram refreshed\n";
    this->UI->heavyComponentRefreshed();
}

// ==================================================

// call back function automatically called when the status of this object changed by others.
// write some tasks here
void IRSpectrogramObject::statusChangedCallback(IRNodeComponentStatus status)
{
    switch (status)
    {
        case EditModeStatus:
            if(isEditMode())
            {
            }else{
            }
            break;
        case SelectableStatus:
            break;
        case HasResizedStatus:
            this->isBeingResized = true;
            break;
        default:
            break;
    }
}


// ==================================================


// events
void IRSpectrogramObject::mouseDownEvent(const MouseEvent& e)
{
    std::cout << "IRSpectrogramObject Object mouse down\n";
    
    //IRNodeObject::mouseDownEvent(e);
    //change preference Window if not yet
    if(getPreferenceWindow() != nullptr)
    {
        /*
        IRPreferenceSpace* space = getPreferenceWindow()->getPreferenceSpace();
        
        IRPreferenceObject* current = space->getPreferenceObj();
            
        if(current != this->UI->getPreference()){
            space->setPreferenceObj(this->UI->getPreference());
        }*/
    }
}

// ==================================================

void IRSpectrogramObject::IRChangeListenerCallback(ChangeBroadcaster* source)
{
   if(source == &this->controller->audioController)
   {
       audioControllerChangeListener();

   }
}

void IRSpectrogramObject::audioFileImportCompleted()
{
    callHeavyComponentCreated(this);
}

// received from AudioController
void IRSpectrogramObject::audioControllerChangeListener()
{
    auto status = this->controller->audioController.getStatus();
    std::cout << "status = " << status << std::endl;
    using s = AudioObjectController::AudioObjectControllerStatus;

    switch(status)
    {
        case s::AudioFileOpen:
            audioFileOpenAction();
            break;
        case s::PLAY:
            
            break;
        case s::PAUSE:
            break;
        case s::STOP:
            break;
            
        default:
            break;
    }
}
// ==================================================

void IRSpectrogramObject::mouseUpEvent(const MouseEvent& e)
{
    if(this->isBeingResized)
    {
        //this->spectrogram->recreateSpectrogram();
        this->isBeingResized = false;
    }
}
// ==================================================


void IRSpectrogramObject::moveToFrontAction()
{    
    this->UI->getSpectrogram()->bringThisToFront();
}
// ==================================================

void IRSpectrogramObject::audioFileOpenAction()
{
    this->UI->openFile();
    String path = this->UI->getFilePath();
    this->controller->audioController.setLoadedAudioFilePath(path);
}

// ==================================================
// ==================================================
// ==================================================
