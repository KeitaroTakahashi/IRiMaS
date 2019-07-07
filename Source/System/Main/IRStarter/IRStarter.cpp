
#include "IRStarter.hpp"

#include <random>





IRStarter::IRStarter(Rectangle<int> frameRect)
{
   
    int w = frameRect.getWidth();
    int h = frameRect.getHeight();
    
    setBounds(w / 2, h / 2, w, h);
    
    this->menuBar.reset(new MenuBarComponent(this));
    
#if JUCE_MAC
    
    setMenuBarPosition(MenuBarPosition::global); // for mac only. Windows takes window menu
    
#endif
    
    addAndMakeVisible(this->menuBar.get());
    setApplicationCommandManagerToWatch (&this->commandManager);
    
    this->commandManager.registerAllCommandsForTarget (this);
    
    addKeyListener(this->commandManager.getKeyMappings());

    this->editCommandTarget = std::make_unique<EditCommandTarget>(commandManager);
    this->editCommandTarget->addListener(this);
    
    addAndMakeVisible(this->editCommandTarget.get());
    
    init();
    setWantsKeyboardFocus(true);
    
}


IRStarter::~IRStarter()
{
    
#if JUCE_MAC
    MenuBarModel::setMacMainMenu(nullptr);
#endif
    
}


void IRStarter::paint(Graphics& g)
{
    g.fillAll(SYSTEMCOLOUR.background);
    g.drawImage(this->logo, this->logoPos);
}


void IRStarter::init()
{
    // randomly chose a logo from 15 variations.
    std::random_device rnd;
    String index = String(rnd() % 15 + 1);
    
    std::cout << "selected image logo = " << index << std::endl;
    
#if JUCE_MAC
    
    String url = "/Contents/Resources/materials/Images/Logo/logo_" + index + ".png";
    this->logo = loadImage(url);
    
#elif JUCE_IOS
    
    loadClef("/materials/scores/clefs/F-clef.png");
    
#endif
    
    if (this->logo.getWidth() == 0 || this->logo.getHeight() == 0)
    {
        std::cout << "Error : could not load image data of " << url << std::endl;
    }
    std::cout << "logo width = " << this->logo.getWidth() << std::endl;
    
    
    float w = 150.0f;
    float h = w * (static_cast<float>(this->logo.getHeight()) / static_cast<float>(this->logo.getWidth()));
    float x = static_cast<float>(getWidth()) / 2.0f - w / 2.0f;
    float y = static_cast<float>(getHeight()) / 2.0f - h / 2.0f;
    this->logoPos = Rectangle<float>(x, y, w, h);
}


Image IRStarter::loadImage(String url)
{
    File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + url);
    return ImageFileFormat::loadFrom(file);
}


StringArray IRStarter::getMenuBarNames()
{
    return { "File", "Edit" , "Window", "Help" };
}


PopupMenu IRStarter::getMenuForIndex(int menuIndex, const String& menuName)
{
    PopupMenu menu;
    
    if (menuIndex == 0)
    {
        menu.addCommandItem(&commandManager, CommandIDs::NewProject);
        menu.addCommandItem(&commandManager, CommandIDs::OpenProject);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, CommandIDs::CloseProject);
        menu.addCommandItem(&commandManager, CommandIDs::SaveProject);
        menu.addCommandItem(&commandManager, CommandIDs::RenameProject);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, CommandIDs::NewWorkspace);
    }
    else if (menuIndex == 1)
    {
        menu.addCommandItem(&commandManager, CommandIDs::EditMode);
        menu.addCommandItem(&commandManager, CommandIDs::LinkMode);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, CommandIDs::Undo);
        menu.addCommandItem(&commandManager, CommandIDs::Redo);
        menu.addSeparator();
        menu.addCommandItem(&commandManager, CommandIDs::Cut);
        menu.addCommandItem(&commandManager, CommandIDs::Copy);
        menu.addCommandItem(&commandManager, CommandIDs::Paste);
        menu.addCommandItem(&commandManager, CommandIDs::Duplicate);
    }
    else if (menuIndex == 2)
    {
        menu.addCommandItem(&commandManager, CommandIDs::menuPreferenceWindow);
        menu.addCommandItem(&commandManager, CommandIDs::fileInspecterWindow);
    }
    else if (menuIndex == 3)
    {
        
    }
    
    return menu;
}


