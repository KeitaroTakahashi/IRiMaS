//
//  AnnotationTextEventComponent.h
//  IRiMaS
//
//  Created by Keitaro on 12/12/2019.
//

#ifndef AnnotationTextEventComponent_h
#define AnnotationTextEventComponent_h

#include "VideoAnnotationEventComponent.h"
#include "FontController.h"
#include "IRTextEditorObject.hpp"

class AnnotationTextEventComponent : public VideoAnnotationEventComponent
{
public:
    AnnotationTextEventComponent(IRStr *str, IRVideoAnnotaterBase* base, int videoLengthInSecond = 0) :
    VideoAnnotationEventComponent(str, base, videoLengthInSecond)
    {
        setType(VideoAnnotationEventComponent::TEXT);
        
        addAndMakeVisible(&this->timeCodeUI);
        
        createTextEditor();
        
    }
    
    AnnotationTextEventComponent(IRStr* str,
                                 IRVideoAnnotaterBase* base,
                                 float beginTime,
                                 float endTime,
                                 int videoLengthInSecond = 0) :
    VideoAnnotationEventComponent(str, base, videoLengthInSecond)

    {
        setType(VideoAnnotationEventComponent::TEXT);
        createTextEditor();
        
        addAndMakeVisible(&this->timeCodeUI);
        this->timeCodeUI.setBeginTime(beginTime);
        this->timeCodeUI.setEndTime(endTime);
        timeCodeChanged();
    }

    
    
    AnnotationTextEventComponent(IRStr* str,
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

        createTextEditor(contents);
      
        timeCodeChanged();
    }
    
    ~AnnotationTextEventComponent()
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

    AnnotationTextEventComponent* copyThis() override
    {
        auto event = new AnnotationTextEventComponent(getStr(),
                                                      getBase(),
                                                      getBeginTimeCode(),
                                                      getEndTimeCode());
        
        return event;
    }
    // ==================================================

    void createTextEditor(std::string contents = "")
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
        this->textEditor.setText(contents, dontSendNotification);
        this->textEditor.setColour(TextEditor::backgroundColourId, getStr()->SYSTEMCOLOUR.contents);
        this->textEditor.applyColourToAllText(getStr()->SYSTEMCOLOUR.text);
        
        this->textEditor.addMouseListener(this, true);
    }
    
    void nodeObjectSetAction(IRNodeObject* obj) override
    {
        this->textEditorObj = static_cast<IRTextEditorObject*>(obj);
        this->textEditorObj->onTextChange = [this] { onTextChangeAction(); };
    }
    
    void onTextChangeAction()
    {
        /*
        // insert only first 10 characters
        String text = this->textEditorObj->textEditor.getText().substring(0, 100);
        
        if(text.containsAnyOf ("\r\n"))
        {
            text = text.replaceCharacters("\r\n", "  ");
        }
        this->textEditor.setText(text);*/
    }
    // ==================================================
    
    
    
    // ==================================================
    srtWriter::SRT_STRUCT getSRT() override
    {
        
        std::cout << "textAnnotation : getSRT " << getTextContents() << std::endl;
        srtWriter::SRT_STRUCT srt = srtWriter::SRT_STRUCT(getBeginTimeInString(),
                                                          getEndTimeInString(),
                                                          getTextContents());
        
        return srt;
    }

    std::string getTextContents()
    {
        return this->textEditor.getText().toStdString();
    }
    
    // ==================================================
    
  
    // ==================================================

private:
    // ==================================================
    
    // ==================================================
    
    IRImageButton TextSettingButton;
    
    TextEditor textEditor;
    IRTextEditorObject* textEditorObj = nullptr;

    // ==================================================
    
    void setVideoLength(int videoLengthInSecond) override
    {
        VideoAnnotationEventComponent::setVideoLength(videoLengthInSecond);
    }
    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnnotationTextEventComponent)

    
};
#endif /* AnnotationTextEventComponent_h */
