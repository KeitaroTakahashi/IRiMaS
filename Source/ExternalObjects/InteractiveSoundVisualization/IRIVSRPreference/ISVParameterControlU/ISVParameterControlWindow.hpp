//
//  ISVParameterControlWindow.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 03/06/2019.
//

#ifndef ISVParameterControlWindow_hpp
#define ISVParameterControlWindow_hpp

#include "JuceHeader.h"
#include "IRComponents.hpp"
#include "ISVParameterControlUI.hpp"


class ISVParameterControlWindow : public DocumentWindow,
public IRComponents,
public ChangeListener,
public ChangeBroadcaster
{
public:
    ISVParameterControlWindow(ISVParameterControlUI* ui, String name, Rectangle<int> frameRect);
    ~ISVParameterControlWindow();
    
    ISVParameterControlUI* getControlUI() const { return this->controlUI.get(); }
    
    void show();
    
private:
    
    void changeListenerCallback(ChangeBroadcaster* source) override;
    void closeButtonPressed() override;

    std::shared_ptr<ISVParameterControlUI> controlUI;
    ISVParameterControlUI* ui;
    String name;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ISVParameterControlWindow)

    
};


#endif /* ISVParameterControlWindow_hpp */
