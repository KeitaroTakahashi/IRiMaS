//
//  IRWorkspaceList.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 12/11/2018.
//

#ifndef IRWorkspaceList_hpp
#define IRWorkspaceList_hpp
#include "JuceHeader.h"
#include "KeAnimationComponent.h"
#include "IRWorkSpace.hpp"

class IRWorkspaceList : public Component,
public ChangeBroadcaster,
public ChangeListener,
public KeyListener,
public KeAnimationComponent
{
public:
    
    enum listEventStatus
    {
        listEventSelected,
        listEventDeleted,
        listEventCopied,
        listEventPasted,
        listEventDuplicated
    };
    IRWorkspaceList(Rectangle<int> frameRect)
    {
        this->frameRect = frameRect;
        
        addKeyListener(this);
        
        setBounds(frameRect);
    }
    
    ~IRWorkspaceList()
    {
        
    }
    
    // ========================================

    void resized() override
    {
        
    }
    
    void paint(Graphics& g) override
    {
        g.fillAll(SYSTEMCOLOUR.contents.brighter());
    
        
        
    }
    
    void updateAnimationFrame() override
    {
        updateList();
    }
    // ========================================

    
    
    void updateList()
    {
        // get index of the selected workspace
        int selectedIndex = -1;
        if(this->currentlySelectedSnap != nullptr)
        {
            selectedIndex = this->currentlySelectedSnap->getIndex() - 1;
        }
        
        removeAllChildren();
        this->snapComponents.clear();
        
        int index = 1;
        int doubleX = this->marginX_left + this->marginX_right;
        
        int w = getWidth() - doubleX;
        int h = w * 4 / 6 + this->marginY*2; // 6:4
        for(auto space : this->workspaces)
        {
            
            int ht = (index-1)*h;

            Rectangle<int> rect(0,
                               ht,
                               getWidth(),       h);
            
            showSnap *s = new showSnap(rect, index, space);
            s->updateImage();
            s->addChangeListener(this);
            this->snapComponents.add(s);
            addAndMakeVisible(s);
            
            index++;
        }
        
        if(selectedIndex >= 0)
        {
            this->snapComponents[selectedIndex]->listEventSelectedAction();
        }
        
        // IMPORTANT!!
        // give back keyboard focus on this object.
        workspaceListFocused();
        
        repaint();
    }
    // ========================================
    void mouseDown(const MouseEvent& e) override
    {
        std::cout << "WorkspaceList mouse down\n";
        
        workspaceListFocused();
    }
    
    // this method is called when this object is focused by mouse down or other behaviors.
    // it is importnat to give Keyboard focus only on this obejct otherwise the keyboard event will be confused.
    void workspaceListFocused()
    {
        setWantsKeyboardFocus(true);
        grabKeyboardFocus();
    }
    
    
    // ========================================

    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        for (auto snap : this->snapComponents)
        {
            if(source == snap){

                switch (snap->getEventStatus())
                {
                    case listEventSelected:
                        std::cout << "IRWorkspaceList : listEventSelected ChangeCallba\n";
                        
                        snapSelectionChange(snap);
                        
                        

                        break;
                    case listEventDeleted:
                        break;
                    default:
                        break;
                }
                
                
            }
        }
    }
    
    // return selected workspace component otherwise return nullptr
    Component* getSelectedComponent() { return this->selectedComponent; }
    
    void setSelectedComponentIndex(int index)
    {
        if(index < this->snapComponents.size())
        {
            snapSelectionChange(this->snapComponents[index]);
        }else{
            std::cout << "Error : setSelectedComponentIndex() could not set index of " << index << "to snapComponents. Out of range.\n";
        }
    }
    
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        virtual void removeWorkspace(IRWorkSpace* workspace) = 0;
    };
    
    virtual void addListener(Listener* newListener) { this->listeners.add(newListener); }
    virtual void removeListener(Listener* listener) { this->listeners.remove(listener);}
    ListenerList<Listener>& getListenerList() { return this->listeners; }
    ListenerList<Listener> listeners;

    std::function<void()> removeWorkspaceCompleted;
    
    void callRemoveWorkspaceAction(IRWorkSpace* workspace)
    {
        
        
        Component::BailOutChecker checker(this);
        //==========
        // check if the objects are not deleted, if deleted, return
        if(checker.shouldBailOut()) return;
        this->listeners.callChecked(checker, [workspace](Listener& l){ l.removeWorkspace(workspace); });
        //check again
        if(checker.shouldBailOut()) return;
        //std::function
        if(this->removeWorkspaceCompleted != nullptr) this->removeWorkspaceCompleted();

    }
    
