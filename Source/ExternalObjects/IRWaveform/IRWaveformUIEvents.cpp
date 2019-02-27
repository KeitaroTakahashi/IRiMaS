//
//  IRWaveformUIEvents.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/08/2018.
//

#include "IRWaveformUI.h"
// ------------------------------------------------------------

void IRWaveformUI::mouseDownHandler(const MouseEvent& e)
{
    IRMultiPurposeObject* nodeObj = dynamic_cast<IRMultiPurposeObject* >(e.originalComponent);

    if(nodeObj != nullptr)
    {

        if(e.mods.isShiftDown() || e.mods.isCommandDown())
        {
            nodeObj->setSelected(! nodeObj->isSelected());
        }else if(! nodeObj->isSelected()){ // if the object is not yet selected.
            deselectAllSquareObject(); // CLEAR
            nodeObj->setSelected(true);
        }
        //repaint obj graphics
        nodeObj->repaint();
        
        //add all selected nodeObj to a list
        addSelectedObjects();
        
        
    }else{
        if(! e.mods.isShiftDown() && ! e.mods.isCommandDown())
        {
            //if background clicked, clear all selected status
            deselectAllSquareObject();
            //add all selected nodeObj to a list
            addSelectedObjects();
        }
        // make selection square
        if(this->selectFlag && ! isEditMode())
        {
            this->selector->mouseDownHandler(e);
            addAndMakeVisible(this->selector);
            this->selectModeFlag = true;
        }
    }
 
}
// ------------------------------------------------------------

void IRWaveformUI::mouseUpHandler(const MouseEvent& e)
{
    if(this->dragdropCalled)
    {

        for(auto obj : this->selectedSquareObjectList)
        {
            //this->callDropOutNodeObjectFromParent();
            this->status = DROPOUT;
            sendChangeMessage();
        }
        
        this->dragdropCalled = false;
    }
}
// ------------------------------------------------------------

void IRWaveformUI::mouseDragHandler(const MouseEvent& e)
{
    
    // get position relative to the parent
    int x = e.getEventRelativeTo(this).getPosition().getX();
    int y = e.getEventRelativeTo(this).getPosition().getY();
    
    
    if(x > getWidth()   ||
       x < 0            ||
       y > getHeight()  ||
       y < 0)
    {
        if(! this->dragdropCalled)
        {
            for(auto obj : this->selectedSquareObjectList)
            {
                //this->callDragOutNodeObjectFromParent();
                this->status = DRAGOUT;
                sendChangeMessage();
            }
            this->dragdropCalled = true;
        }
    }
}
// ------------------------------------------------------------

void IRWaveformUI::playSelectedArea()
{
    for(auto obj : this->selectedSquareObjectList)
    {
        int displayedBegingSample = getStart() * (int)this->getSampleRate();
        int displaySampleLength = getDisplayDuration() * (int)this->getSampleRate();
        int totalLength = getTotalLength() * (int)this->getSampleRate();

        Rectangle<float> bounds = obj->getBoundsInRatio();
        int startSample     = round((float)displayedBegingSample + (float)displaySampleLength*bounds.getX());
        int durationSample  = round((float)displaySampleLength * bounds.getWidth());
        
        int endSampleIndex = startSample + durationSample;
        if(endSampleIndex > totalLength) durationSample -= (endSampleIndex - totalLength);
        play(startSample, durationSample, this->playOffset, this->looping);
        
        //std::cout << "play data from " << startSample << " for " << durationSample << " : displayedSampleLength = " << displaySampleLength << " : width ratio = " << bounds.getWidth() << std::endl;
    }
}
// ------------------------------------------------------------
void IRWaveformUI::stopPlaying()
{
   if(this->player->isPlaying())
   {
       this->player->stop();
   }
}
// ------------------------------------------------------------