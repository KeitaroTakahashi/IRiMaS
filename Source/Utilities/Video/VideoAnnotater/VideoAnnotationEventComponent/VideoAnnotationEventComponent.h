//
//  VideoAnnotationEventComponent.h
//  IRiMaS
//
//  Created by Keitaro on 11/12/2019.
//

#ifndef VideoAnnotationEventComponent_hpp
#define VideoAnnotationEventComponent_hpp

#include "IRStrComponent.hpp"
#include "IRVideoAnnotaterDelegate.h"
#include "IRImageButton.hpp"
#include "TimeCodeInput.h"
#include "srtWriter.h"
#include "srtLoader.h"


class VideoAnnotationEventComponent : public Component,
public IRStrComponent,
public IRVideoAnnotaterDelegate
{
public:
    
    enum VideoAnnotationType
    {
        TEXT,
        SHAPE,
        IMAGE,
        AUDIO
    };
    
    VideoAnnotationEventComponent(IRStr* str, IRVideoAnnotaterBase* base, int videoLengthInSecond) :
    IRStrComponent(str),
    IRVideoAnnotaterDelegate(base),
    timeCodeUI(str, videoLengthInSecond),
    videoLengthInSecond(videoLengthInSecond)
    {
        this->activeButton.setImage(getStr()->ICONBANK.icon_active.white);
        this->activeButton.setDrawCircle(true);
        this->activeButton.setDrawRoundedSquare(false);
        addAndMakeVisible(&this->activeButton);
        this->activeButton.onClick = [this]{ changeActive(); };
    }
    

    ~VideoAnnotationEventComponent() {}
    
    // ==================================================
    // ==================================================
    virtual void paint(Graphics& g ) override
    {
        if(isSelected())
            g.fillAll(getStr()->SYSTEMCOLOUR.fundamental.brighter());
        else
            g.fillAll(getStr()->SYSTEMCOLOUR.fundamental);
        
    }
    
    virtual void resized() override
    {
        int margin = 5;
        int h = 40    ;
        int buttonSize = h - margin*2;
        this->activeButton.setBounds(getWidth() - margin*2 - buttonSize, margin,
                                     buttonSize, buttonSize);
    }
    // ==================================================
    
    void mouseDown(const MouseEvent& e) override
    {
        
        std::cout << "VideoAnnotationEventComponent mouseDown\n";
        selectedAction();
    }
    // ==================================================
    // when currently showed on the video player, then call this
    void isCurrentlyShownOnVideoPlayer(bool flag)
    {
        this->isShown = flag;
        if(this->isShown) callEventShownOnVideoPlayer();
        else callEventHiddenOnVideoPlayer();
    }
    std::function<void(bool)> shownOnVideoPlayerChanged;
    
    // ==================================================

    void selectedAction()
    {
        callEventComponentSelected();
    }
    
    void setSelected(bool flag) { this->isSelectedFlag = flag; repaint(); }
    bool isSelected() const { return this->isSelectedFlag; }
    void setActive(bool flag) { this->isActiveFlag = flag; }
    
    void changeActive()
    {
        if(isActive())
        {
            setActive(false);
            this->activeButton.setImage(getStr()->ICONBANK.icon_active.gray);
            this->activeButton.setDrawColour(Colour(100, 100, 100));
        }else{
            setActive(true);
            this->activeButton.setImage(getStr()->ICONBANK.icon_active.white);
            this->activeButton.setDrawColour(Colour(255, 255, 255));

        }
        callEventActiveChanged();
    }
    bool isActive() const { return this->isActiveFlag; }
    // ==================================================

    int getVideoLength() const { return this->videoLengthInSecond; }
    // set any functions when video length has been changed, but keep calling the original method
    virtual void setVideoLength(int videoLengthInSecond)
    {
        this->videoLengthInSecond = videoLengthInSecond;
    }
    // ==================================================

    void setType(VideoAnnotationType type) { this->type = type; }
    VideoAnnotationType getType() const { return this->type; }
    
    // get VideoAnnotation in SRT format (only text is relevant to the usual SRT and others are TIAALS original formats)
    virtual srtWriter::SRT_STRUCT getSRT() = 0;
    
    void setNodeObject(IRNodeObject* obj) { this->nodeObj = obj; }
    IRNodeObject* getNodeObject() { return this->nodeObj; }
    
    // ==================================================
    void eventDeactivated();
    void eventActivated();
    // ==================================================

    // SORT
    float sortVal1 = 0;
    
    void setSortValue(float sortVal)
    {
        this->sortVal1 = sortVal;
    }

    // ==================================================
    void eventModified()
    {
        callEventModified();
    }
    
    // ==================================================
    
    // ==================================================
    // LISTENER
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        virtual void eventComponentSelected(VideoAnnotationEventComponent* comp) = 0;
        virtual void eventModified(VideoAnnotationEventComponent* comp) = 0;
        
        virtual void eventActiveChanged(VideoAnnotationEventComponent* comp) = 0;
        
        virtual void eventShownOnVideoPlayer(VideoAnnotationEventComponent* comp) = 0;
        virtual void eventHiddenOnVideoPlayer(VideoAnnotationEventComponent* comp) = 0;

    };

    void addListener(Listener* newListener) { this->listeners.add(newListener); }
    void removeListener(Listener* listener) { this->listeners.remove(listener); }
    ListenerList<Listener> listeners;
    
    void callEventComponentSelected()
    {
        Component::BailOutChecker checker(this);
        //==========
        // check if the objects are not deleted, if deleted, return
        if(checker.shouldBailOut()) return;
        this->listeners.callChecked(checker, [this](Listener& l){ l.eventComponentSelected(this); });
        //check again
        if(checker.shouldBailOut()) return;
    }
    
    void callEventModified()
    {
        Component::BailOutChecker checker(this);
        //==========
        // check if the objects are not deleted, if deleted, return
        if(checker.shouldBailOut()) return;
        this->listeners.callChecked(checker, [this](Listener& l){ l.eventModified(this); });
        //check again
        if(checker.shouldBailOut()) return;
    }
    
    void callEventActiveChanged()
    {
        Component::BailOutChecker checker(this);
        //==========
        // check if the objects are not deleted, if deleted, return
        if(checker.shouldBailOut()) return;
        this->listeners.callChecked(checker, [this](Listener& l){ l.eventActiveChanged(this); });
        //check again
        if(checker.shouldBailOut()) return;
    }

    void callEventShownOnVideoPlayer()
    {
        Component::BailOutChecker checker(this);
        //==========
        // check if the objects are not deleted, if deleted, return
        if(checker.shouldBailOut()) return;
        this->listeners.callChecked(checker, [this](Listener& l){ l.eventShownOnVideoPlayer(this); });
        //check again
        if(checker.shouldBailOut()) return;
    }
    
    void callEventHiddenOnVideoPlayer()
    {
        Component::BailOutChecker checker(this);
        //==========
        // check if the objects are not deleted, if deleted, return
        if(checker.shouldBailOut()) return;
        this->listeners.callChecked(checker, [this](Listener& l){ l.eventHiddenOnVideoPlayer(this); });
        //check again
        if(checker.shouldBailOut()) return;
    }

    // ==================================================
    // ==================================================
    
    float getBeginTimeCode()
    {
        return this->timeCodeUI.getBeginTimeCode();
    }
    
    std::string getBeginTimeInString()
    {
        return this->timeCodeUI.getBeginTimeInString();
    }
    
    float getEndTimeCode()
       {
           return this->timeCodeUI.getEndTimeCode();
       }
    
    std::string getEndTimeInString()
    {
        return this->timeCodeUI.getEndTimeInString();
    }
    // ==================================================
    
    int getInitHeight() const { return this->init_height; }
    void setInitHeight(int newHeight) { this->init_height = newHeight; }
    // ==================================================
    
protected:
    TimeCodeInput timeCodeUI;

private:
    bool isSelectedFlag = false;
    bool isActiveFlag = true;
    bool isShown = false;
    
    
    int videoLengthInSecond;
    
    int init_height = 40;
    
    VideoAnnotationType type = TEXT;
    // ==================================================
    IRImageButton activeButton;
   
    // ==================================================
    
    IRNodeObject* nodeObj = nullptr;
    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoAnnotationEventComponent)

    
};
#endif /* VideoAnnotationEventComponent_hpp */