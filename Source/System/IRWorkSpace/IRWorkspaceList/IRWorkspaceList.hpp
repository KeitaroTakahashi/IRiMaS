
#ifndef IRWorkspaceList_hpp
#define IRWorkspaceList_hpp

#include "JuceHeader.h"
#include "KeAnimationComponent.h"
#include "IRWorkSpace.hpp"





class IRWorkspaceList : public Component,
                        public IRComponents,
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
    
    IRWorkspaceList(Rectangle<int> frameRect);
    ~IRWorkspaceList();

    void resized() override;
    void paint(Graphics& g) override;
    
    void updateAnimationFrame() override;
    
    void updateList();
    
    void mouseDown(const MouseEvent& e) override;
    
    // this method is called when this object is focused by mouse down or other behaviors.
    // it is importnat to give Keyboard focus only on this obejct otherwise the keyboard event will be confused.
    void workspaceListFocused();

    void changeListenerCallback (ChangeBroadcaster* source) override;
    
    // return selected workspace component otherwise return nullptr
    Component* getSelectedComponent();
    
    void setSelectedComponentIndex(int index);
    int getWorkspaceCount() const { return this->workspaces.size(); }

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

    std::function<void()> removeWorkspaceCompleted; // FD note to myself - need to understand this decl. with no implementation
    
    void callRemoveWorkspaceAction(IRWorkSpace* workspace);
    
private:
    
    class ShowSnap : public Component,
                     public ChangeBroadcaster
    {
        
    public:
        
        ShowSnap(Rectangle<int> frameRect, int index, Component *parent)
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
            
            if (this->isSelected)
            {
                g.setColour (SYSTEMCOLOUR.contents);
                g.fillRoundedRectangle(area.toFloat(), 5.0);
                
                g.setColour (Colours::white);
                g.drawSingleLineText(String(this->index), this->marginX_right, this->snapRect.getHeight() - this->halfMarginY);
            }
            else
            {
                g.setColour (SYSTEMCOLOUR.fundamental);
                g.drawSingleLineText(String(this->index), this->marginX_right, this->snapRect.getHeight() - this->halfMarginY);
            }
    
            if (this->isBackgroundImage == true)
            {
                g.drawImage(this->backgroundImage, this->snapRect);
            }
        }
        
        // ========================================
        
        void updateImage()
        {
            
            std::cout << "updateImage = " << this->parent << std::endl;
            IRWorkSpace* space = dynamic_cast<IRWorkSpace* >(this->parent);
            if(space != nullptr)
            {
                Image img = space->getSnap();
                setImage(img);
                
            }
            
            std::cout << "updateImage done"<< std::endl;

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
        Component* parent = nullptr;

        // ========================================
        IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
        
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ShowSnap)
    };
    

public:
    
    void addWorkspace(IRWorkSpace* space);
    
    Array<IRWorkSpace*> getWorkspaceList();
    
    void removeWorkspace(ShowSnap* snap);
    
    void snapSelectionChange(ShowSnap* snap);
    
    listEventStatus getEventStatus() const;
    
private:
    
    bool keyPressed (const KeyPress& key,
                     Component* originatingComponent) override;
    
    Array<IRWorkSpace*> workspaces;
    Array<ShowSnap*> snapComponents;
    
    ShowSnap* currentlySelectedSnap = nullptr;
    ShowSnap* previouslySelectedSnap = nullptr;
    
    Component* selectedComponent = nullptr;

    listEventStatus eventStatus;
    Rectangle<int> frameRect;
    
    int marginX_left = 30;
    int marginX_right = 10;
    int marginY = 10;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRWorkspaceList)
};


#endif /* IRWorkspaceList_hpp */
