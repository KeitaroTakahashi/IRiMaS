//
//  IRWorkspaceSlide.hpp
//  IRiMaS
//
//  Created by Keitaro on 17/09/2019.
//

#ifndef IRWorkspaceSlide_hpp
#define IRWorkspaceSlide_hpp

#include "JuceHeader.h"
#include "IRStrComponent.hpp"


class IRWorkspaceSlide : public Component,
public IRStrComponent
{
public:
    IRWorkspaceSlide(IRStr* str, int index);
    ~IRWorkspaceSlide();
    // ==================================================

    void paint(Graphics& g) override;
    
    void mouseDown(const MouseEvent& e) override;
    
    std::function<void()> onClick = nullptr;
    // ==================================================
    
    void setSelectedFlag(bool flag) { this->isSelectedFlag = flag; repaint(); }
    bool isSelected() const { return this->isSelectedFlag; }
    // ==================================================

private:
    
    int index;
    bool isSelectedFlag = false;
};
#endif /* IRWorkspaceSlide_hpp */
