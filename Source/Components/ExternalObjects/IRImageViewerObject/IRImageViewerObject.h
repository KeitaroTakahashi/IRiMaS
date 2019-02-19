//
//  IRImageViwerObject.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 24/08/2018.
//

#ifndef IRImageViwerObject_h
#define IRImageViwerObject_h

#include "IRImageLoader.hpp"
#include "IRImageViewer.h"

#include "IRNodeObject.hpp"

class IRImageViewerObject : public IRNodeObject, private ChangeListener
{
public:
    IRImageViewerObject(Component* parent) : IRNodeObject(parent, "IRImageViwer")
    {
        
        printf("IRImageViewerObject\n");
        this->imageViewer.setBounds(5,5, getWidth()-10, getHeight()-10);
        addAndMakeVisible(this->imageViewer);
        childComponentManager(&this->imageViewer);
        
        this->imageViewer.addChangeListener(this);
        setSize(150,150);
    }
    ~IRImageViewerObject()
    {
        
    }
    
    // ------------------------------------------------------------
    // copy constructor
    IRNodeObject* copyThis() override
    {
        return new IRImageViewerObject(this->parent);
    }
    
    // ------------------------------------------------------------
    t_json saveThisToSaveData() override
    {
        std::string imgPath = this->imageViewer.getFilePath().toStdString();
        
        Rectangle<int> b = this->imageViewer.getBounds();
        t_json imageViwerData = t_json::object({
            {"bounds", t_json::array({b.getX(), b.getY(), b.getWidth(), b.getHeight()})},
            {"imgPath", imgPath},
        });
        

        
        t_json saveData = t_json::object({
            
            {"imageViewer", imageViwerData}
        });
        
        
        return saveData;
    }
    
    void loadThisFromSaveData(t_json saveData) override
    {
        t_json data = saveData["imageViewer"];
        
        this->imageViewer.openFile(String(data["imgPath"].string_value()));
        
        t_json::array b = data["bounds"].array_items();
        this->imageViewer.setBounds(b[0].int_value(),
                                    b[1].int_value(),
                                    b[2].int_value(),
                                    b[3].int_value());
        
        std::cout << "image viewer " << b[2].int_value() << ", " << b[3].int_value() << std::endl;
        
        std::cout << "image path = " << data["imgPath"].string_value() << std::endl;
    }
    
    // ------------------------------------------------------------
    void resized() override
    {
        std::cout << "IRImageViewerObject resized " << getWidth() << ", " << getHeight() << std::endl;

        this->imageViewer.setBounds(5,5, getWidth()-10, getHeight()-10);
    }
    // ------------------------------------------------------------
    void paint(Graphics& g) override
    {
        if(isEditMode())
        {
            auto area = getLocalBounds().reduced (2);
            g.setColour (SYSTEMCOLOUR.contents);
            g.drawRoundedRectangle (area.toFloat(), 5.0f, 2.0f);
        }
    }
    // ------------------------------------------------------------
    // resizing method
    // this method employs different resizing way with shift key.
    void resizeThisComponentEvent(const MouseEvent& e) override
    {
        double ratio = this->imageViewer.getAspectRatio();
        if(e.mods.isShiftDown() && ratio != 0)
        {
            float deltaX = e.getScreenX() - e.getMouseDownScreenX();
            float deltaY = e.getScreenY() - e.getMouseDownScreenY();

            float newWidth = getPreviousWidth();
            float newHeight = getPreviousHeight();
            
            if(deltaX > deltaY)
            {
                newWidth += deltaX;
                newHeight = (double) newWidth / ratio;
            }else{
                newHeight += deltaY;
                newWidth = (double) newHeight * ratio;
            }
            setSize(newWidth, newHeight);
        }else{
            IRNodeComponent::resizeThisComponentEvent(e); // ordinary resizing method
        }
    }
    
private:
    
    // ------------------------------------------------------------
    // call back function automatically called when the status of this object changed by others.
    // write some tasks here
    void statusChangedCallback(IRNodeComponentStatus status) override
    {
        switch (status)
        {
            case EditModeStatus:
                break;
            case SelectableStatus:
                
                break;
            case HasResizedStatus:
                
                break;
            default:
                break;
        }
    }
    // ------------------------------------------------------------
    void changeListenerCallback (ChangeBroadcaster* source)override
    {
        if(source == &this->imageViewer)
        {
            setSize(this->imageViewer.getWidth(), this->imageViewer.getHeight());
        }
    }
    // ------------------------------------------------------------

    IRImageViwer imageViewer;
};

#endif /* IRImageViwerObject_h */
