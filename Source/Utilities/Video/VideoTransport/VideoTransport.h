//
//  VideoTransport.h
//  IRiMaS
//
//  Created by Keitaro on 08/12/2019.
//

#ifndef VideoTransport_h
#define VideoTransport_h

#include "JuceHeader.h"
#include "IRImageButton.hpp"

class IRVideoTransport : public Component,
public IRStrComponent,
public ChangeBroadcaster
{
public:
    
    enum IRVideoTransportStatus
    {
        OpenVideoFile,
        addEventButtonClicked,
        deleteEventButtonClicked,
        NONE
    };
    
    IRVideoTransport(IRStr* str) : IRStrComponent(str)
    {
        addAndMakeVisible(&this->openVideoButton);
        this->openVideoButton.onClick = [this]{ openVideoButtonClicked(); };
        this->openVideoButton.setButtonText("Open Video");
        
        createButton(&this->addEventButton, getStr()->ICONBANK.icon_newSlide);
        this->addEventButton.setDrawCircle(false);
        this->addEventButton.setDrawRoundedSquare(true);
        this->addEventButton.onClick = [this]{ addEventButtonClickedAction(); };
        
        createButton(&this->deleteEventButton, getStr()->ICONBANK.icon_deleteSlide);
        this->deleteEventButton.setDrawCircle(false);
        this->deleteEventButton.setDrawRoundedSquare(true);
        this->deleteEventButton.onClick = [this]{ deleteEventButtonClickedAction(); };


    }
    
    ~IRVideoTransport()
    {
        
    }
    
    void paint(Graphics& g) override
    {
        g.fillAll(getStr()->SYSTEMCOLOUR.contents);
    }
    
    void resized() override
    {
        int h = 30;
        
        this->openVideoButton.setBounds(getWidth() - 110, 5, 100, 30);
        
        
        Point<int> b = Point<int> (((float)h / this->addEventButton.getAspectRatio()), h);

        this->addEventButton.setBounds(5, 5, b.getX(), b.getY());
        this->deleteEventButton.setBounds(10 + b.getX(), 5, b.getX(), b.getY());

    }
    
    // ==================================================
    
    void addEventButtonClickedAction()
    {
        this->status = addEventButtonClicked;
        sendChangeMessage();
    }
    
    void deleteEventButtonClickedAction()
    {
        this->status = deleteEventButtonClicked;
        sendChangeMessage();
    }
    // ==================================================

    void openVideoButtonClicked()
    {
        this->status = OpenVideoFile;
        sendChangeMessage();
    }
    // ==================================================
    
    void closeEventMenu()
    {
    }
    // ==================================================
    // ==================================================
    IRVideoTransportStatus getStatus() const { return this->status; }
    
private:
    // ==================================================
    IRVideoTransportStatus status = NONE;
    
    // ==================================================
    void createButton(IRImageButton* button, IRIconBank::IRIconImage img)
    {
        if(getStr()->SYSTEMCOLOUR.isWhiteBased)
            button->setImage(img.black);
        else  button->setImage(img.white);
        addAndMakeVisible(button);
    }
    // ==================================================

    TextButton openVideoButton;
    IRImageButton addEventButton;
    IRImageButton deleteEventButton;

};

#endif /* VideoTransport_h */
