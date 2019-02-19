//
//  IRSpectrogramUI.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 05/09/2018.
//

#ifndef IRSpectrogramUI_h
#define IRSpectrogramUI_h

#include "IRSpectrogram.h"
#include "IRNodeObjectSelector.hpp"
#include "IRGraphSelector.hpp"

class IRSpectrogramUI : public IRSpectrogram,
                        public IRNodeObject::Listener,
                        public KeyListener
{
public:
    IRSpectrogramUI() : IRSpectrogram()
    {
        this->selector = new IRGraphSelector();
        // only horizontal axis is movable and height follows the object height.
        this->selector->setSelectionMode(true, false);
        this->selector->setParentSize(getWidth(), getHeight());
        
    }
    ~IRSpectrogramUI() {}
    // ============================================================
    // ------------------------------------------------------------
    void resized() override
    {
        IRSpectrogram::resized();
        
        // ----------
        this->selector->setParentSize(getWidth(), getHeight());
        for(auto obj : this->selectionSquareObjects)
        {
            obj->renewBounds();
        }
        // ----------
        this->previousWidth = getWidth();
        this->previousHeight = getHeight();
    }
    // ============================================================
    bool isSelectMode() const { return this->selectModeFlag; }
    // ------------------------------------------------------------
    void setEditMode(bool flag)
    {
        this->editModeFlag = flag;
 
        // change edit mode for
        for(auto obj : this->selectionSquareObjects)
        {
            obj->setEditMode(flag);
        }
        
        
        // if not editMode, get keyboardFocus
        if(! flag){
            setWantsKeyboardFocus(true);
            addKeyListener(this);

        }else{ // otherwise out of keyboard Focus and initialize dragdropCalled flag
            setWantsKeyboardFocus(false);
            removeKeyListener(this);
            this->dragdropCalled = false;// selection

        }
    }
    // ------------------------------------------------------------
    bool isEditMode() const { return this->editModeFlag; }
    
    // ============================================================
    void mouseDown(const MouseEvent& e) override
    {
        this->previousWidth = getWidth();
        this->previousHeight = getHeight();
        
        std::cout << "spectrogramUI mouseDown\n";
        mouseDownHandler(e);
        
        // play selected area
        //playSelectedArea();
    }
    void mouseDownHandler(const MouseEvent& e);
    // ------------------------------------------------------------
    void mouseUp(const MouseEvent& e) override
    {
        if(this->selectModeFlag && isSelectMode())
        {
            createSquareObject(this->selector->getBounds());
            this->selector->mouseUpHandler(e);
            this->selectModeFlag = false;
        }
        
        mouseUpHandler(e);
    }
    void mouseUpHandler(const MouseEvent& e);
    // ------------------------------------------------------------
    void mouseDrag(const MouseEvent& e) override
    {
        if(this->selectModeFlag && isSelectMode())
        {
            this->selector->mouseDragHandler(e);
        }
        
        mouseDragHandler(e);
    }
    void mouseDragHandler(const MouseEvent& e);
    // ------------------------------------------------------------
    void createSquareObject(Rectangle<int> rect)
    {
        if(rect.getWidth() > 1)
        {
            IRMultiPurposeObject* obj = new IRMultiPurposeObject(this);
            //obj->addListener(this->parentListener);
            obj->setEditMode(isEditMode());
            obj->setBoundsRatio(true);
            obj->setMovable(false,false,false);
            addAndMakeVisible(obj);
            
            float x = (float)rect.getX() / (float)getWidth();
            float w = (float)rect.getWidth() / (float)getWidth();
            if(x < 0){ // when mouse drag goes to the outside of the parent component, x becomes below 0.
                w += x; // fix
                x = 0.0;
            }
            
            float y = 0.0;
            if((w+x) > 1.0) w = 1.0 - x;
            
            float h = 1.0;
            
            obj->setBoundsInRatio(x, y, w, h);
            obj->renewBounds();
            this->selectionSquareObjects.add(obj);
        }
    }
    // ------------------------------------------------------------
    void addSquareObject(IRMultiPurposeObject* obj)
    {
        obj->setEditMode(isEditMode());
        obj->setBoundsRatio(true);
        obj->setMovable(false,false,false);
        addAndMakeVisible(obj);
        this->selectionSquareObjects.add(obj);
    }
    // ------------------------------------------------------------
    void deleteSquareObject()
    {
        std::cout << "delete\n";
        for(auto obj : this->selectedSquareObjectList)
        {
            
            int index = this->selectionSquareObjects.indexOf(obj);
            if(index >= 0) this->selectionSquareObjects.remove(index);
            removeChildComponent(obj);
        }
        this->selectedSquareObjectList.clear();
        
        // stop playing
        //stopPlaying();
        
    }
    // ------------------------------------------------------------
    void deselectAllSquareObject()
    {
        std::cout << "deselected all square object\n";
        for(auto obj : this->selectionSquareObjects)
        {
            if(obj->isSelected()) obj->setSelected(false);
            obj->repaint();
        }
        this->selectedSquareObjectList.clear();
        
    }
    // ------------------------------------------------------------
    void addSelectedObjects()
    {
        //std::cout << "addSelectedObjects() :: " << std::endl;
        
        this->selectedSquareObjectList.clear();
        for(auto obj : this->selectionSquareObjects)
        {
            //std::cout << obj << " : selected? = " << obj->isSelected() << std::endl;
            if(obj->isSelected())
            {
                this->selectedSquareObjectList.add(obj);
            }
        }
    }
    
    // ============================================================
    // listener IRNodeObject
    // This object takes an exclusive listener which is expected to be Workspace.
    void setListener(IRNodeObject::Listener* newListener)
    {
        this->parentListener = newListener;
        
        // replace listener
        for(auto obj : this->selectedSquareObjectList)
        {
            obj->getListenerList().clear();
            obj->addListener(newListener);
        }
    }
    // ============================================================
    
    bool keyPressed (const KeyPress& key,
                     Component* originatingComponent) override
    {
        std::cout << "IRWaveformUI keyPressed : " << key.getKeyCode() << std::endl;
        if(key.getKeyCode() == key.deleteKey || key.getKeyCode() == key.backspaceKey)
        {
            if(! isEditMode())
                deleteSquareObject();
        }
        
        if(key.getTextDescription() == "command + E")
        {
            this->status = EDITMODECHANGE;
            sendChangeMessage();
            
        }
        return true;
    }
    // ============================================================
    
    enum UISTATUS
    {
        DRAGOUT,
        DROPOUT,
        EDITMODECHANGE,
        PROJECTSAVE
    };
    
    UISTATUS status;

    Array<IRMultiPurposeObject* > selectionSquareObjects;
    Array<IRMultiPurposeObject* > selectedSquareObjectList;
    // ============================================================
private:
    IRGraphSelector* selector;
    
    IRNodeObject::Listener* parentListener;

    
    // drag drop action
    bool dragdropCalled = false;

    bool selectFlag = true;
    bool selectModeFlag = false;
    bool editModeFlag = true;

    int previousWidth = 0;
    int previousHeight = 0;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRSpectrogramUI)
};

#endif /* IRSpectrogramUI_h */