void IRStarter::menuItemSelected (int /*menuItemID*/, int /*topLevelMenuIndex*/)
{
    
}


IRStarter::MenuActionStatus IRStarter::getMenuActionStatus() const
{
    return this->menu_action_status;
}



// **** **** PRIVATE METHODS **** **** //


void IRStarter::createNewProject()
{
    std::cout << "Creating new project... in Starter" << std::endl;
    this->menu_action_status = MenuActionStatus::CreateNewProjectAction;
    sendChangeMessage();
}


void IRStarter::openProject()
{
    std::cout << "Opening a project..." << std::endl;
    this->menu_action_status = MenuActionStatus::OpenProjectAction;
    sendChangeMessage();
}


void IRStarter::changeListenerCallback(ChangeBroadcaster* source)
{
    
}


void IRStarter::setMenuBarPosition(MenuBarPosition newPosition)
{
    if (menuBarPosition != newPosition)
    {
        menuBarPosition = newPosition;
        if (menuBarPosition != MenuBarPosition::burger)
        {
            sidePanel.showOrHide(false);
        }
        
#if JUCE_MAC
        
        MenuBarModel::setMacMainMenu (menuBarPosition == MenuBarPosition::global ? this : nullptr);
        
#endif
        
        menuBar->setVisible   (menuBarPosition == MenuBarPosition::window);
        menuItemsChanged();
        resized();
    }
}


ApplicationCommandTarget* IRStarter::getNextCommandTarget()
{
    return editCommandTarget.get();
}


void IRStarter::getAllCommands(Array<CommandID>&c)
{
    Array<CommandID> commands
    {
        CommandIDs::NewProject,
        CommandIDs::OpenProject,
        CommandIDs::CloseProject,
        CommandIDs::SaveProject,
        CommandIDs::RenameProject,
        CommandIDs::NewWorkspace
    };
    c.addArray (commands);
}


void IRStarter::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
    switch (commandID)
    {
        case CommandIDs::NewProject:
            result.setInfo("New Project", "Sets the outer colour to red", "File", 0);
            result.addDefaultKeypress('n', ModifierKeys::shiftModifier);
            result.setActive(true);
            break;
        case CommandIDs::OpenProject:
            result.setInfo("Open Project", "Sets the outer colour to red", "File", 0);
            result.addDefaultKeypress('o', ModifierKeys::commandModifier);
            result.setActive(true);
            break;
        case CommandIDs::CloseProject:
            result.setInfo("Close Project", "Sets the outer colour to red", "File", 0);
            result.addDefaultKeypress('w', ModifierKeys::commandModifier);
            result.setActive(false);
            break;
        case CommandIDs::SaveProject:
            result.setInfo("Save Project", "Sets the outer colour to red", "File", 0);
            result.addDefaultKeypress('s', ModifierKeys::commandModifier);
            result.setActive(false);
            break;
        case CommandIDs::RenameProject:
            result.setInfo("Rename Project", "Sets the outer colour to red", "File", 0);
            result.addDefaultKeypress('r', ModifierKeys::commandModifier);
            result.setActive(false);
            break;
        case CommandIDs::NewWorkspace:
            result.setInfo("New Workspace", "Sets the outer colour to red", "File", 0);
            result.addDefaultKeypress('n', ModifierKeys::commandModifier);
            result.setActive(false);
            break;
        default:
            break;
    }
}


bool IRStarter::perform(const InvocationInfo& info)
{
    switch (info.commandID)
    {
        case CommandIDs::NewProject:
            createNewProject();
            break;
        case CommandIDs::OpenProject:
            openProject();
            break;
        case CommandIDs::CloseProject:
            break;
        case CommandIDs::SaveProject:
            //saveProject();
            break;
        case CommandIDs::RenameProject:
            break;
        case CommandIDs::NewWorkspace:
            //createNewWorkspace();
            break;
            
        default:
            return false;
    }
    
    repaint();
    return true;
}




