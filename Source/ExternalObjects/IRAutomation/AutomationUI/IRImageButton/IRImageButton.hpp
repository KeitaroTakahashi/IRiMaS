//
//  IRImageButton.hpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 02/05/2019.
//

#ifndef IRImageButton_hpp
#define IRImageButton_hpp

#include "JuceHeader.h"

class IRImageButton : public Component
{
public:
    IRImageButton();
    ~IRImageButton();
    
    void paint(Graphics& g) override;
    void resized() override;
    
    void setImage(Image img);
    
    virtual void mouseDown(const MouseEvent& e) override;
    
    std::function<void()> onClick = nullptr;
    
private:
    
    Image buttonImage;

};
#endif /* IRImageButton_hpp */
