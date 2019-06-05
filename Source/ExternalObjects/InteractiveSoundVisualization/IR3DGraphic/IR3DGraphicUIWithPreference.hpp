//
//  IR3DGraphicUIWithPreference.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 01/06/2019.
//

#ifndef IR3DGraphicUIWithPreference_hpp
#define IR3DGraphicUIWithPreference_hpp

#include "IR3DGraphicUI.hpp"
#include "IRISVObjectPreference.h"

class IR3DGraphicUIWithPreference : public IR3DGraphicUI,
private ChangeListener,
private ISVPreferenceUI::Listener
{
public:
    IR3DGraphicUIWithPreference(IRNodeObject*nodeObject);
    ~IR3DGraphicUIWithPreference();
    // ==================================================

    IRISVObjectPreference * getPreference() const { return this->preference.get(); }
    // ==================================================

    void resized() override;
    // ==================================================

private:
    
    // ==================================================
    void intensitySliderChanged(ISVPreferenceUI* obj) override;
    void finenessSliderChanged(ISVPreferenceUI* obj) override;
    void lightingColourChanged(ISVPreferenceUI* obj) override;
    void materialColourChanged(ISVPreferenceUI* obj) override;
    
    void stretchValueChanged(ISVPreferenceUI* obj) override;
    // ==================================================

    
    void changeListenerCallback(ChangeBroadcaster* source) override;

    // ==================================================

    std::shared_ptr<IRISVObjectPreference> preference;
    ISVParameterControlUI controlUI;
    
};
#endif /* IR3DGraphicUIWithPreference_hpp */
