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
#include "IRStrComponent.hpp"
#include "IRImageButton.hpp"
#include "LeftBarObjectMenu.hpp"
#include "LeftBarSlideMenu.hpp"
#include "IROpenGLManager.hpp"
#include "ObjectSlideSwitchSpace.h"

class IRLeftBar : public Component,
public IRStrComponent,
public IRHeavyWeightComponent,
public ChangeBroadcaster,
public ChangeListener,
private KeAnimationComponent
{
public:
    IRLeftBar(IRStr* str);
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

    void bringToFrontCompleted() override;
    
    //==================================================
    void openMenu(objectCategory type);
    void closeMenu();
    //==================================================
    void addNewWorkspaceSlide(IRWorkspace* space);
    
    void workspaceSelectedAction(IRWorkspace* space);
    std::function<void(IRWorkspace*)> workspaceSelectedCallback;
    
    //==================================================

    IRWindowBarActionStatus getStatus() const { return this->status; }
    
    Point<int> pos;
    Point<int> currentPos;
    Point<int> prevPos;
    
    //==================================================
    
    // @param1 : notify the action to objectSlideSwitchButton or not
    void toNavigatorAction(bool notify = false);
    void toObjectMenuAction(bool notify = false);
    
    //==================================================   
   
private:
    
    //==================================================
    //OpenGL
    OpenGLContext openGLContext;
    bool isOpenGLComponentClosed = false;
    
   

    
    //==================================================
    void addButtons();

    void createButton(IRImageButton* button, IRIconBank::IRIconImage img);
    //==================================================
    void changeListenerCallback (ChangeBroadcaster* source) override;

    //==================================================

    IRWindowBarActionStatus status;
    bool isResizable = true;
    
    //==================================================

    std::shared_ptr<LeftBarObjectMenu> objectMenuComponent;
    std::shared_ptr<LeftBarSlideMenu> slideMenuComponent;

    objectCategory currentMenuType = objectCategory::NONE;
    objectCategory previousMenuType = objectCategory::NONE;
    // ==================================================
    void updateAnimationFrame() override;
    // ==================================================

    //==================================================

    IRImageButton toNavigatorButton;
    IRImageButton toObjectMenuButton;
    
    std::shared_ptr<ObjectSlideSwitchSpace> objectSlideSwitchButton;
    
    TextButton addNewSlideButton;
    TextButton deleteSlideButton;
    //==================================================
    // layout
    int buttonSize = 40;
    
    int topMarge = 10;
    int switchSpace = 50;
    int menuSpace = 150;
    int buttomSpace = 130;
    int yMarge = 6; // better to be even
    int leftMarge = 10;
    int rightMarge = 85;
    int slideMenuButtomMargin = 50;
    
    int maxWidth = 0;
    int preferenceMaxWidth = 600;
    int ordinaryWidth = 0;
    
    // ==================================================
    bool openMenuSpace = false;
    bool openPrefernceSpace = false;
    int openPreferenceSpeed = 24;
    int openSpeed = 12;
    bool isOpened = false; // if menu area is visible or not
    
    void openDefaultMenu();
    void openPreferenceMenu();
    
    void attachObjectMenu();
    void removeObjectMenu();
    void attachPreferenceMenu();
    void removePreferenceMenu();

    //==================================================
    
    //IRIconBank& ImageBank = singleton<IRIconBank>::get_instance();
    //IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    //==================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRLeftBar)
    
};
#endif /* IRLeftBar_hpp */
