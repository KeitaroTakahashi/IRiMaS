//
//  IRVideoAnnotaterObject2.cpp
//  IRiMaS
//
//  Created by Keitaro on 10/05/2020.
//

#include "IRVideoAnnotaterObject2.hpp"


IRVideoAnnotaterObject2::IRVideoAnnotaterObject2(Component* parent, IRStr* str, bool withOpenButton) :
IRNodeObject(parent, "IRVideoAnnotater2", str, NodeObjectType(ordinaryIRComponent))

{
    
    this->workspace.reset(new IRVideoAnnotaterWorkspace("Video Annotater", Rectangle<int>(0, 0, 0, 0), str, withOpenButton));
    this->workspace->setDraggableMargin(Rectangle<int>(0, 0, 0, 0));
    this->workspace->videoLoadCompletedCallback = [this] { videoLoadCompletedAction(); };
    this->workspace->videoPlayingUpdateCallback = [this] (double pos) { videoPlayingUpdateAction(pos); };
    addAndMakeVisible(this->workspace.get());
    //this->annotaterWorkspace->setBackgroundColour(Colours::transparentBlack);
    //this->annotaterWorkspace->addListener(this);
    //this->annotaterWorkspace->addKeyListener(this);
    
    setObjectSize(300, 200);
    
    setWantsKeyboardFocus(true);
    
    
    //createTextObject();
}

IRVideoAnnotaterObject2::~IRVideoAnnotaterObject2()
{
    this->workspace.reset();
}
// --------------------------------------------------

void IRVideoAnnotaterObject2::resized()
{
    std::cout << "videoANnoter2 resized\n";
    this->workspace->setBounds(getLocalBounds());
    

    

}


void IRVideoAnnotaterObject2::resizeThisComponentEvent(const MouseEvent& e)
{
    // turn off controller otherwise mouse event will be stolen by the controller,
    // and resize event can not be acomplished properly.
    
    auto videoPlayer = this->workspace->getVideoPlayerObject()->getVideoPlayer();
    
    
    double ratio = videoPlayer->getAspectRatio();
    if(ratio != 0){
        
        float deltaX = e.getScreenX() - e.getMouseDownScreenX();
        float deltaY = e.getScreenY() - e.getMouseDownScreenY();
        
        float newWidth = getPreviousWidth();
        float newHeight = getPreviousHeight();
        
        if(deltaX > deltaY)
        {
            newWidth += deltaX;
            newHeight = (double) newWidth / videoPlayer->getAspectRatio();
        }else{
            newHeight += deltaY;
            newWidth = (double) newHeight * videoPlayer->getAspectRatio();
        }
        setObjectSize(newWidth, newHeight);

    }else{
        IRNodeComponent::resizeThisComponentEvent(e);
    }
    
   // this->resizing = true;
}

void IRVideoAnnotaterObject2::resizeAndCentredThisComponent(Rectangle<int> rect)
{
    double ratio = this->workspace->getVideoPlayerObject()->getVideoPlayer()->getAspectRatio();
    
    std::cout << "IRVideoPlayerObject2 resizeThisComponent : ratio = " << ratio << std::endl;

    if(ratio >= 0)
    {
        float curr_w = (float)getWidth();
        float curr_h = (float)getHeight();
        float new_w  = (float)rect.getWidth();
        float new_h  = (float)rect.getHeight();
        
        float ratio_w = curr_w / new_w;
        float ratio_h = curr_h / new_h;
        
        std::cout << "ratio_w = " << ratio_w << " : ratio_h = " << ratio_h << std::endl;
        
        float fixed_h = new_w / ratio;
        float fixed_w = new_h * ratio;
        if(fixed_h > new_h){
            fixed_h = new_h;
            fixed_w = fixed_h * ratio;
        }
        
        if(fixed_w > new_w){
            fixed_w = new_w;
            fixed_h = fixed_w / ratio;
        }
        
        
                
        // if w is larger, then follow w
        if(ratio_w >= ratio_h)
        {
            
            //float fixed_h = new_w / ratio;
            float y = (rect.getHeight() - fixed_h) / 2.0;
            setObjectBounds(rect.getX(), rect.getY() + y, new_w, fixed_h);
            //this->workspace->setBounds(rect.getX(), rect.getY() + y, new_w, fixed_h);

        }else{
            //float fixed_w = new_h * ratio;
            float x = (rect.getWidth() - fixed_w) / 2.0;

            setObjectBounds(rect.getX() + x, rect.getY(), fixed_w, new_h);
            //this->workspace->setBounds(rect.getX() + x, rect.getY(), fixed_w, new_h);

        }
        
    }else{
        setObjectBounds(rect);
        //this->workspace->setBounds(rect);

    }
    
}


