//
//  IRVideoThumnailObject.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 04/02/2019.
//

#ifndef IRVideoThumnailObject_hpp
#define IRVideoThumnailObject_hpp

/*
#include "IRNodeObject.hpp"
#include "IRVideoThumbnail.hpp"

class IRVideoThumbnailObject : public IRNodeObject
{
public:
    IRVideoThumbnailObject(Component* parent) : IRNodeObject(parent, "IRVideoThumbnail")
    {
        this->thumb = new IRVideoThumbnail();
        this->thumb->setBounds(getLocalBounds().reduced(5));
        this->thumb->thumbLoadCompleted = [this]{ thumbLoadedAction(); };
        addAndMakeVisible(this->thumb);
    }
    
    ~IRVideoThumbnailObject()
    {
        delete this->thumb;
    }
    // --------------------------------------------------
    void resized() override
    {
        this->thumb->setBounds(getLocalBounds().reduced(5));
    }
    void paint(Graphics& g) override
    {
        IRNodeObject::paint(g);
        
        auto area = getLocalBounds().reduced (2);
        g.setColour (SYSTEMCOLOUR.contents);
        g.drawRoundedRectangle (area.toFloat(), 5.0f, 4.0f);
    }
    // --------------------------------------------------
    void thumbLoadedAction()
    {
        std::cout << "thum loaded!!\n";
        int h = this->thumb->getThumbSize().getY() + 5;
        setSize(500,h);
        setMaximumHeight(h);
        setMinimumHeight(h);
    }
    // --------------------------------------------------
    // --------------------------------------------------

    
    
private:
    IRVideoThumbnail* thumb;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRVideoThumbnailObject)

};
 */
#endif /* IRVideoThumnailObject_hpp */
