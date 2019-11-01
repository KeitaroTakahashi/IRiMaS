//
//  IRTextEditorController.hpp
//  IRiMaS
//
//  Created by Keitaro on 02/10/2019.
//

#ifndef IRTextEditorController_hpp
#define IRTextEditorController_hpp

#include "IRObjectController.hpp"
#include "FontController.h"

class IRTextEditorController : public IRObjectController,
public ChangeListener,
public ChangeBroadcaster
{
public:
    
    IRTextEditorController(IRStr* str);
    ~IRTextEditorController();

    void resized() override;
    void paint(Graphics& g) override;
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
        
    FontController* getFontController();

        
private:

    Label LabelTitle;
    
        // FontGUI* fontGUI;
    std::unique_ptr<FontController> fontController;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRTextEditorController)

};
#endif /* IRTextEditorController_hpp */
