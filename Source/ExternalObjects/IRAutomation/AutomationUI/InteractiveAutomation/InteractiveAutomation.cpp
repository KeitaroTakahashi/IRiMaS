//
//  InteractiveAutomation.cpp
//  InteractiveAutomation - App
//
//  Created by Keitaro on 29/04/2019.
//

#include "InteractiveAutomation.hpp"

InteractiveAutomation::InteractiveAutomation(IRNodeObject* nodeObject) :
IRUIFoundation(nodeObject)
{
    this->verteces.clear();
    this->selector = std::make_shared<VertecesSelector>(&this->verteces, this);
    this->selector->setShiftConstrainsDirection(true);
    this->selector->setConstrainBoundsToParent(true, {0,0,10,10});
    this->selector->setMovableDirection(true, false);
    
    this->controller = std::make_shared<AutomationController>();
    this->controller->setZoomInEvent([this]{ zoomInClicked(); });
    this->controller->setZoomOutEvent([this]{ zoomOutClicked(); });
    this->controller->setMovableEvent([this](IRAutomation::movableStatus status){ movableClicked(status); });
    
    setWantsKeyboardFocus(true);
    //addKeyListener(this);
    
    
}

InteractiveAutomation::~InteractiveAutomation()
{
    
    this->verteces.clear();
    
    
}
// ==================================================


void InteractiveAutomation::paint(Graphics& g)
{
    g.fillAll(Colours::white);
    
    drawLinesBetweenVerteces(g);
    //drawVerteces(g);
    
}

void InteractiveAutomation::drawVerteces(Graphics& g)
{
    for(auto ver : this->verteces)
    {
        if(ver->getCondition() == uiSelected) g.setColour(Colours::red);
        else g.setColour(Colours::orange);
        
        //g.fillEllipse(ver->getCentreX(), ver->getCentreY(), ver->getWidth(), ver->getHeight());
    }
}

void InteractiveAutomation::drawLinesBetweenVerteces(Graphics& g)
{
    g.setColour(Colours::grey);
    
    for(int i = 1; i < this->verteces.size(); i++)
    {
        int wh = this->verteces[i-1]->getWidth()/2;
        int wh2 = this->verteces[i]->getWidth()/2;
        
        g.drawLine(this->verteces[i-1]->getX() + wh,
                   this->verteces[i-1]->getY() + wh,
                   this->verteces[i]->getX() + wh2,
                   this->verteces[i]->getY() + wh2,
                   2);
    }
}


// ==================================================

void InteractiveAutomation::resized()
{
    
    if(getHeight() < 50) setBounds(getX(), getY(), getWidth(), 50);
    
    int s = getHeight();
    if(s > 50) s = 50;
    int y = getHeight() / 2 - s / 2;
    
    this->controller->setBounds(0, y, getWidth(), s);
    
    reCalcPos();
    
    this->previousBounds = getLocalBounds();
}

void InteractiveAutomation::reCalcPos()
{
    
    float boundsRatio_h;
    if(this->previousBounds.getHeight() > 0.0) boundsRatio_h = (float)getLocalBounds().getHeight() / (float)this->previousBounds.getHeight();
    else boundsRatio_h = 1.0;
    
    if(boundsRatio_h == 0) boundsRatio_h = 1.0;
    //std::cout << "boundsRatio_h = " << boundsRatio_h << std::endl;
    float increment = (float) getWidth() / (float) this->verteces.size();
    // draw point on each Vertex if increment is larger than 1 pixel.
    bool isPoint = true;
    if(increment < 1.0) isPoint = false;
    
    
    for(int i = 0; i < this->verteces.size(); i ++)
    {
        float y = (this->verteces[i]->getPosition().getY()) * boundsRatio_h;
        
        //std::cout << "boundsRatio_h = " << boundsRatio_h << " : min = " << this->MinVertexValue <<" : val = " << (this->verteces[i]->getY() ) << " : y = " << y << std::endl;

        //this->verteces[i]->setTopLeftPosition((float)i * increment, y);
        this->verteces[i]->setPosition(Point<float>((float)i * increment, y));
        
        if(isPoint) this->verteces[i]->setPoint(true);
        else this->verteces[i]->setPoint(false);
    }
    
    // if there is no verteces, then add two verteces with 0 values.
    if(this->verteces.size() == 0)
    {
        createVertex(Point<int>(0,getHeight()), false);
        createVertex(Point<int>(getWidth(),getHeight()), false);
    }
}

void InteractiveAutomation::mouseDown(const MouseEvent& e)
{
    this->currentMousePos = e.getEventRelativeTo(this).getPosition();
    
    
    if(e.mods.isCtrlDown())
    {
        std::cout << "show controller\n";
        addAndMakeVisible(this->controller.get());
    }else {
        if(this->controller->isVisible())
            removeChildComponent(this->controller.get());
    }
    
    
    
    this->selector->mouseDownHandler(e);
    addAndMakeVisible(this->selector.get());
}

