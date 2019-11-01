
#ifndef IRImageViwerObject_h
#define IRImageViwerObject_h

#include "IRImageViewer.hpp"
#include "IRNodeObject.hpp"
#include "IRImageViewerController.hpp"

class IRImageViewerObject : public IRNodeObject,
private ChangeListener
{
    
public:
    
    IRImageViewerObject(Component* parent, IRStr* str);
    ~IRImageViewerObject();
    
    // copy constructor
    IRNodeObject* copyThis() override;
    
    t_json saveThisToSaveData() override;
    void loadThisFromSaveData(t_json saveData) override;
    
    void resized() override;
    void paint(Graphics& g) override;
    
    void resizeThisComponentEvent(const MouseEvent& e) override;
    
    std::unique_ptr<IRImageViewerController> controller;

private:
    
    // ------------------------------------------------------------
    // call back function automatically called when the status of this object changed by others.
    // write some tasks here
    void statusChangedCallback(IRNodeComponentStatus status) override;
    void changeListenerCallback (ChangeBroadcaster* source) override;

    std::shared_ptr<IRImageViewer> imageViewer { nullptr };
    
    
};

#endif /* IRImageViwerObject_h */
