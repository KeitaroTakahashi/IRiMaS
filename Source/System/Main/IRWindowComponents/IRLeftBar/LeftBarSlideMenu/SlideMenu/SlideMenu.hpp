//
//  SlideMenu.hpp
//  IRiMaS
//
//  Created by Keitaro on 17/09/2019.
//

#ifndef SlideMenu_hpp
#define SlideMenu_hpp

#include "IRWorkspaceSlide.hpp"

class SlideMenu : public Component,
public IRStrComponent
{
public:
    SlideMenu(IRStr* str);
    ~SlideMenu();
    // ==================================================

    void resized() override;
    void paint(Graphics& g) override;
    // ==================================================

    void addSlide(IRWorkspace* space);
    void slideSelectedAction(IRWorkspaceSlide* slide);
    
    // ==================================================

    void addNewWorkspaceSlide(IRWorkspace* space);
    
    // ==================================================

    std::function<void()> slideMenuUpdated;
    std::function<void(IRWorkspaceSlide*)> slideHasSelected;
    // ==================================================

private:
    // ==================================================

    std::vector<IRWorkspaceSlide* > slides;
    // ==================================================

    int yMargin = 10;
    // ==================================================

    int sceneHeight = 60;
    int itemHeight = 80;
    
    // ==================================================
    // ==================================================

};
#endif /* SlideMenu_hpp */
