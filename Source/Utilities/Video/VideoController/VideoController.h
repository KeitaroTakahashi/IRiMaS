//
//  VideoController.h
//  IRiMaS
//
//  Created by Keitaro on 24/10/2019.
//

#ifndef VideoController_h
#define VideoController_h

#include "JuceHeader.h"
class VideoController : public Component,
public IRStrComponent,
public ChangeBroadcaster,
public ChangeListener
{
public:
    
    enum VideoControllerStatus
    {
        OpenMovieFile,
        WidthChange,
        HeightChange,
        W_H_Sync,
        NONE
    };
    
    VideoController(IRStr * str) : IRStrComponent(str)
    {
        addAndMakeVisible(&this->openMovieButton);
        this->openMovieButton.setButtonText("Open Movie File");
        this->openMovieButton.onClick = [this]{ OpenMovieFileAction(); };
    }
    
    ~VideoController()
    {
        
    }
    
    void resized() override
    {
        int y = 0;
       // int yIncrement = 30;
        //int yBigIncrement = 30;
        
        int menuWidth = getWidth() - 20;
        
        this->openMovieButton.setBounds(10, y, menuWidth, 30);
    }
    
    void paint(Graphics& g) override
    {
        
    }
    
    void OpenMovieFileAction()
    {
        
        std::cout << "OpenMovieFileAction\n";
        this->status = OpenMovieFile;
        sendChangeMessage();
    }
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        
    }

    VideoControllerStatus getStatus() const { return this->status; }

private:
    
    TextButton openMovieButton;
    
    VideoControllerStatus status = NONE;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoController)

};
#endif /* VideoController_h */
