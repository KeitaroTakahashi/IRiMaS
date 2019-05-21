//
//  IRImageMovableButton.cpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 20/05/2019.
//

#include "IRImageMovableButton.hpp"

IRImageMovableButton::IRImageMovableButton()
{
    setImageByStatus();
}

IRImageMovableButton::~IRImageMovableButton()
{
    
}


void IRImageMovableButton::mouseDown(const MouseEvent& e)
{
    
    this->status = static_cast<movableStatus>((static_cast<int>(this->status) + 1) % 4);
    setImageByStatus(); 
    if(this->statusClicked != nullptr) statusClicked(this->status);

}

Image IRImageMovableButton::setImageByStatus()
{
    switch(this->status)
    {
        case NOTMOVABLE:
            setImage(ImageBank.getNotMovable());
            break;
        case HORIZONTALMOVABLE:
            setImage(ImageBank.getHorizontalMovable());
            break;
        case VERTICALMOVABLE:
            setImage(ImageBank.getVerticalMovable());
            break;
        case CROSSMOVABLE:
            setImage(ImageBank.getCrossMovable());
            break;
        default:
            return Image();
            break;
            
    }
    
    return Image();
}

