//
//  IRSpectrogram.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 03/09/2018.
//

#ifndef IRSpectrogramObject_hpp
#define IRSpectrogramObject_hpp

#include "IRNodeObject.hpp"
#include "IRObjectSelection.hpp"
#include "IRSpectrogramObjectPreference.h"

#include "IRSpectrogramUI.h"

class IRSpectrogramObject : public IRNodeObject,
IRNodeObject::Listener,
KeyListener,
private ChangeListener
{
public:
    IRSpectrogramObject(Component* parent) : IRNodeObject(parent, "IRSpectrogram")
    {
        this->preference = new IRSpectrogramObjectPreference("Spectrogram Preference", Rectangle<int>(400,720));
        
        this->spectrogram = new IRSpectrogramUI();
        this->spectrogram->addChangeListener(this);
        //this->spectrogram->addKeyListener(this);
        this->spectrogram->setBounds(this->xMargin,
                                  this->yMargin,
                                  getWidth()-(this->xMargin*2),
                                  getHeight()-(this->yMargin*2));
        this->spectrogram->setEditMode(isEditMode());
        addAndMakeVisible(this->spectrogram);
        childComponentManager(this->spectrogram);
        
        this->selector = new IRObjectSelection<Component*>();
        
        // in case we use any AudioSources, we must add them to the internal mixer via this method.
        addAudioComponent(this->spectrogram->getPlayer());
        
        setSize(300,100);

    }
    ~IRSpectrogramObject() { /* delete this->selector; // FD ATTEMPT */ }
    
    //==========================================================================
    // copy constructor
    IRNodeObject* copyThis() override
    {
        return new IRSpectrogramObject(this->parent);
    }
    // ------------------------------------------------------------
    IRNodeObject* copyThisWithContents() override
    {
        IRSpectrogramObject* obj = new IRSpectrogramObject(this->parent);
        // temporary set the same bounds to calculate all other child components at the right positions.
        obj->setBounds(this->getX(), this->getY(), this->getWidth(), this->getHeight());
        // open the same audio file
        obj->spectrogram->audioFile.openFileFromPath(this->spectrogram->audioFile.getPath());
        // add all selectionSquares
        for(auto o : this->spectrogram->selectionSquareObjects)
        {
            Rectangle<int>rect(o->getX(), o->getY(), o->getWidth(), o->getHeight());
            obj->spectrogram->createSquareObject(rect);
        }
        return obj;
    }
    // ------------------------------------------------------------
    IRNodeObject* copySelectedContents() override
    {
        IRSpectrogramObject* obj = new IRSpectrogramObject(this->parent);
        // temporary set the same bounds to calculate all other child components at the right positions.
        //obj->setBounds(this->getX(), this->getY(), this->getWidth(), this->getHeight());
        // open the same audio file
        obj->spectrogram->audioFile.openFileFromPath(this->spectrogram->audioFile.getPath());
        
        for(auto o : this->spectrogram->selectedSquareObjectList)
        {
            //Rectangle<int>rect(o->getX(), o->getY(), o->getWidth(), o->getHeight());
            //obj->waveform->createSquareObject(rect);
            obj->setBounds(o->getX(), o->getY(), o->getWidth() + (this->xMargin*2), o->getHeight() + (this->yMargin*2));
            
            Rectangle<float> bounds = o->getBoundsInRatio();
            double startms = ((double)this->spectrogram->getStart() + (float)this->spectrogram->getDisplayDuration()*bounds.getX());
            double durationms = ((double)this->spectrogram->getDisplayDuration() * bounds.getWidth()) + startms;
            obj->spectrogram->setStart(startms);
            obj->spectrogram->setDisplayDuration(durationms);
            
            std::cout << "start = " << startms << " : duration = " << durationms << std::endl;
        }
        
        return obj;
    }
    // ------------------------------------------------------------
    void loadThisFromSaveData(t_json saveData) override
    {
        
    }
    
    //==========================================================================
    // ------------------------------------------------------------
    void resized() override
    {
        this->spectrogram->setSize(getWidth()-10, getHeight()-10);
    }
    // ------------------------------------------------------------
    
    void paint(Graphics& g) override
    {
        if(isEditMode())
        {
            auto area = getLocalBounds();//.reduced (2);
            
            g.setColour (SYSTEMCOLOUR.contents);
            g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
        }
    }
    
    // ------------------------------------------------------------
    
    void mouseDownEvent(const MouseEvent& e) override
    {
        //change preference Window if not yet
        IRPreferenceSpace* space = getPreferenceWindow()->getPreferenceSpace();
        IRPreferenceObject* current = space->getPreferenceObj();
        
        if(current != preference){
            space->setPreferenceObj(preference);
        }
    }
    
    //==========================================================================
    // ------------------------------------------------------------
    // call back function automatically called when the status of this object changed by others.
    // write some tasks here
    void statusChangedCallback(IRNodeComponentStatus status) override
    {
        switch (status)
        {
            case EditModeStatus:
                this->spectrogram->setEditMode(isEditMode());
                break;
            case SelectableStatus:
                break;
            case HasResizedStatus:
                break;
            default:
                break;
        }
    }
    // ------------------------------------------------------------
    void changeListenerCallback (ChangeBroadcaster* source)override
    {
        if(source == this->spectrogram)
        {
            switch(this->spectrogram->status)
            {
                case IRSpectrogramUI::DRAGOUT:
                    this->callDragOutNodeObjectFromParent();
                    break;
                case IRSpectrogramUI::DROPOUT:
                    this->callDropOutNodeObjectFromParent();
                    break;
                case IRSpectrogramUI::EDITMODECHANGE:
                    // first change its EditMode status
                    setEditMode(! isEditMode());
                    // then inform the new edit mode status by calling this
                    this->callEditModeChangedInNodeObject();
                    break;
                case IRSpectrogramUI::PROJECTSAVE:
                    // request IRMAIN to save this project.
                    // IRNodeObject -> IRWorkspace -> IRProject -> IRMAIN
                    this->callSaveProject();
                default:
                    break;
            }
        }
    }
    //==========================================================================
    // ------------------------------------------------------------
    int getXMargin() const { return this->xMargin; }
    int getYMargin() const { return this->yMargin; }
    //==========================================================================
    bool keyPressed (const KeyPress& key,
                     Component* originatingComponent) override
    {
        return true;
    }

    
    IRSpectrogramUI* spectrogram;
    // preference
    IRSpectrogramObjectPreference *preference;
    
private:
    
    IRObjectSelection<Component* >* selector;
    bool selectFlag = true;
    bool isSelectMode = false;
    
    int xMargin = 5;
    int yMargin = 5;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRSpectrogramObject)
};


#endif /* IRSpectrogramObject_hpp */
