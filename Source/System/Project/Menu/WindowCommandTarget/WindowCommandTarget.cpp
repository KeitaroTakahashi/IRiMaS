
#include "WindowCommandTarget.hpp"




WindowCommandTarget::WindowCommandTarget(ApplicationCommandManager& m) : commandManager(m)
{
    commandManager.registerAllCommandsForTarget(this);
}


void WindowCommandTarget::resized()
{
    
}


void WindowCommandTarget::paint(Graphics& g)
{
    
}


ApplicationCommandTarget* WindowCommandTarget::getNextCommandTarget()
{
    return findFirstTargetParentComponent();
}


void WindowCommandTarget::getAllCommands(Array<CommandID>& c)
{
    Array<CommandID> commands { CommandIDs::menuPreferenceWindow};
    c.addArray (commands);
}


void WindowCommandTarget::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
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


bool WindowCommandTarget::perform(const InvocationInfo& info)
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




