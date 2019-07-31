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

class IRSpectrogramWithPreference : public IRSpectrogramUI,
private KeAnimationComponent
{
public:
    // ==================================================

    IRSpectrogramWithPreference(IRNodeObject* nodeObject);
    ~IRSpectrogramWithPreference();
    
    IRSpectrogramObjectPreference* getPreference() const { return this->preference.get(); }
    
    // ==================================================

    
private:
    // ==================================================

    int gridSize = 30;
    
    
    // ==================================================
    void changeListenerCallback (ChangeBroadcaster* source) override;
    // ==================================================

    void openAudioFileAction();
    void fileImportCompletedAction() override;
    void updateAudioPreferenceUI();
    
    void updateAnimationFrame() override;
    // ==================================================
    bool isFileImportCompletedFlag = false;
    // ==================================================

    std::shared_ptr<IRSpectrogramObjectPreference> preference;
    
    // ==================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRSpectrogramWithPreference)

    
};
#endif /* IRSpectrogramWithPreference_hpp */
