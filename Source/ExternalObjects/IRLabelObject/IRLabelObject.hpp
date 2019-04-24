
#ifndef IRLabelObject_h
#define IRLabelObject_h

#include "IRNodeObject.hpp"
#include "IRLabelObjectPreference.hpp"




class IRLabelObject : public IRNodeObject,
                      public ChangeListener
{
    
public:
    
    IRLabelObject(Component* parent);
    ~IRLabelObject();
    
    IRNodeObject* copyThis() override;

    t_json saveThisToSaveData() override;
    void loadThisFromSaveData(t_json data) override;

    void paint(Graphics &g) override;
    void paintOnWorkspace(Graphics& g, Component* workspace) override;
    void resized() override;
    
    void mouseDownEvent(const MouseEvent& e) override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    void setFont(Font font);
    Font getFont() const;
    
    void setAlign(int newAlign);
    
    Label label;
    
    // preference
    IRLabelObjectPreference *preference;

private:
    // ------------------------------------------------------------
    // call back function automatically called when the status of this object changed by others.
    // write some tasks here
    void statusChangedCallback(IRNodeComponentStatus status) override;
    
    Font font;

    
};




#endif /* IRLabelObject_h */




