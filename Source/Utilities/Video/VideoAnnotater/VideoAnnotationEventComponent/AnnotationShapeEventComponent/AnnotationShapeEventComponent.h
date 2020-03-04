//
//  AnnotationShapeEventComponent.h
//  IRiMaS
//
//  Created by Keitaro on 12/12/2019.
//

#ifndef AnnotationShapeEventComponent_h
#define AnnotationShapeEventComponent_h

#include "VideoAnnotationEventComponent.h"

class AnnotationShapeEventComponent : public VideoAnnotationEventComponent
{
public:
    AnnotationShapeEventComponent(IRStr *str, IRVideoAnnotaterBase* base, int videoLengthInSecond = 0) :
    VideoAnnotationEventComponent(str, base, videoLengthInSecond)
    {
        setType(VideoAnnotationEventComponent::SHAPE);
        
        addAndMakeVisible(&this->timeCodeUI);
        this->timeCodeUI.addMouseListener(this, true);
        this->timeCodeUI.timeCodeChangedCallback = [this]{timeCodeChanged();};

    }
    
    ~AnnotationShapeEventComponent()
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
        
        int h = 40;
        int margin = 5;
        
        this->timeCodeUI.setBounds(0, 0, 226, h);
    }
    // ==================================================
    srtWriter::SRT_STRUCT getSRT() override
    {
        return srtWriter::SRT_STRUCT();
    }
    // ==================================================
    
    // set sort value for ascending sort
    void timeCodeChanged()
    {
        setSortValue(this->timeCodeUI.getBeginTimeCode());
        
        eventModified();
    }
    
    // ==================================================
    /*
    srtWriter::SRT_STRUCT getSRT() override
    {
        srtWriter::SRT_STRUCT srt = srtWriter::SRT_STRUCT(getBeginTimeInString(),
                                                          getEndTimeInString(),
                                                          getTextContents());
        
        return srt;
    }*/
    // ==================================================
    
    void setVideoLength(int videoLengthInSecond) override
    {
        VideoAnnotationEventComponent::setVideoLength(videoLengthInSecond);
    }

private:
    // ==================================================
    // ==================================================
    // ==================================================
    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnnotationShapeEventComponent)

    
};
#endif /* AnnotationShapeEventComponent_h */
