
#ifndef Menus_h
#define Menus_h

enum CommandIDs
{
    NewProject = 1, // Should be 1
    OpenProject,
    CloseProject,
    SaveProject,
    RenameProject,
    
    NewWorkspace,

    // Edit
    EditMode,
    Undo,
    Redo,
    Cut,
    Copy,
    Paste,
    Duplicate,
    
    //Window
    menuPreferenceWindow
};
//==============================================================================

class WindowCommandTarget : public Component,
public ApplicationCommandTarget
{
public:
    WindowCommandTarget(ApplicationCommandManager& m) : commandManager(m)
    {
        commandManager.registerAllCommandsForTarget(this);
    }
    
    void resized() override
    {
        
    }
    
    void paint(Graphics& g)override
    {
        
    }
    
    //==============================================================================
    ApplicationCommandTarget* getNextCommandTarget() override
    {
        return findFirstTargetParentComponent();
    }
    
    void getAllCommands(Array<CommandID>& c) override
    {
        Array<CommandID> commands { CommandIDs::menuPreferenceWindow};
        c.addArray (commands);
    }
    
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override
    {
        
        switch (commandID)
        {
            case CommandIDs::menuPreferenceWindow:
                result.setInfo ("Preference Window", "Sets the outer colour to red", "Window", 0);
                result.addDefaultKeypress ('p', ModifierKeys::shiftModifier);
                break;
                
            default:
                break;
        }
    }
    
    bool perform (const InvocationInfo& info) override
    {
        switch(info.commandID)
        {
            case CommandIDs::menuPreferenceWindow:
                break;
                
            default:
                return false;
        }
        repaint();
        return true;
    }
    
private:
    ApplicationCommandManager& commandManager;
    
};
//==============================================================================

class EditCommandTarget : public Component,
public ApplicationCommandTarget
{
public:
    EditCommandTarget(ApplicationCommandManager& m)
    : commandManager(m),
      windowCommandTarget(commandManager)
    {
        printf("EditCommandTarget\n");
        commandManager.registerAllCommandsForTarget(this);
        
        addAndMakeVisible(this->windowCommandTarget);
    }
    
    void resized() override
    {
        this->windowCommandTarget.setBounds(getLocalBounds().reduced (50));
    }
    
    void paint(Graphics& g)override
    {
        
    }
    
    //==============================================================================
    ApplicationCommandTarget* getNextCommandTarget() override
    {
        return &this->windowCommandTarget;
    }
    
    void getAllCommands(Array<CommandID>& c) override
    {
        Array<CommandID> commands {
            CommandIDs::EditMode,
            CommandIDs::Undo,
            CommandIDs::Redo,
            CommandIDs::Cut,
            CommandIDs::Copy,
            CommandIDs::Paste,
            CommandIDs::Duplicate
        };
        c.addArray (commands);
    }
    
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override
    {
        
        switch (commandID)
        {
            case CommandIDs::EditMode:
                result.setInfo ("EditMode", "Switch Edit Mode", "Edit", 0);
                result.addDefaultKeypress ('e', ModifierKeys::commandModifier);
                break;
            case CommandIDs::Undo:
                result.setInfo ("Undo", "Sets the outer colour to red", "Edit", 0);
                result.addDefaultKeypress ('z', ModifierKeys::commandModifier);
                break;
            case CommandIDs::Redo:
                result.setInfo ("Redo", "Sets the outer colour to red", "Edit", 0);
                result.addDefaultKeypress ('z', ModifierKeys::shiftModifier);
                break;
            case CommandIDs::Cut:
                result.setInfo ("Cut", "Sets the outer colour to red", "Edit", 0);
                result.addDefaultKeypress ('x', ModifierKeys::commandModifier);
                break;
            case CommandIDs::Copy:
                result.setInfo ("Copy", "Sets the outer colour to red", "Edit", 0);
                result.addDefaultKeypress ('c', ModifierKeys::commandModifier);
                break;
            case CommandIDs::Paste:
                result.setInfo ("Paste", "Sets the outer colour to red", "Edit", 0);
                result.addDefaultKeypress ('v', ModifierKeys::commandModifier);
                break;
            case CommandIDs::Duplicate:
                result.setInfo ("Duplicate", "Sets the outer colour to red", "Edit", 0);
                result.addDefaultKeypress ('d', ModifierKeys::commandModifier);
                break;
                
            default:
                break;
        }
    }
    
    bool perform (const InvocationInfo& info) override
    {
        switch(info.commandID)
        {
            case CommandIDs::EditMode:
                callPerformEditModeChange();
                break;
            case CommandIDs::Undo:
                break;
            case CommandIDs::Redo:
                break;
            case CommandIDs::Cut:
                break;
            case CommandIDs::Copy:
                break;
            case CommandIDs::Paste:
                break;
            case CommandIDs::Duplicate:
                break;
            default:
                return false;
        }
        repaint();
        return true;
    }
    
    // ===========================================================================
    // Listener
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        //write methods called by IRNodeComponent objects.
        virtual void performEditModeChange() {};
        
    };
    
    virtual void addListener(Listener* newListener) { this->listeners.add(newListener); }
    virtual void removeListener(Listener* listener) { this->listeners.remove(listener);}
    
    // ---------------------------------------------------------------------------
    std::function<void()> performEditModeChangeCompleted;
    // fire performEditModeChange() methods in Listener
    void callPerformEditModeChange()
    {
        std::cout << "callPerformEditModeChange : " << std::endl;
        Component::BailOutChecker checker(this);
        //==========
        // check if the objects are not deleted, if deleted, return
        if(checker.shouldBailOut()) return;
        this->listeners.callChecked(checker, [this](Listener& l){ l.performEditModeChange(); });
        //check again
        if(checker.shouldBailOut()) return;
        //std::function
        if(this->performEditModeChangeCompleted != nullptr) this->performEditModeChangeCompleted();
        
    }
    
    // ===========================================================================

private:
    ApplicationCommandManager& commandManager;
    
    ListenerList<Listener> listeners;
    
    WindowCommandTarget windowCommandTarget;
    
};
//==============================================================================



#endif /* Menus_h */
