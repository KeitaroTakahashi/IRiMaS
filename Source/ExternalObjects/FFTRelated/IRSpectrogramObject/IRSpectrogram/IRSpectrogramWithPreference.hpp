//
//  IRSpectrogramWithPreference.hpp
//  IRiMaS
//
//  Created by Keitaro on 08/07/2019.
//

#ifndef IRSpectrogramWithPreference_hpp
#define IRSpectrogramWithPreference_hpp

#include "IRSpectrogramUI.hpp"
#include "IRSpectrogramObjectPreference.h"

class IRSpectrogramWithPreference : public IRSpectrogramUI
{
public:
    // ==================================================

    IRSpectrogramWithPreference(IRNodeObject* nodeObject, IRStr* str);
    ~IRSpectrogramWithPreference();
    
    IRSpectrogramObjectPreference* getPreference() const { return this->preference.get(); }
    
    // ==================================================

    
private:
    // ==================================================

    int gridSize = 30;
    
    
    // ==================================================
    void changeListenerCallback (ChangeBroadcaster* source) override;
    // ==================================================

    void updateAudioPreferenceUI();
    
    // ==================================================
    bool isFileImportCompletedFlag = false;
    // ==================================================

    std::shared_ptr<IRSpectrogramObjectPreference> preference;
    
    // ==================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRSpectrogramWithPreference)

    
};
#endif /* IRSpectrogramWithPreference_hpp */
