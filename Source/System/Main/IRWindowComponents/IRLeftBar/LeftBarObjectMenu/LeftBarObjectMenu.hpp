//
//  LeftBarObjectMenu.hpp
//  IRiMaS
//
//  Created by Keitaro on 10/08/2019.
//

#ifndef LeftBarObjectMenu_hpp
#define LeftBarObjectMenu_hpp

#include "TextMenuComponent.hpp"
#include "ImageMenuComponent.hpp"
#include "AudioMenuComponent.hpp"

#include "IRIconBank.hpp"
#include "ColourLib.h"
#include "IRImageButton.hpp"
#include "KeAnimationComponent.h"
#include "IRNodeObjectType.h"


class LeftBarObjectMenu : public Component,
public ChangeBroadcaster,
private KeAnimationComponent
{
public:


    LeftBarObjectMenu(int buttonSize, int topMarge, int leftMarge, int yMarge, int menuSpace, int buttomSpace, Component* parent);
    ~LeftBarObjectMenu();
    
    // ==================================================

    void resized() override;
    void paint(Graphics& g) override;
    void paintSelectedItem(Graphics& g);
    // ==================================================

    void addButtons();
    void showExtraMenu(objectCategory type);
    
    objectCategory getSelectedButtonType() const { return this->selectedButtonType; }
    void resetSelection() { this->selectedButtonType = NONE; repaint(); }
    // ==================================================

    ObjectMenuComponent* getCurrentMenuComponent() { return this->currentMenu; }
    
private:
    // ==================================================
    void createButton(IRImageButton* button, IRIconBank::IRIconImage img);
    
    void updateAnimationFrame() override;
    
    void replaceCurrentMenu(ObjectMenuComponent* obj);
    // ==================================================
    void toNavigatorAction();
    
    void textAction();
    void imageAction();
    void audioAction();
    void chartAction();
    void playerAction();
    void objectAction();
    
    void inspectorAction();
    void preferenceAction();
    
    // ==================================================
    int buttonSize = 40;
    objectCategory selectedButtonType = NONE;
    // ==================================================

    
    IRImageButton textButton;
    IRImageButton imageButton;
    IRImageButton audioButton;
    IRImageButton chartButton;
    IRImageButton playerButton;
    IRImageButton objectButton;

    IRImageButton inspectorButton;
    IRImageButton preferenceButton;
    // ==================================================
    std::shared_ptr<TextMenuComponent> textMenu;
    std::shared_ptr<ImageMenuComponent> imageMenu;
    std::shared_ptr<AudioMenuComponent> audioMenu;
    std::shared_ptr<ObjectMenuComponent> chartMenu;
    std::shared_ptr<ObjectMenuComponent> playerMenu;
    std::shared_ptr<ObjectMenuComponent> objectMenu;
    
    std::shared_ptr<ObjectMenuComponent> inspectorMenu;
    std::shared_ptr<ObjectMenuComponent> preferenceMenu;
    
    ObjectMenuComponent* currentMenu = nullptr;

    // ==================================================
    // marge
    
    int topMarge = 10;
    int switchSpace = 50;
    int menuSpace = 200;
    int buttomSpace = 110;
    
    int yMarge = 5;
    int leftMarge = 10;
    // ==================================================

    Component* parent = nullptr;
    
    Rectangle<int> menuSpaceRect;
    // ==================================================

    IRIconBank& ImageBank = singleton<IRIconBank>::get_instance();
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LeftBarObjectMenu)

    
};


#endif /* LeftBarObjectMenu_hpp */
