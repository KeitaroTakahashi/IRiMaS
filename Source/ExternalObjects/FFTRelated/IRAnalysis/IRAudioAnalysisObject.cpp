//
//  IRAudioAnalysisObject.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/04/2019.
//

#include "IRAudioAnalysisObject.hpp"



IRAudioAnalysisObject::IRAudioAnalysisObject(Component* parent) : IRNodeObject(parent, "IRAudioAnalysis")
{
    
    this->analysisUI = std::make_shared<IRAnalysisUI>(this);
    this->analysisUI->addChangeListener(this);
    //this->waveform->addKeyListener(this);
    this->analysisUI->setBounds(this->xMargin,
                              this->yMargin,
                              getWidth()-(this->xMargin*2),
                              getHeight()-(this->yMargin*2));
    this->analysisUI->setEditMode(isEditMode());
    
    this->analysisUI->completeAnalysis = [this]{ analaysisCompleted(); };
    
    //IRFileManager
    
    //addKeyListener(this->waveform);
    addAndMakeVisible(this->analysisUI.get());
    childComponentManager(this->analysisUI.get());
    
    setSize(300,100);
    
    clearLinkParam();
    addLinkParam(AudioLinkFlag);
    addLinkParam(DataLinkFlag);
    addLinkParam(ConsoleLinkFlag);
    
    
}


IRAudioAnalysisObject::~IRAudioAnalysisObject()
{
  
}


// copy constructor
IRNodeObject* IRAudioAnalysisObject::copyThis()
{
    return new IRAudioAnalysisObject(this->parent);
}


IRNodeObject* IRAudioAnalysisObject::copyContents(IRNodeObject* object)
{
    IRAudioAnalysisObject* obj = static_cast<IRAudioAnalysisObject*>(object);
    
    // temporary set the same bounds to calculate all other child components at the right positions.
    obj->setBounds(getLocalBounds());
    // open the same audio file
    obj->analysisUI->openFile(this->analysisUI->getPath());
   
    return obj;
}

IRNodeObject* IRAudioAnalysisObject::copyDragDropContents(IRNodeObject* object)
{
    IRAudioAnalysisObject* obj = static_cast<IRAudioAnalysisObject*>(object);
    
    obj->analysisUI->openFile(this->analysisUI->getPath());
    
    return obj;
}


t_json IRAudioAnalysisObject::saveThisToSaveData()
{
    //start making Json for Selections
    
    
    //waveform
    Rectangle<int> wb = this->analysisUI->getBounds();
    t_json save_analysisUI = t_json::object({
        {"bounds", t_json::array({wb.getX(), wb.getY(), wb.getWidth(), wb.getHeight()})},
        {"filePath", this->analysisUI->getPath().toStdString()}
    });
    
    
    t_json save = t_json::object({
        {"analysisUI", save_analysisUI}
    });
    
    return save;
}


void IRAudioAnalysisObject::loadThisFromSaveData(t_json data)
{
    t_json w = data["analysisUI"];
    
    this->analysisUI->openFile(w["filePath"].string_value());
    this->analysisUI->repaint();
}


void IRAudioAnalysisObject::resized()
{
    this->analysisUI->setSize(getWidth()-10, getHeight()-10);
}


void IRAudioAnalysisObject::paint(Graphics& g)
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
void IRAudioAnalysisObject::statusChangedCallback(IRNodeComponentStatus status)
{
    switch (status)
    {
        case EditModeStatus:
            this->analysisUI->setEditMode(isEditMode());
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
void IRAudioAnalysisObject::mouseDownEvent(const MouseEvent& e)
{
    IRNodeObject::mouseDownEvent(e);
    
}


void IRAudioAnalysisObject::changeListenerCallback(ChangeBroadcaster* source)
{
  
}

int IRAudioAnalysisObject::getXMargin() const
{
    return this->xMargin;
}


int IRAudioAnalysisObject::getYMargin() const
{
    return this->yMargin;
}

void IRAudioAnalysisObject::analaysisCompleted()
{
    String id = "IRAnalysisMagnitude";
    IRAnalysisDataStr* magData = this->analysisUI->magData.get();
    
    this->callAddObjectGlobal(static_cast<IRObjectPtr>(magData), id);
    
    
    IRObjectPtr val = nullptr;
    
    //get pointer of the data
    val = this->callGetObjectGlobal(id);

    // if success
    if(val != nullptr){
    
    }else{
        std::cout << id << " NOT found\n";
    }
    std::cout << id << " stored\n";
}
