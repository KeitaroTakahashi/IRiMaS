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
    this->controller->setCommentEvent([this]{ commentClicked(); });
    this->controller->setBezierEvent([this](IRAutomation::lineStatus status){ bezierClicked(status); });

    setWantsKeyboardFocus(true);
    //addKeyListener(this);
    
    //addAndMakeVisible(&this->mouseGrid);

    
}

InteractiveAutomation::~InteractiveAutomation()
{
    
    this->verteces.clear();
    
    
}
// ==================================================


void InteractiveAutomation::paint(Graphics& g)
{
    g.fillAll(Colours::white);
    
    if(isBezierShow()) paintBezierLines(g);
    else drawLinesBetweenVerteces(g);
    //drawVerteces(g);
    
    if(isCommentShow()) paintComment(g);
    
}

void InteractiveAutomation::paintComment(Graphics& g)
{
    g.setColour(SYSTEMCOLOUR.contents);
    g.setOpacity(0.8);
    g.drawLine(0, this->currentMousePos.getY(), getWidth(), this->currentMousePos.getY(), 2);
    g.drawLine(this->currentMousePos.getX(), 0, this->currentMousePos.getX(), getHeight(), 2);
    
    // draw rect
    int rectX, rectY;
    if(this->currentMousePos.getX() > (this->visibleArea.getX() + this->visibleArea.getWidth()/2))
        rectX = this->currentMousePos.getX() - 50;
    else rectX = this->currentMousePos.getX();
    if(this->currentMousePos.getY() > (this->visibleArea.getY() + this->visibleArea.getHeight()/2))
        rectY = this->currentMousePos.getY() - 25;
    else rectY = this->currentMousePos.getY();
    
    g.fillRect(Rectangle<int>(rectX, rectY, 50, 25));
    
    Font f("Avenir Next",20, Font::plain);
    g.setColour(SYSTEMCOLOUR.text);
    g.setFont(f);
    g.drawText("test", rectX, rectY, 50, 25, Justification::right,true);
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

void InteractiveAutomation::paintBezierLines(Graphics& g)
{
    g.setColour(Colours::grey);

    for(int i = 1; i < this->verteces.size(); i++)
    {
        
        Path path;
        
        int wh = this->verteces[i-1]->getWidth()/2;
        int wh2 = this->verteces[i]->getWidth()/2;
        
        path.startNewSubPath(this->verteces[i-1]->getX() + wh,
                             this->verteces[i-1]->getY() + wh);
        
        float cx = this->verteces[i-1]->getX() +
        (this->verteces[i]->getX() - this->verteces[i-1]->getX()) * this->verteces[i-1]->getBezierRatio();
        
        Point<float> c1 (cx, this->verteces[i-1]->getY());
        Point<float> c2 (cx, this->verteces[i]->getY());
        Point<float> d (this->verteces[i]->getX() + wh2,
                        this->verteces[i]->getY() + wh2);
        
        path.cubicTo(c1, c2, d);
        
        g.strokePath(path, PathStrokeType(2));
    }
}

// ==================================================

void InteractiveAutomation::resized()
{
    
    if(getHeight() < 50) setBounds(getX(), getY(), getWidth(), 50);
    
    int s = getHeight();
    if(s > 50) s = 50;
    int y = getHeight() / 2 - s / 2;
    
    this->controller->setBounds(this->visibleArea.getX(), y, getWidth(), s);
    
    this->mouseGrid.setBounds(getLocalBounds());
    
    reCalcPos();
    
    this->previousBounds = getLocalBounds();
}

void InteractiveAutomation::setVisibleArea(Rectangle<int> area) {
    int s = getHeight();
    if(s > 50) s = 50;
    int y = getHeight() / 2 - s / 2;
    this->visibleArea = area;
    this->controller->setBounds(this->visibleArea.getX(), y, getWidth(), s);
    
    this->previousOffsetX = this->visibleArea.getX();
}


void InteractiveAutomation::reCalcPos()
{
    
    float boundsRatio_h;
    float boundsRatio_w;
    if(this->previousBounds.getHeight() > 0.0 && getLocalBounds().getHeight() > 0.0) boundsRatio_h = (float)getLocalBounds().getHeight() / (float)this->previousBounds.getHeight();
    else boundsRatio_h = 1.0;
    
    if(this->previousBounds.getWidth() > 0.0 && getLocalBounds().getWidth() > 0.0) boundsRatio_w = (float)getLocalBounds().getWidth() / (float)this->previousBounds.getWidth();
    else boundsRatio_w = 1.0;
    
    float increment = (float) getWidth() / (float) this->verteces.size();
    // draw point on each Vertex if increment is larger than 1 pixel.
    bool isPoint = true;
    if(increment < 1.0) isPoint = false;
    
    
    for(int i = 0; i < this->verteces.size(); i ++)
    {
        float y = (this->verteces[i]->getPosition().getY()) * boundsRatio_h;
        float x = (this->verteces[i]->getPosition().getX()) * boundsRatio_w;
        
       // std::cout << " x, y = " << this->verteces[i]->getPosition().getX() << ", " << this->verteces[i]->getPosition().getY() << " new x y = " << x << ", " << y << std::endl;

        this->verteces[i]->setPosition(Point<float>(x, y));
        
        if(isPoint) this->verteces[i]->setPoint(true);
        else this->verteces[i]->setPoint(false);
    }
    
    // if there is no verteces, then add two verteces with 0 values.
    if(this->verteces.size() == 0)
    {
        createVertex(Point<float>(0,getHeight()), false);
        createVertex(Point<float>(getWidth(),getHeight()), false);
    }
    
    repaint();
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
    
    if(!isHitAnyVerteces(area)) createVertex(this->currentMousePos.toFloat(), true);
    
}

void InteractiveAutomation::mouseUp(const MouseEvent& e)
{
    std::cout << "interactiveAutomation mouse up\n";

    this->selector->mouseUpHandler(e);
    removeChildComponent(this->selector.get());
    
    repaint();
    
    std::cout << "interactiveAutomation mouse up\n";
    
}
void InteractiveAutomation::mouseMove(const MouseEvent& e)
{
    this->currentMousePos = e.getEventRelativeTo(this).getPosition();
    repaint();
   // this->mouseGrid.setMousePosition(this->currentMousePos);
    
}
void InteractiveAutomation::mouseDrag(const MouseEvent& e)
{
    this->currentMousePos = e.getEventRelativeTo(this).getPosition();
    this->selector->mouseDragHandler(e);
    repaint();
}
// ==================================================

void InteractiveAutomation::createVertex(Point<float> pos, bool isSelected)
{
    if(pos.getY() > this->MaxVertexValue) this->MaxVertexValue = pos.getY();
    if(pos.getY() < this->MinVertexValue) this->MinVertexValue = pos.getY();
    
    vertex* obj = new vertex (this);
    obj->setPosition(pos);
    //obj->setCentrePosition(pos.getX(), pos.getY());
    obj->setSelected(isSelected);
    obj->addMouseListener(this, true);
    addAndMakeVisible(obj);
    this->verteces.add( obj );
    // sort
    ascendingSorter sorter;
    this->verteces.sort(sorter);
    
    int index = this->verteces.indexOf(obj);
    
    // bezier
    if(index > 0)
    {
        vertex* current = this->verteces[index];
        vertex* previous = this->verteces[index - 1];
        
        
        vertex* b1 = new vertex (this);
        vertex* b2 = new vertex (this);
        
        int x_b1 = previous->getPosition().getX() +
                   (current->getPosition().getX() -
                    previous->getPosition().getX())/2;
        int y_b1 = previous->getPosition().getY();
        
        Point<float> p1 (x_b1, y_b1);
        b1->setPosition(p1);
        
        int y_b2 = current->getPosition().getY();
        
        Point<float> p2 (x_b1, y_b2);
        b2->setPosition(p2);
        
        current->setBezier(b1, b2);
        
    }
   
    
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
    
    for(int i = 0; i < num ; i++)
    {
        float value = (rand() % getHeight());
        createVertex(Point<float>((float)i * increment, value), false);
        
        
        if(value < this->MinVertexValue) this->MinVertexValue = value;
        if(value > this->MaxVertexValue) this->MaxVertexValue = value;
    }
    
    deselectAllVerteces();
    //showVerteces();
    reCalcPos();
}

void InteractiveAutomation::setDescriptor(IRAnalysisDataStr* data)
{
    std::cout << "setDescriptor \n";

    int num = data->nframe;
    float increment = (float) getWidth() / (float) num;
    this->verteces.clear();
    this->dataBuffer.clear();
    
    float max = -999999;
    float min = 999999;
    
    for(int i = 0; i < num ; i++)
    {
        if(max < data->data[i]) max = data->data[i];
        if(min > data->data[i]) min = data->data[i];
    }
    
    std::cout << "max = " << max << " : " << min << std::endl;

    
    for(int i = 0; i < num ; i++)
    {
        
        float value = getHeight() - ((data->data[i] + min) / max) * getHeight();
        createVertex(Point<float>((float)i * increment, value), false);
    
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

void InteractiveAutomation::bezierClicked(IRAutomation::lineStatus status)
{
    
    switch (status)
    {
        case IRAutomation::lineStatus::LINEAR:
            setBezierShow(false);
            break;
        case IRAutomation::lineStatus::BEZIER:
            setBezierShow(true);
            break;
            
        default:
            break;
    }
}

void InteractiveAutomation::commentClicked()
{
    setCommentShow(!isCommentShow());
    
    std::cout << "comment show " << isCommentShow() << std::endl;
}

// ==================================================

