//
//  IRProjectWindow2.hpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#ifndef IRProjectWindow2_hpp
#define IRProjectWindow2_hpp

#include "IRMainWindow.hpp"

class IRProjectWindow2 : public IRMainWindow,
public ChangeListener
{
public:
    IRProjectWindow2(String name);
    ~IRProjectWindow2();
    
    
    void windowMoveToPos(Point<int>pos);
    
    
private:
    
    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    void closeButtonPressed() override;
    

    
    std::shared_ptr<IRWindowComponent> comp;
   
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRProjectWindow2)

};

#endif /* IRProjectWindow2_hpp */