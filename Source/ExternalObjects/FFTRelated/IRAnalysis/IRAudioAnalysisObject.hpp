//
//  IRAudioAnalysisObject.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 28/04/2019.
//

#ifndef IRAudioAnalysisObject_hpp
#define IRAudioAnalysisObject_hpp
#include "IRNodeObject.hpp"
#include "IRAnalysisUI.hpp"

class IRAudioAnalysisObject : public IRNodeObject,
private IRNodeObject::Listener,
private ChangeListener
{
public:
    IRAudioAnalysisObject(Component *parent);
    ~IRAudioAnalysisObject();
    
    IRNodeObject* copyThis() override; // copy constructor
    IRNodeObject* copyContents(IRNodeObject* object) override;
    IRNodeObject* copyDragDropContents(IRNodeObject* object) override;
    
    t_json saveThisToSaveData() override;
    void loadThisFromSaveData(t_json data) override;
    
    void resized() override;
    void paint(Graphics& g) override;
    
    void statusChangedCallback(IRNodeComponentStatus status) override;
    
    // events
    void mouseDownEvent(const MouseEvent& e) override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    int getXMargin() const;
    int getYMargin() const;
    
    
private:
    
    void analaysisCompleted();
    
    std::shared_ptr<IRAnalysisUI> analysisUI;
    
    int xMargin = 5;
    int yMargin = 5;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRAudioAnalysisObject)

};

#endif /* IRAudioAnalysisObject_hpp */
