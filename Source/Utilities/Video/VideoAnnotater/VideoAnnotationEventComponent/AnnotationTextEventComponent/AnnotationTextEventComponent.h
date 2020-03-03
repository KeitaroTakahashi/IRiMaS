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
        this->timeCodeUI.addMouseListener(this, true);
        this->timeCodeUI.timeCodeChangedCallback = [this]{timeCodeChanged();};

        createTextEditor();
        
        this->TextSettingButton.setImage(getStr()->ICONBANK.icon_text.white);
        this->TextSettingButton.addMouseListener(this, true);
        this->TextSettingButton.setDrawCircle(true);
        this->TextSettingButton.setDrawRoundedSquare(false);
        addAndMakeVisible(&this->TextSettingButton);
        this->TextSettingButton.onClick = [this]{ TextSettingButtonClickedAction(); };
    }
    
    AnnotationTextEventComponent(IRStr* str,
                                 IRVideoAnnotaterBase* base,
                                 float beginTime,
                                 float endTime,
                                 int videoLengthInSecond = 0) :
    VideoAnnotationEventComponent(str, base, videoLengthInSecond)

    {
        setType(VideoAnnotationEventComponent::TEXT);
        
        addAndMakeVisible(&this->timeCodeUI);
        this->timeCodeUI.addMouseListener(this, true);
        this->timeCodeUI.timeCodeChangedCallback = [this]{timeCodeChanged();};
        this->timeCodeUI.setBeginTime(beginTime);
        this->timeCodeUI.setEndTime(endTime);

        createTextEditor();
        
        this->TextSettingButton.setImage(getStr()->ICONBANK.icon_text.white);
        this->TextSettingButton.setDrawCircle(true);
        this->TextSettingButton.setDrawRoundedSquare(false);
        addAndMakeVisible(&this->TextSettingButton);
        this->TextSettingButton.onClick = [this]{ TextSettingButtonClickedAction(); };
        
        
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
        this->timeCodeUI.addMouseListener(this, true);
        this->timeCodeUI.timeCodeChangedCallback = [this]{timeCodeChanged();};
        this->timeCodeUI.setBeginTime(beginTime);
        this->timeCodeUI.setEndTime(endTime);

        createTextEditor();
        
        this->TextSettingButton.setImage(getStr()->ICONBANK.icon_text.white);
        this->TextSettingButton.setDrawCircle(true);
        this->TextSettingButton.setDrawRoundedSquare(false);
        addAndMakeVisible(&this->TextSettingButton);
        this->TextSettingButton.onClick = [this]{ TextSettingButtonClickedAction(); };
        
        
        timeCodeChanged();
    }
    
    ~AnnotationTextEventComponent()
    {
        this->textEditor.reset();
    }
    // ==================================================
    void paint(Graphics& g ) override
    {
        
        VideoAnnotationEventComponent::paint(g);
        
        g.fillAll(getStr()->SYSTEMCOLOUR.fundamental.withAlpha(0.7f));
        
    }
    
    void resized() override
    {
        int h = 40;
        int margin = 5;
        VideoAnnotationEventComponent::resized();
        
        this->timeCodeUI.setBounds(0, 0, 226, h);
        //this->textContents.setBounds(230, 5, getWidth() - 200 - 120, 30);
        this->textEditor->setObjectBounds(230, margin,
                                          getWidth() - 200 -120,
                                          getHeight() - margin*2);
        
        int buttonSize = h - margin*2;
        this->TextSettingButton.setBounds(getWidth() - margin*3 - buttonSize * 2, margin, buttonSize, buttonSize);
    }
    // ==================================================

    void createTextEditor()
    {
        std::cout << "createTextEditor\n";
        this->textEditor.reset( new IRTextEditorObject(this, getStr()) );
        
        //set default bounds adjusted to the video size in pixel
        auto videoSize = this->getBase()->getVideoSize();
        auto rect = Rectangle<int> (0, 0,
                                    videoSize.getX(),
                                    videoSize.getY());
        rect.setHeight(rect.getHeight() * 0.2);
        if(rect.getHeight() < 50) rect.setHeight(50);
        this->textEditor->controller->getArrangeController()->setRectangle(rect);
        
        
        addAndMakeVisible(this->textEditor.get());
        this->textEditor->addMouseListener(this,true);
        // here initialy in Edit mode
        this->textEditor->setEditMode(false);
        this->textEditor->onTextChange = [this]{ onTextChange(); };
        this->textEditor->onReturnKey  = [this]{ onReturnKey(); };
        
        this->textEditor->setBackgroundColour(getStr()->SYSTEMCOLOUR.fundamental.withAlpha(0.7f));
        this->textEditor->applyColourToAllText(Colours::white);
    }
    // ==================================================
    
    // set sort value for ascending sort
    void timeCodeChanged()
    {
        setSortValue(this->timeCodeUI.getBeginTimeCode());
        
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
    
    IRTextEditorObject* getIRTextEditorObject()
    {
        return this->textEditor.get();
    }
    // ==================================================
    
    void onTextChange()
    {
        int margin = 5;
        int h = this->textEditor->getTextHeight() + margin*4;
        setSize(getWidth(), h);
        
        setInitHeight(h);

        //eventModified();

        // IRVideoAnnotaterDelegate calls eventModifiedAction method in IRVidenoAnnotaterBase
        eventModifiedAction(this);
    }
    
    void onReturnKey()
    {
        
    }
    // ==================================================

private:
    // ==================================================
    
    // ==================================================
    Label textContents;
    
    IRImageButton TextSettingButton;
    
    std::shared_ptr<IRTextEditorObject> textEditor;
    
    void TextSettingButtonClickedAction()
    {
        selectedAction();
    }

    // ==================================================
    
    void setVideoLength(int videoLengthInSecond) override
    {
        VideoAnnotationEventComponent::setVideoLength(videoLengthInSecond);
    }
    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnnotationTextEventComponent)

    
};
#endif /* AnnotationTextEventComponent_h */