void InteractiveAutomation::mouseDoubleClick(const MouseEvent& e)
{
    this->currentMousePos = e.getEventRelativeTo(this).getPosition();
    
    Rectangle<float> area ( this->currentMousePos.getX(),
                           this->currentMousePos.getY(),
                           1,1);
    
    if(!isHitAnyVerteces(area)) createVertex(this->currentMousePos, true);
    
}

void InteractiveAutomation::mouseUp(const MouseEvent& e)
{
    this->selector->mouseUpHandler(e);
    removeChildComponent(this->selector.get());
    
    repaint();
    
}
void InteractiveAutomation::mouseMove(const MouseEvent& e)
{
    this->currentMousePos = e.getEventRelativeTo(this).getPosition();
    
    
}
void InteractiveAutomation::mouseDrag(const MouseEvent& e)
{
    this->selector->mouseDragHandler(e);
    repaint();
}
// ==================================================

void InteractiveAutomation::createVertex(Point<int> pos, bool isSelected)
{
    if(pos.getY() > this->MaxVertexValue) this->MaxVertexValue = pos.getY();
    if(pos.getY() < this->MinVertexValue) this->MinVertexValue = pos.getY();
    
    vertex* obj = new vertex (this);
    obj->setPosition(pos.toFloat());
    //obj->setCentrePosition(pos.getX(), pos.getY());
    obj->setSelected(isSelected);
    obj->addMouseListener(this, true);
    addAndMakeVisible(obj);
    this->verteces.add( obj );
    // sort
    ascendingSorter sorter;
    this->verteces.sort(sorter);
    
    repaint();
    repaintAllverteces();
}

void InteractiveAutomation::selectAllVerteces()
{
    for(auto v : this->selectedVerteces)
    {
        v->setSelected(true);
    }
    this->selectedVerteces.clear();
}

void InteractiveAutomation::deselectAllVerteces()
{
    for(auto v : this->selectedVerteces)
    {
        v->setSelected(false);
    }
    this->selectedVerteces.clear();
    
    reCalcPos();
}

void InteractiveAutomation::deleteSelectedVerteces()
{
    int i = (int) this->verteces.size() - 1;
    
    while (i >= 0)
    {
        if(this->verteces[i]->isSelected())
        {
            removeChildComponent(this->verteces[i]);
            this->verteces.remove(i);
        }
        
        i --;
    }
    
    reCalcPos();
    
    repaint();
}

void InteractiveAutomation::deleteAllVerteces()
{
    this->verteces.clear();
}

bool InteractiveAutomation::isHitAnyVerteces(Rectangle<float> area) const
{
    bool hitResult = false;
    
    for(auto v : this->verteces)
    {
        hitResult = v->hitTest(area);
        if(hitResult) return true;
    }
    
    return false;
}

void InteractiveAutomation::repaintAllverteces()
{
    for(auto v : this->verteces)
    {
        v->repaint();
    }
}

// ==================================================

void InteractiveAutomation::demoData(int num)
{
    
    float increment = (float) getWidth() / (float) num;
    this->verteces.clear();
    this->dataBuffer.clear();
    
    this->verteces.clear();
    for(int i = 0; i < num ; i++)
    {
        float value = (rand() % getHeight());
        createVertex(Point<float>((float)i * increment, value).roundToInt(), false);
        
        
        if(value < this->MinVertexValue) this->MinVertexValue = value;
        if(value > this->MaxVertexValue) this->MaxVertexValue = value;
    }
    
    deselectAllVerteces();
    //showVerteces();
    reCalcPos();
}


// ==================================================
/*
bool InteractiveAutomation::keyPressed(const KeyPress &key,
                                       Component* originatingComponent)
{
    int keyCode = key.getKeyCode();
    
    std::cout << "key code = " << keyCode << std::endl;
    if(keyCode == key.deleteKey || keyCode == key.backspaceKey)
    {
        deleteSelectedVerteces();
    }else if(key.getTextDescription() == "command + A")
    {
        for(auto v : this->verteces)
        {
            v->setSelected(true);
            v->repaint();
        }
    }
    
    return true;
}*/

void InteractiveAutomation::IRKeyPressed(int keyCode, String keyText)
{
    KeyPress key;

    std::cout << "key code = " << keyCode << std::endl;
    if(keyCode == key.deleteKey || keyCode == key.backspaceKey)
    {
        deleteSelectedVerteces();
    }else if(keyText == "command + A")
    {
        for(auto v : this->verteces)
        {
            v->setSelected(true);
            v->repaint();
        }
    }
    
}


void InteractiveAutomation::setMovable(bool movable, bool verticalMovable, bool horizontalMovable)
{
    this->selector->setMovableDirection(verticalMovable, horizontalMovable);
}

// ==================================================

void InteractiveAutomation::zoomInClicked()
{
    if(this->zoomInClickedCallback != nullptr) this->zoomInClickedCallback();
}
void InteractiveAutomation::zoomOutClicked()
{
    if(this->zoomOutClickedCallback != nullptr) this->zoomOutClickedCallback();
}
void InteractiveAutomation::movableClicked(IRAutomation::movableStatus status)
{
    if(this->movableClickedCallback != nullptr)
        this->movableClickedCallback(status);
}

// ==================================================

