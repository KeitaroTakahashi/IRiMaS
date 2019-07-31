//
//  IRShapeUI.cpp
//  IRiMaS
//
//  Created by Keitaro on 26/07/2019.
//

#include "IRShapeUI.hpp"

IRShapeUI::IRShapeUI(IRNodeObject* parent) : IRUIFoundation(parent),
colour(255,0, 0)
{
    this->preference = std::make_shared<IRShapePreference>("Shape Preference", Rectangle<int>(400, 720));
    this->preference->getUI()->addChangeListener(this);

}

IRShapeUI::~IRShapeUI()
{
    
}

//========================================================

void IRShapeUI::resized()
{
    
}

void IRShapeUI::paint(Graphics& g)
{
    
    g.fillAll(Colours::transparentWhite);
    g.setColour(this->colour);
    switch ( this->status)
    {
        case SQUARE:
            drawSquare(g);
            break;
        case CIRCLE:
            drawCircle(g);
            break;
        case TRIANGLE:
            drawTriangle(g);
            break;
        default:
            break;
    }
    
}

//========================================================

void IRShapeUI::drawSquare(Graphics& g)
{
    
    if(this->isFill)
        g.fillRect(getLocalBounds());
    else
        g.drawRect(getLocalBounds(),this->lineWidth);
}


void IRShapeUI::drawBorderSquare(Graphics& g)
{
    if(this->isFill)
        g.fillRect(getLocalBounds());
    else
        g.drawRect(getLocalBounds(),this->lineWidth);
    
}
void IRShapeUI::drawCircle(Graphics& g)
{
    Rectangle<float> area = getLocalBounds().toFloat();
    area.setX(area.getX() + this->lineWidth/2);
    area.setY(area.getY() + this->lineWidth/2);

    area.setWidth(area.getWidth() - this->lineWidth);
    area.setHeight(area.getHeight() - this->lineWidth);

    if(this->isFill)
        g.fillEllipse(area);
    else
        g.drawEllipse(area, this->lineWidth);
}
void IRShapeUI::drawTriangle(Graphics& g)
{

    Path p;
    
    p.startNewSubPath(this->lineWidth/2, this->lineWidth/2);
    p.lineTo(getWidth() - this->lineWidth/2, this->lineWidth/2);
    p.lineTo(getWidth()/2, getHeight() - this->lineWidth/2);
    p.closeSubPath();
    if(this->isFill)
        g.fillPath(p);
    else g.strokePath(p, PathStrokeType(this->lineWidth));
    
}

void IRShapeUI::changeListenerCallback(ChangeBroadcaster* source)
{
    auto gui = this->preference->getUI();
    
    if(source == gui)
    {
        IRShapePreferenceUI::IRShapePreferenceUIStatus status = gui->getStatus();
        
        if(status == IRShapePreferenceUI::IRShapePreferenceUIStatus::FillMenuSelected)
        {
            
            std::cout << "FillMenu = " << gui->getFillMenuIndex() << std::endl;
            if(gui->getFillMenuIndex() == 1)
            {
                this->isFill = true;
            }else if(gui->getFillMenuIndex() == 2)
            {
                this->isFill = false;
            }
        }else if(status == IRShapePreferenceUI::IRShapePreferenceUIStatus::ShapeMenuSelected)
        {
            std::cout << "selected shape = " << gui->getSelectedShapeIndex() << std::endl;
            switch(gui->getSelectedShapeIndex())
            {
                case 1:
                    this->status = SQUARE;
                    break;
                case 2:
                    this->status = CIRCLE;
                    break;
                case 3:
                    this->status = TRIANGLE;
                    break;
                default:
                    break;
            }
        }else if(status == IRShapePreferenceUI::IRShapePreferenceUIStatus::ColourChanged)
        {
            this->colour = gui->getColour();
        }else if(IRShapePreferenceUI::IRShapePreferenceUIStatus::BorderWidthChanged)
        {
            this->lineWidth = gui->getLineWidth();
        }
        repaint();
    }
}
