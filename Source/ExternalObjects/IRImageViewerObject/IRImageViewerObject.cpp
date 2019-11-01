
#include "IRImageViewerObject.hpp"

IRImageViewerObject::IRImageViewerObject(Component* parent, IRStr* str) :
IRNodeObject(parent, "IRImageViewer", str, NodeObjectType(orginaryIRComponent))
{
    
    this->controller.reset( new IRImageViewerController(str) );
    this->controller->addChangeListener(this);
    setObjController(this->controller.get());
    
    // original function to give this ChangeListener to controller->UI
    this->controller->addChangeListener(this);
    
    std::cout << "IRImageViewerObject" << std::endl;
    this->imageViewer = std::make_shared<IRImageViewer>(this, str);

    this->imageViewer->setBounds(5, 5, getWidth() - 10, getHeight() - 10);
    addAndMakeVisible(this->imageViewer.get());
    childComponentManager(this->imageViewer.get());
    this->imageViewer->addChangeListener(this);


    setSize(150, 150);
    
    clearLinkParam();
    addLinkParam(ImageLinkFlag);
    addLinkParam(ConsoleLinkFlag);  
}


IRImageViewerObject::~IRImageViewerObject()
{
    this->controller.reset();
}


// copy constructor
IRNodeObject* IRImageViewerObject::copyThis()
{
    return new IRImageViewerObject(this->parent, getStr());
}


t_json IRImageViewerObject::saveThisToSaveData()
{
    std::string imgPath = this->imageViewer->getFilePath().toStdString();
    
    Rectangle<int> b = this->imageViewer->getBounds();
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
    
    this->imageViewer->openFile(String(data["imgPath"].string_value()));
    
    t_json::array b = data["bounds"].array_items();
    this->imageViewer->setBounds(b[0].int_value(),
                                b[1].int_value(),
                                b[2].int_value(),
                                b[3].int_value());
    
    std::cout << "image viewer " << b[2].int_value() << ", " << b[3].int_value() << std::endl;
    
    std::cout << "image path = " << data["imgPath"].string_value() << std::endl;
}


void IRImageViewerObject::resized()
{
    std::cout << "IRImageViewerObject resized " << getWidth() << ", " << getHeight() << std::endl;
    
    this->imageViewer->setBounds(0, 0, getWidth(), getHeight());
}


void IRImageViewerObject::paint(Graphics& g)
{
    if (isEditMode())
    {
        auto area = getLocalBounds();
        g.setColour(getStr()->SYSTEMCOLOUR.contents);
        g.drawRect(area.toFloat(), 1.0f);
    }
}


// resizing method
// this method employs different resizing way with shift key.
void IRImageViewerObject::resizeThisComponentEvent(const MouseEvent& e)
{
    double ratio = this->imageViewer->getAspectRatio();
    
    
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
    if (source == this->imageViewer.get())
    {
        setSize(this->imageViewer->getWidth(), this->imageViewer->getHeight());
    }
    else if(source == this->controller.get())
    {
        
        std::cout << "changeListenerCallback\n";

        using uiStatus = ImageController::ImageControllerStatus;
        
        switch(this->controller->getStatus())
        {
            case uiStatus::OpenImageFile:
                this->imageViewer->openFile();
                break;
            default:
                break;
        }
    }
}