void IRVideoAnnotaterObject2::resizeThisComponent(Rectangle<int> rect)
{
    double ratio = this->workspace->getVideoPlayerObject()->getVideoPlayer()->getAspectRatio();
    
    std::cout << "IRVideoPlayerObject2 resizeThisComponent : ratio = " << ratio << std::endl;

    if(ratio >= 0 && ratio != 1.0)
    {
        float curr_w = (float)getWidth();
        float curr_h = (float)getHeight();
        float new_w  = (float)rect.getWidth();
        float new_h  = (float)rect.getHeight();
        
        float ratio_w = curr_w / new_w;
        float ratio_h = curr_h / new_h;
        
        std::cout << "ratio_w = " << ratio_w << " : ratio_h = " << ratio_h << std::endl;
        
        float fixed_h = new_w / ratio;
        float fixed_w = new_h * ratio;
        if(fixed_h > new_h){
            fixed_h = new_h;
            fixed_w = fixed_h * ratio;
        }
        
        if(fixed_w > new_w){
            fixed_w = new_w;
            fixed_h = fixed_w / ratio;
        }
        
        
                
        // if w is larger, then follow w
        if(ratio_w >= ratio_h)
        {
            
            //float fixed_h = new_w / ratio;
            float y = (rect.getHeight() - fixed_h) / 2.0;
            setObjectBounds(rect.getX(), rect.getY(), new_w, fixed_h);
            //this->workspace->setBounds(rect.getX(), rect.getY(), new_w, fixed_h);
            //this->workspace->setBounds(0, 0, new_w, fixed_h);

        }else{
            //float fixed_w = new_h * ratio;
            float x = (rect.getWidth() - fixed_w) / 2.0;

            setObjectBounds(rect.getX(), rect.getY(), fixed_w, new_h);
            //this->workspace->setBounds(rect.getX(), rect.getY(), fixed_w, new_h);
            //this->workspace->setBounds(0, 0, fixed_w, new_h);

        }
        
    }else{
        setObjectBounds(rect);
        //this->workspace->setBounds(rect);

    }
    
}


void IRVideoAnnotaterObject2::setFixObjectSizeRatioWithOriginalSize(bool flag, Rectangle<float> originalSize)
{
    this->workspace->setFixObjectSizeRatioWithOriginalSize(flag, originalSize);
}
// --------------------------------------------------

void IRVideoAnnotaterObject2::paint(Graphics& g) 
{
    //g.fillAll(Colours::yellow);
}

// --------------------------------------------------

// --------------------------------------------------
// --------------------------------------------------
// --------------------------------------------------

// --------------------------------------------------

void IRVideoAnnotaterObject2::videoLoadCompletedCallback()
{
    std::cout << "IRVideoAnnotaterObject2::videoLoadCompletedCallback\n";

    //resized();

}


// --------------------------------------------------
// define if call videoLoadCompletedCallbackFunc();
void IRVideoAnnotaterObject2::openFile(File file, bool isCallback)
{
    
}
void IRVideoAnnotaterObject2::openFile(bool isCallbback)
{
    
}

// --------------------------------------------------
void IRVideoAnnotaterObject2::play()
{
    this->workspace->getVideoPlayerObject()->play();
}
void IRVideoAnnotaterObject2::stop()
{
    this->workspace->getVideoPlayerObject()->stop();

}
void IRVideoAnnotaterObject2::setPlayPosition(double newPositionInSec)
{
    this->workspace->getVideoPlayerObject()->setPlayPosition(newPositionInSec);
}

juce::Point<int> IRVideoAnnotaterObject2::getVideoSize()
{
    return this->workspace->getVideoPlayerObject()->getVideoSize();

}
// --------------------------------------------------

// --------------------------------------------------
void IRVideoAnnotaterObject2::enableController(bool flag)
{
    this->workspace->getVideoPlayerObject()->enableController(flag);

}
// --------------------------------------------------
IRVideoPlayerObject2* IRVideoAnnotaterObject2::getVideoPlayerObject()
{
    return this->workspace->getVideoPlayerObject();
}

// --------------------------------------------------

// --------------------------------------------------

void IRVideoAnnotaterObject2::videoLoadCompletedAction()
{

    resizeThisComponent(getBounds());
    
    if(this->videoLoadCompletedCallbackFunc != nullptr)
        this->videoLoadCompletedCallbackFunc();
}

void IRVideoAnnotaterObject2::videoPlayingUpdateAction(double pos)
{
    //std::cout << "IRVideoAnnotaterObject2::videoPlayingUpdateAction : " << pos << std::endl;
    
    if(this->videoPlayingUpdateCallbackFunc != nullptr)
        this->videoPlayingUpdateCallbackFunc(pos);
}


// --------------------------------------------------

void IRVideoAnnotaterObject2::createTextObject(Component* event)
{
    this->workspace->createTextObject(event);
}

void IRVideoAnnotaterObject2::createShapeObject(Component* event)
{
    this->workspace->createShapeObject(event);

}
void IRVideoAnnotaterObject2::createImageObject(Component* event)
{
    this->workspace->createImageObject(event);

}
// --------------------------------------------------
