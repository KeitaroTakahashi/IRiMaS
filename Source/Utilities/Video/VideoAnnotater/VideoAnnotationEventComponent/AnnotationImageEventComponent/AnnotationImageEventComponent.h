//
//  AnnotationImageEventComponent.h
//  IRiMaS
//
//  Created by Keitaro on 12/12/2019.
//

#ifndef AnnotationImageEventComponent_h
#define AnnotationImageEventComponent_h

#include "VideoAnnotationEventComponent.h"
#include "IRImageViewerObject.hpp"

class AnnotationImageEventComponent : public VideoAnnotationEventComponent
{
public:
    AnnotationImageEventComponent(IRStr *str, IRVideoAnnotaterBase* base, int videoLengthInSecond = 0) :
    VideoAnnotationEventComponent(str, base, videoLengthInSecond)
    {
        setType(VideoAnnotationEventComponent::TEXT);
        
        addAndMakeVisible(&this->timeCodeUI);
        
        createTextEditor();
        
    }
    
    AnnotationImageEventComponent(IRStr* str,
                                 IRVideoAnnotaterBase* base,
                                 float beginTime,
                                 float endTime,
                                 int videoLengthInSecond = 0) :
    VideoAnnotationEventComponent(str, base, videoLengthInSecond)

    {
        setType(VideoAnnotationEventComponent::TEXT);
        
        addAndMakeVisible(&this->timeCodeUI);
        this->timeCodeUI.setBeginTime(beginTime);
        this->timeCodeUI.setEndTime(endTime);

        createTextEditor();
        
        timeCodeChanged();
    }

    
    
    AnnotationImageEventComponent(IRStr* str,
                                 IRVideoAnnotaterBase* base,
                                std::string beginTime,
                                std::string endTime,
                                std::string contents,
                                int videoLengthInSecond = 0) :
    VideoAnnotationEventComponent(str, base, videoLengthInSecond)
    {
        setType(VideoAnnotationEventComponent::TEXT);
        
        addAndMakeVisible(&this->timeCodeUI);
        this->timeCodeUI.setBeginTime(beginTime);
        this->timeCodeUI.setEndTime(endTime);

        createTextEditor();
      
        timeCodeChanged();
    }
    
    ~AnnotationImageEventComponent()
    {
    }
    // ==================================================
    void paint(Graphics& g ) override
    {
        
        VideoAnnotationEventComponent::paint(g);
        
        //g.fillAll(getStr()->SYSTEMCOLOUR.fundamental.withAlpha(0.7f));
        
    }
    
    void resized() override
    {
        int h = 40;
        int margin = 5;
        VideoAnnotationEventComponent::resized();
        
        this->timeCodeUI.setBounds(0, 0, 226, h);
        this->textEditor.setBounds(230, margin,
                                   getWidth() - 200 -120,
                                   getHeight() - margin*2);
        
        this->textEditor.setInputRestrictions(this->textEditor.getWidth() / 12);

        int buttonSize = h - margin*2;
        this->TextSettingButton.setBounds(getWidth() - margin*3 - buttonSize * 2, margin, buttonSize, buttonSize);
    }
    // ==================================================
    AnnotationImageEventComponent* copyThis() override
    {
        auto event = new AnnotationImageEventComponent(getStr(),
                                                       getBase(),
                                                       getBeginTimeCode(),
                                                       getEndTimeCode());
        
        return event;
    }
    // ==================================================
    void createTextEditor()
    {
        //set default bounds adjusted to the video size in pixel
        auto videoSize = this->getBase()->getVideoSize();
        auto rect = Rectangle<int> (0, 0,
                                    videoSize.getX(),
                                    videoSize.getY());
        rect.setHeight(rect.getHeight() * 0.2);
        if(rect.getHeight() < 50) rect.setHeight(50);
        
        addAndMakeVisible(this->textEditor);
        // here initialy in Edit mode
        
        this->textEditor.setMultiLine(false);
        this->textEditor.setCaretVisible(false);
        this->textEditor.setReadOnly(true);
        this->textEditor.setColour(TextEditor::backgroundColourId, getStr()->SYSTEMCOLOUR.contents);
        this->textEditor.applyColourToAllText(getStr()->SYSTEMCOLOUR.text);
        
        this->textEditor.addMouseListener(this, true);
    }
    
    void nodeObjectSetAction(IRNodeObject* obj) override
    {
        this->imageViewerObj = static_cast<IRImageViewerObject*>(obj);
    }
    
  
    // ==================================================
    
    
    
    // ==================================================
    srtWriter::SRT_STRUCT getSRT() override
    {
        srtWriter::SRT_STRUCT srt = srtWriter::SRT_STRUCT(getBeginTimeInString(),
                                                          getEndTimeInString(),
                                                          "");
        
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
    
    IRImageButton TextSettingButton;
    
    TextEditor textEditor;
    IRImageViewerObject* imageViewerObj = nullptr;

    // ==================================================
    
    void setVideoLength(int videoLengthInSecond) override
    {
        VideoAnnotationEventComponent::setVideoLength(videoLengthInSecond);
    }
    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnnotationImageEventComponent)

    
};
#endif /* AnnotationImageEventComponent_h */
