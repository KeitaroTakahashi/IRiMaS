//
//  AnnotationTextEventComponent.h
//  IRiMaS
//
//  Created by Keitaro on 12/12/2019.
//

#ifndef AnnotationTextEventComponent_h
#define AnnotationTextEventComponent_h

#include "VideoAnnotationEventComponent.hpp"

class AnnotationTextEventComponent : public VideoAnnotationEventComponent
{
public:
    AnnotationTextEventComponent(IRStr *str, int videoLengthInSecond = 0) : VideoAnnotationEventComponent(str, videoLengthInSecond)
    {
        setType(VideoAnnotationEventComponent::TEXT);
        
        addAndMakeVisible(&this->timeCodeUI);
        this->timeCodeUI.addMouseListener(this, true);
        this->timeCodeUI.timeCodeChangedCallback = [this]{timeCodeChanged();};
        addAndMakeVisible(&this->textContents);
        this->textContents.setText("text...", dontSendNotification);
        this->textContents.setColour(Label::outlineColourId, Colour(255,255,255));
        this->textContents.addMouseListener(this, true);
        this->textContents.setEditable(true);
        this->textContents.onTextChange = [this]{ textContentsChanged(); };
    }
    
    AnnotationTextEventComponent(IRStr* str,
    std::string beginTime,
    std::string endTime,
    std::string contents,
    int videoLengthInSecond = 0) :
    VideoAnnotationEventComponent(str, videoLengthInSecond)
    {
        setType(VideoAnnotationEventComponent::TEXT);
        
        addAndMakeVisible(&this->timeCodeUI);
        this->timeCodeUI.addMouseListener(this, true);
        this->timeCodeUI.timeCodeChangedCallback = [this]{timeCodeChanged();};
        this->timeCodeUI.setBeginTime(beginTime);
        this->timeCodeUI.setEndTime(endTime);
        addAndMakeVisible(&this->textContents);
        this->textContents.setText(contents, dontSendNotification);
        this->textContents.setColour(Label::outlineColourId, Colour(255,255,255));
        this->textContents.addMouseListener(this, true);
        this->textContents.setEditable(true);
        this->textContents.onTextChange = [this]{ textContentsChanged(); };
        
        timeCodeChanged();
    }
    
    ~AnnotationTextEventComponent()
    {
        
    }
    // ==================================================
    void paint(Graphics& g ) override
    {
        
        VideoAnnotationEventComponent::paint(g);
        
        
    }
    
    void resized() override
    {
        VideoAnnotationEventComponent::resized();
        
        this->timeCodeUI.setBounds(0, 0, 226, getHeight());
        this->textContents.setBounds(230, 5, getWidth() - 200 - 100, 30);
    }
    // ==================================================
    
    // set sort value for ascending sort
    void timeCodeChanged()
    {
        setSortValue(this->timeCodeUI.getBeginTimeCode());
        
        eventModified();
    }
    
    void textContentsChanged()
    {
        eventModified();
    }
    // ==================================================
    srtWriter::SRT_STRUCT getSRT() override
    {
        srtWriter::SRT_STRUCT srt = srtWriter::SRT_STRUCT(getBeginTimeInString(),
                                                          getEndTimeInString(),
                                                          getTextContents());
        
        return srt;
    }

    std::string getTextContents()
    {
        return this->textContents.getText().toStdString();
    }
    
    // ==================================================
    // ==================================================

private:
    // ==================================================
    
    // ==================================================
    Label textContents;
    // ==================================================
    
    void setVideoLength(int videoLengthInSecond) override
    {
        VideoAnnotationEventComponent::setVideoLength(videoLengthInSecond);
    }
    // ==================================================

    
    
};
#endif /* AnnotationTextEventComponent_h */
