//
//  IRWindowComponent.hpp
//  IRiMaS
//
//  Created by Keitaro on 09/08/2019.
//

#ifndef IRWindowComponent_hpp
#define IRWindowComponent_hpp


#include "IRStr.h"
#include "IRTitleBar.hpp"
#include "IRLeftBar.hpp"
#include "IRMainSpace.hpp"

class IRWindowComponent : public Component,
public ChangeListener,
public KeyListener
{
public:
    IRWindowComponent(String projectName,
                      Rectangle<int> frameRect);
    ~IRWindowComponent();
    
    // ==================================================
    
    void paint(Graphics& g) override;
    void resized() override;
    
    // ==================================================
    void mouseDrag(const MouseEvent& e) override;
    void mouseUp(const MouseEvent& e) override;
    void mouseDown(const MouseEvent& e)override;
    void mouseMove(const MouseEvent& e)override;
    // ==================================================
    
    void setComponentsHeight(int barHeight, int mainHeight);
    
    // ==================================================
    // ==================================================
    // ==================================================
    // ==================================================
    // IRWorkspace
    void createNewWorkspace();

    std::vector<IRWorkspace* > getWorkspaces() const { return this->workspaces; }
    IRWorkspace* getTopWorkspace() const { return this->topWorkspace; }
    
    // ==================================================
    Point<int> pos;
    Point<int> currentPos;
    Point<int> prevPos;
    
    std::function<void(Point<int>)> windowMoveAction;
    
    bool isResizable = false;
    int resizableMargin = 20;
    
private:
    // ==================================================
    // SYSTEM
    void initialize();
    void createComponents();
    
    void changeListenerCallback (ChangeBroadcaster* source) override;

    // ==================================================

    // Key Event
    bool keyPressed(const KeyPress& key, Component* originatingComponent) override;
    void modifierKeysChanged(const ModifierKeys &mod) override;

    bool isShiftPressed = false;
    bool isCommandPressed = false;
    bool isControlPressed = false;
    bool isAltPressed = false;
    bool isOptionPressed = false;
    
    // ==================================================

    int barHeight = 50;
    int leftBarWidth = 135;
    int mainComponentHeight = 400;

    // ==================================================
    // Project
    String projectName;
    Rectangle<int> frameRect;
    
    std::shared_ptr<IRStr> ir_str;

    std::shared_ptr<IRMainSpace> mainSpace;
    std::shared_ptr<IRTitleBar> bar;
    std::shared_ptr<IRLeftBar> leftBar;
    
    // ==================================================

    // workspace
    std::vector<IRWorkspace* > workspaces;
    IRWorkspace* topWorkspace = nullptr;
    
    
    
    // ==================================================
    
    // define the minimum size of the window
    int minWidth = 400;
    int minHeight = 700;
    
    // store window size before changing it.
    Point<int> prevSize;
    // ==================================================

    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    // ==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRWindowComponent)
    
};
#endif /* IRWindowComponent_hpp */
