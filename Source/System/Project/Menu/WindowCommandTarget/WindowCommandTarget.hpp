
#ifndef WindowCommandTarget_hpp
#define WindowCommandTarget_hpp

#include "JuceHeader.h"

#include "Menus.h"



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


#endif /* WindowCommandTarget_hpp */
