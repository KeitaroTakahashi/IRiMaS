//
//  VideoAnnotationMenuComponent.h
//  IRiMaS
//
//  Created by Keitaro on 10/12/2019.
//

#ifndef VideoAnnotationMenuComponent_h
#define VideoAnnotationMenuComponent_h
#include "IRStrComponent.hpp"
#include "IRImageButton.hpp"

class VideoAnnotationMenuComponent : public Component,
public IRStrComponent,
public ChangeBroadcaster
{
public:
    
    enum VideoAnnotationMenuComponentStatus
    {
        CreateTextEvent,
        CreateImageEvent,
        CreateShapeEvent,
        CreateAudioEvent,
        NONE
    };
    
    
    VideoAnnotationMenuComponent(IRStr* str) : IRStrComponent(str)
    {
        createButton(&this->TextEventButton, getStr()->ICONBANK->icon_TextEvent);
        this->TextEventButton.setDrawCircle(false);
        this->TextEventButton.setDrawRoundedSquare(true);
        this->TextEventButton.onClick = [this]{ textEventButtonClicked(); };
        
        createButton(&this->ImageEventButton, getStr()->ICONBANK->icon_ImageEvent);
        this->ImageEventButton.setDrawCircle(false);
        this->ImageEventButton.setDrawRoundedSquare(true);
        this->ImageEventButton.onClick = [this]{ imageEventButtonClicked(); };
        
        createButton(&this->ShapeEventButton, getStr()->ICONBANK->icon_ShapeEvent);
        this->ShapeEventButton.setDrawCircle(false);
        this->ShapeEventButton.setDrawRoundedSquare(true);
        this->ShapeEventButton.onClick = [this]{ ShapeEventButtonClicked(); };
        
        createButton(&this->AudioEventButton, getStr()->ICONBANK->icon_AudioEvent);
        this->AudioEventButton.setDrawCircle(false);
        this->AudioEventButton.setDrawRoundedSquare(true);
        this->AudioEventButton.onClick = [this]{ AudioEventButtonClicked(); };

    }
    
    ~VideoAnnotationMenuComponent()
    {
        
    }
    // ==================================================

    void paint(Graphics& g) override
    {
        g.fillAll(Colour::fromFloatRGBA(0.4,0.4,0.4,0.4));
    }
    
    void resized() override
    {
        int centreW = getWidth() / 2;
        int centreH = getHeight() / 2;
        int margin = 5;
        float h = 100;
        juce::Point<int> b = juce::Point<int> (((float)h / this->TextEventButton.getAspectRatio()), h);

        this->TextEventButton.setBounds(0, 0, b.getX(), b.getY());
        this->TextEventButton.setCentrePosition(centreW - b.getX()/2 - margin,
                                                centreH - b.getY()/2 - margin);
        
        this->ShapeEventButton.setBounds(0, 0, b.getX(), b.getY());
        this->ShapeEventButton.setCentrePosition(centreW + b.getX()/2 + margin,
                                                 centreH - b.getY()/2 - margin);
        
        this->ImageEventButton.setBounds(0, 0, b.getX(), b.getY());
        this->ImageEventButton.setCentrePosition(centreW - b.getX()/2 - margin,
                                                centreH + b.getY()/2 + margin);
        
        this->AudioEventButton.setBounds(0, 0, b.getX(), b.getY());
        this->AudioEventButton.setCentrePosition(centreW + b.getX()/2 + margin,
                                                 centreH + b.getY()/2 + margin);
    }
    // ==================================================
    
    std::function<void()>closeCallback;
    // ==================================================
    
    VideoAnnotationMenuComponentStatus getStatus() const { return this->status; }

private:
    
    void createButton(IRImageButton* button, IRIconBank::IRIconImage img)
    {
        if(getStr()->SYSTEMCOLOUR.isWhiteBased)
            button->setImage(img.black);
        else  button->setImage(img.white);
        addAndMakeVisible(button);
    }
    // ==================================================
    
   

    void textEventButtonClicked()
    {
        this->status = CreateTextEvent;
        
        std::cout << "textEventButtonClicked\n";
        sendChangeMessage();
        
    }
    
    void imageEventButtonClicked()
    {
        this->status = CreateImageEvent;
        sendChangeMessage();
    }
    
    void ShapeEventButtonClicked()
    {
        this->status = CreateShapeEvent;
        sendChangeMessage();
    }
    
    void AudioEventButtonClicked()
    {
        this->status = CreateAudioEvent;
        sendChangeMessage();
    }
    
    // ==================================================

    IRImageButton TextEventButton;
    IRImageButton ImageEventButton;
    IRImageButton ShapeEventButton;
    IRImageButton AudioEventButton;
    // ==================================================
    
    VideoAnnotationMenuComponentStatus status = NONE;
    // ==================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoAnnotationMenuComponent)

    
};
#endif /* VideoAnnotationMenuComponent_h */
