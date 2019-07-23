
#ifndef IRWaveformObject_hpp
#define IRWaveformObject_hpp

#include "IRNodeObject.hpp"
#include "IRWaveformObjectUI2.hpp"
#include "IRObjectSelection.hpp"

class IRWaveformObject : public IRNodeObject,
                                IRNodeObject::Listener,
                                private ChangeListener
{
    
public:
    
    IRWaveformObject(Component* parent);
    ~IRWaveformObject();
    
    
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

    /*
    bool keyPressed (const KeyPress& key,
                             Component* originatingComponent) override
    {
     
        IRNodeObject::keyPressed(key, originatingComponent);
        return true;
    }*/

    IRWaveformObjectUI2* waveform;

    
private:
    
    IRObjectSelection<Component* >* selector;
    bool selectFlag = true;
    bool isSelectMode = false;
    
    int xMargin = 0;
    int yMargin = 0;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRWaveformObject)
};




#endif /* IRWaveformObject_hpp */




