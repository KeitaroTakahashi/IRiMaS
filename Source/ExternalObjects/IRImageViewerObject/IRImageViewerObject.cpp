
#include "IRImageViewerObject.hpp"





IRImageViewerObject::IRImageViewerObject(Component* parent) : IRNodeObject(parent, "IRImageViewer"),
imageViewer(this)
{
    
    std::cout << "IRImageViewerObject" << std::endl;
    this->imageViewer.setBounds(5, 5, getWidth() - 10, getHeight() - 10);
    addAndMakeVisible(this->imageViewer);
    childComponentManager(&this->imageViewer);
    
    this->imageViewer.addChangeListener(this);
    setSize(150, 150);
}


IRImageViewerObject::~IRImageViewerObject()
{

}


// copy constructor
IRNodeObject* IRImageViewerObject::copyThis()
{
    return new IRImageViewerObject(this->parent);
}


t_json IRImageViewerObject::saveThisToSaveData()
{
    std::string imgPath = this->imageViewer.getFilePath().toStdString();
    
    Rectangle<int> b = this->imageViewer.getBounds();
    t_json imageViewerData = t_json::object({
        {"bounds", t_json::array({b.getX(), b.getY(), b.getWidth(), b.getHeight()})},
        {"imgPath", imgPath},
    });
    
    
    
    t_json saveData = t_json::object({
        
        { "imageViewer", imageViewerData }
    });
    
    
    return saveData;
}


void IRImageViewerObject::loadThisFromSaveData(t_json saveData)
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


void IRImageViewerObject::resized()
{
    std::cout << "IRImageViewerObject resized " << getWidth() << ", " << getHeight() << std::endl;
    
    this->imageViewer.setBounds(5,5, getWidth()-10, getHeight()-10);
}


void IRImageViewerObject::paint(Graphics& g)
{
    if (isEditMode())
    {
        auto area = getLocalBounds().reduced(2);
        // FD - PROV. ADD-
        if (isSelected())
            g.setColour(Colours::red);
        else
            g.setColour(SYSTEMCOLOUR.contents);
        g.drawRoundedRectangle(area.toFloat(), 5.0f, 2.0f);
    }
}


// resizing method
// this method employs different resizing way with shift key.
void IRImageViewerObject::resizeThisComponentEvent(const MouseEvent& e)
{
    double ratio = this->imageViewer.getAspectRatio();
    
    
    std::cout << "resizeThisComponentEvent isShiftDown = " << e.mods.isShiftDown() << " : ratio = " << ratio << std::endl;
    
    if (e.mods.isShiftDown() && ratio != 0)
    {
        float deltaX = e.getScreenX() - e.getMouseDownScreenX();
        float deltaY = e.getScreenY() - e.getMouseDownScreenY();
        
        float newWidth = getPreviousWidth();
        float newHeight = getPreviousHeight();
        
        if(deltaX > deltaY)
        {
            newWidth += deltaX;
            newHeight = (double) newWidth / ratio;
        }
        else
        {
            newHeight += deltaY;
            newWidth = (double) newHeight * ratio;
        }
        setSize(newWidth, newHeight);
    }
    else
    {
        IRNodeComponent::resizeThisComponentEvent(e); // ordinary resizing method
    }
}




// **** **** PRIVATE METHODS **** **** //

void IRImageViewerObject::statusChangedCallback(IRNodeComponentStatus status)
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


void IRImageViewerObject::changeListenerCallback (ChangeBroadcaster* source)
{
    if (source == &this->imageViewer)
    {
        setSize(this->imageViewer.getWidth(), this->imageViewer.getHeight());
    }
}