private:
    
    class showSnap : public Component,
    public ChangeBroadcaster
    {
    public:
        showSnap(Rectangle<int> frameRect, int index, Component *parent)
        {
            setBounds(frameRect);
            this->parent = parent;
            
            this->index = index;
            
            float doubleX = float(this->marginX_left + this->marginX_right);
            float w = float(getWidth() - doubleX);
            float h = w * 4.0 / 6.0; // 6:4
            
            this->snapRect = Rectangle<float>(this->marginX_left,
                                              this->marginY,
                                              w,
                                              h);
            
            setWantsKeyboardFocus(true);

            
        }
        // ========================================
        void resized() override{}
        
        void paint(Graphics& g) override
        {
            auto area = getLocalBounds();//.reduced (0);
            
            if(this->isSelected){
                g.setColour (SYSTEMCOLOUR.contents);
                g.fillRoundedRectangle(area.toFloat(), 5.0);
                
                g.setColour (Colours::white);
                g.drawSingleLineText(String(this->index), this->marginX_right, this->snapRect.getHeight() - this->halfMarginY);
            }else{
                g.setColour (SYSTEMCOLOUR.fundamental);
                g.drawSingleLineText(String(this->index), this->marginX_right, this->snapRect.getHeight() - this->halfMarginY);
            }
    
            if(this->isBackgroundImage == true){
                g.drawImage(this->backgroundImage, this->snapRect);
            }
        }
        
        // ========================================
        
        void updateImage()
        {
            IRWorkSpace* space = dynamic_cast<IRWorkSpace* >(this->parent);
            if(space != nullptr)
            {
                Image img = space->getSnap();
                setImage(img);
                
            }
        }
        void resize(Rectangle<int> frameRect)
        {
            setBounds(frameRect);
        }
        void setImage(Image& img)
        {
            this->backgroundImage = img;
            this->isBackgroundImage = true;
            repaint();
        }
        
        void setSelected(bool flag)
        {
            this->isSelected = flag;
        }
        
        int getIndex() const { return this->index; }
        // ========================================
        void mouseDown (const MouseEvent& event) override
        {
            listEventSelectedAction();
        }
        
        void listEventSelectedAction()
        {
            // fire changeBroadCaster when this item is selected.
            this->eventStatus = listEventSelected;
            sendChangeMessage();
        }
        
        // ========================================
        
        listEventStatus getEventStatus() const { return this->eventStatus; }
        Component* getParent() { return this->parent; }

    private:
        Image backgroundImage;
        bool isBackgroundImage = false;
        listEventStatus eventStatus;
        
        bool isSelected = false;
        
        Rectangle<float> snapRect;
        
        int marginX_left = 30;
        int marginX_right = 10;
        int marginY = 10;
        int halfMarginY = 5;
        
        int index;
        
        
        // workspace corresponding to this snap component
        Component* parent;

        // ========================================
        IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
    };
    
    // ============================================================

public:
    void addWorkspace(IRWorkSpace* space)
    {
        this->workspaces.add(space);
        updateList();
    }
    
    Array<IRWorkSpace*> getWorkspaceList() { return this->workspaces; }
    
    
    void removeWorkspace(showSnap* snap)
    {
        if(snap == nullptr) return;
        
        int removeIndex = snap->getIndex() - 1;
        
        if(removeIndex >= 0){
            
            callRemoveWorkspaceAction(this->workspaces[removeIndex]);
            this->workspaces.remove(removeIndex);
        }
        delete snap;
        
        
        updateList();
    }
    
    void snapSelectionChange(showSnap* snap)
    {
        if(this->currentlySelectedSnap != nullptr){
            this->previouslySelectedSnap = this->currentlySelectedSnap;
            this->previouslySelectedSnap->updateImage();
            this->previouslySelectedSnap->setSelected(false);
        }
        this->currentlySelectedSnap = snap;
        this->currentlySelectedSnap->setSelected(true);
        
        this->selectedComponent = snap->getParent();
        
        this->eventStatus = listEventSelected;
        sendChangeMessage();
        
    }
    
    listEventStatus getEventStatus() const { return this->eventStatus; }
private:
    // ============================================================
    bool keyPressed (const KeyPress& key,
                     Component* originatingComponent) override;
    
    
    // ============================================================
    
    Array<IRWorkSpace*> workspaces;
    Array<showSnap*> snapComponents;
    
    showSnap* currentlySelectedSnap = nullptr;
    showSnap* previouslySelectedSnap = nullptr;
    
    Component* selectedComponent = nullptr;

    listEventStatus eventStatus;
    Rectangle<int> frameRect;
    
    int marginX_left = 30;
    int marginX_right = 10;
    int marginY = 10;
    
    
    // ========================================
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

};


#endif /* IRWorkspaceList_hpp */
