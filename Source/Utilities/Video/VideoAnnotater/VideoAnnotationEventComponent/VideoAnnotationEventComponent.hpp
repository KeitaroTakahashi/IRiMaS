//
//  VideoAnnotationEventComponent.hpp
//  IRiMaS
//
//  Created by Keitaro on 11/12/2019.
//

#ifndef VideoAnnotationEventComponent_hpp
#define VideoAnnotationEventComponent_hpp

#include "IRStrComponent.hpp"
#include "IRImageButton.hpp"
#include "TimeCodeInput.h"
#include "srtWriter.h"
#include "srtLoader.h"
class VideoAnnotationEventComponent : public Component,
public IRStrComponent
{
public:
    
    enum VideoAnnotationType
    {
        TEXT,
        SHAPE,
        IMAGE,
        AUDIO
    };
    
    VideoAnnotationEventComponent(IRStr* str, int videoLengthInSecond);
    

    ~VideoAnnotationEventComponent();
    
    // ==================================================
    // ==================================================
    virtual void paint(Graphics& g ) override;
    virtual void resized() override;
    // ==================================================
    
    void mouseDown(const MouseEvent& e) override;
    // ==================================================
    void selectedAction();
    void setSelected(bool flag) { this->isSelectedFlag = flag; repaint(); }
    bool isSelected() const { return this->isSelectedFlag; }
    void setActive(bool flag) { this->isActiveFlag = flag; }
    void changeActive();
    bool isActive() const { return this->isActiveFlag; }
    
    int getVideoLength() const { return this->videoLengthInSecond; }
    // set any functions when video length has been changed, but keep calling the original method
    virtual void setVideoLength(int videoLengthInSecond);
    
    void setType(VideoAnnotationType type) { this->type = type; }
    VideoAnnotationType getType() const { return this->type; }
    
    // get VideoAnnotation in SRT format (only text is relevant to the usual SRT and others are TIAALS original formats)
    virtual srtWriter::SRT_STRUCT getSRT() = 0;
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
    void eventModified();
    
    // ==================================================

    class Listener
    {
    public:
        virtual ~Listener() {}
        
        virtual void eventComponentSelected(VideoAnnotationEventComponent* comp) = 0;
        virtual void eventModified(VideoAnnotationEventComponent* comp) = 0;
        
        virtual void eventActiveChanged(VideoAnnotationEventComponent* comp) = 0;
    };

    void addListener(Listener* newListener) { this->listeners.add(newListener); }
    void removeListener(Listener* listener) { this->listeners.remove(listener); }
    ListenerList<Listener> listeners;
    
    void callEventComponentSelected();
    void callEventModified();
    void callEventActiveChanged();

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
    int videoLengthInSecond;
    
    int init_height = 40;
    
    VideoAnnotationType type = TEXT;
    // ==================================================
    IRImageButton activeButton;
   
    // ==================================================
    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VideoAnnotationEventComponent)

    
};
#endif /* VideoAnnotationEventComponent_hpp */
