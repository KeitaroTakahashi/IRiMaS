//
//  IRPlayPauseButton.h
//  IRiMaS
//
//  Created by Keitaro on 30/06/2020.
//

#ifndef IRPlayPauseButton_h
#define IRPlayPauseButton_h

#include "IRIconBank.hpp"
#include "IRImageButton.hpp"
#include "IRStrComponent.hpp"

class IRPlayPauseButton : public IRImageButton,
public IRStrComponent
{
public:
    IRPlayPauseButton(IRStr* str) : IRStrComponent(str)
    {
        
    }
    
    ~IRPlayPauseButton()
    {
        
    }
    
    // --------------------------------------------------
    void resized() override
    {
        
    }
    
    void paint(Graphics& g) override
    {
        
    }
    
    // --------------------------------------------------

    
    // --------------------------------------------------
    
    void setImage(IRIconBank::IRIconImage trueImage, IRIconBank::IRIconImage falseImage)
    {
        if(getStr()->SYSTEMCOLOUR.isWhiteBased)
        {
            this->trueImage = trueImage.black;
            this->falseImage = falseImage.black;
        }else{
            this->trueImage = trueImage.white;
            this->falseImage = falseImage.white;
        }
        
        repaint();
    }
    
    void setImage(Image trueImage, Image falseImage)
    {
        this->trueImage = trueImage;
        this->falseImage = falseImage;
        
        repaint();
    }
    // --------------------------------------------------

    
    // --------------------------------------------------
    // --------------------------------------------------

private:
    
    IRImage trueImage;
    IRImage falseImage;
    
};


#endif /* IRPlayPauseButton_h */
