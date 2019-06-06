//
//  ISVParameterControlComponent.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 05/06/2019.
//

#ifndef ISVParameterControlComponent_hpp
#define ISVParameterControlComponent_hpp

#include "ISVParameterSliders.hpp"
#include "ISVParameterControlUI.hpp"

class ISVParameterControlComponent : public Component,
public ChangeBroadcaster
{
public:
    
    ISVParameterControlComponent();
    ~ISVParameterControlComponent();
    
    // ==================================================
    void setParameterUI(ISVParameterControlUI* ui);
    void addParameterUI();
    void removeParameterUI();
    // ==================================================
    // ==================================================
    // ==================================================

private:
    
    void paint(Graphics& g) override;
    void resized() override;
    
    // ==================================================

    ISVParameterControlUI* ui;
    // ==================================================

    // ==================================================

    
};
#endif /* ISVParameterControlComponent_hpp */
