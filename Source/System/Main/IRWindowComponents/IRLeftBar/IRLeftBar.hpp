//
//  IRLeftBar.hpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#ifndef IRLeftBar_hpp
#define IRLeftBar_hpp


#include "JuceHeader.h"
#include "IRWindowHeader.h"
#include "IRIconBank.hpp"
#include "IRImageButton.hpp"
#include "LeftBarObjectMenu.hpp"

class IRLeftBar : public Component,
public ChangeBroadcaster,
public ChangeListener,
private KeAnimationComponent
{
public:
    IRLeftBar();
    ~IRLeftBar();
    //==================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseDown(const MouseEvent& e)override;
    void mouseMove(const MouseEvent& e)override;
    //==================================================
    
    void checkResizableFromMouseDownPosition(Point<int> pos);
    
    //==================================================
    
    IRWindowBarActionStatus getStatus() const { return this->status; }
    
    Point<int> pos;
    Point<int> currentPos;
    Point<int> prevPos;
    
    //==================================================
    
private:
    //==================================================
    void addButtons();
    void toNavigatorAction();
    void toObjectMenuAction();
    void createButton(IRImageButton* button, IRIconBank::IRIconImage img);
    //==================================================
    void changeListenerCallback (ChangeBroadcaster* source) override;

    //==================================================

    IRWindowBarActionStatus status;
    bool isResizable = true;
    
    //==================================================

    std::shared_ptr<LeftBarObjectMenu> objectMenuComponent;
    
    objectCategory currentMenuType = objectCategory::NONE;
    // ==================================================
    void updateAnimationFrame() override;
    
    
    //==================================================

    IRImageButton toNavigatorButton;
    IRImageButton toObjectMenuButton;
    //==================================================
    // layout
    int buttonSize = 50;
    
    int topMarge = 10;
    int switchSpace = 50;
    int menuSpace = 150;
    int buttomSpace = 130;
    int yMarge = 6; // better to be even
    int leftMarge = 10;
    
    int maxWidth = 0;
    int ordinaryWidth = 0;
    
    // ==================================================
    bool openMenuSpace = false;
    int openSpeed = 8;
    //==================================================
    
    IRIconBank& ImageBank = singleton<IRIconBank>::get_instance();
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    //==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRLeftBar)
    
};
#endif /* IRLeftBar_hpp */
