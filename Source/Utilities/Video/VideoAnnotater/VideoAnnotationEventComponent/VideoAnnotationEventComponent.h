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
        
        std::cout << "VideoAnnotationEventComponent\n";
        this->activeButton.setImage(getStr()->ICONBANK->icon_active.white);
        this->activeButton.setDrawCircle(true);
        this->activeButton.setDrawRoundedSquare(false);
        addAndMakeVisible(&this->activeButton);
        this->activeButton.onClick = [this]{ changeActive(); };
        this->activeButton.addMouseListener(this, true);
        
        this->searchButton.setImage(getStr()->ICONBANK->icon_search.white);
        this->searchButton.setDrawCircle(true);
        this->searchButton.setDrawRoundedSquare(false);
        addAndMakeVisible(&this->searchButton);
        this->searchButton.onClick = [this]{ searchButtonPressed(); };
        this->searchButton.addMouseListener(this, true);

        this->timeCodeUI.addMouseListener(this, true);
        this->timeCodeUI.timeCodeChangedCallback = [this]{timeCodeChanged();};
        std::cout << "end\n";

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
        int x = getWidth() - margin*2 - buttonSize;
        this->activeButton.setBounds(x, margin,
                                     buttonSize, buttonSize);
        
        x = x - buttonSize - margin*2;
        this->searchButton.setBounds(x, margin,
                                     buttonSize, buttonSize);
    }
    // ==================================================
    // copy this event component
    virtual VideoAnnotationEventComponent* copyThis() {};
    
    
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
            this->activeButton.setImage(getStr()->ICONBANK->icon_active.gray);
            this->activeButton.setDrawColour(Colour(100, 100, 100));
            
            // deactivate animation
            //auto obj = getNodeObject();
            //if(obj != nullptr) obj->setAnimationActive(false);
            
        }else{
            setActive(true);
            this->activeButton.setImage(getStr()->ICONBANK->icon_active.white);
            this->activeButton.setDrawColour(Colour(255, 255, 255));
            
            // activate animation 
            //auto obj = getNodeObject();
            //if(obj != nullptr) obj->setAnimationActive(true);

        }
        callEventActiveChanged();
    }
    bool isActive() const { return this->isActiveFlag; }
    // ==================================================
    
    void searchButtonPressed()
    {
        // delegate
        showEventPosition(this);
    }
    
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
    virtual srtWriter::SRT_STRUCT getSRT() { return srtWriter::SRT_STRUCT(); };
    virtual srtWriter::IRSRT_STRUCT getIRSRT(){ return srtWriter::IRSRT_STRUCT(); };
    
    void setNodeObject(IRNodeObject* obj)
    {
        this->nodeObj = obj;
        updateNodeObjTimeCode();
        nodeObjectSetAction(obj);
        
    }
    IRNodeObject* getNodeObject() { return this->nodeObj; }
    virtual void nodeObjectSetAction(IRNodeObject* obj) {};
    
    // ==================================================
    // set sort value for ascending sort
    void timeCodeChanged()
    {
        setSortValue(this->timeCodeUI.getBeginTimeCode());
        updateNodeObjTimeCode();
        eventModified();
    }
    
    void updateNodeObjTimeCode()
    {
        if(this->nodeObj != nullptr)
        {            
            this->nodeObj->setStartTimeSec( getBeginTimeCode() );
            this->nodeObj->setEndTimeSec( getEndTimeCode() );
        }
    }
    
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
        
        //virtual void showEventPosition(VideoAnnotationEventComponent* comp) = 0;

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
    
    /*
    void callShowEventPosition()
    {
        Component::BailOutChecker checker(this);
        //==========
        // check if the objects are not deleted, if deleted, return
        if(checker.shouldBailOut()) return;
        this->listeners.callChecked(checker, [this](Listener& l){ l.showEventPosition(this); });
        //check again
        if(checker.shouldBailOut()) return;
    }*/
    

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
    IRImageButton searchButton;
    // ==================================================
    
    IRNodeObject* nodeObj = nullptr;
    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoAnnotationEventComponent)

    
};
#endif /* VideoAnnotationEventComponent_hpp */
