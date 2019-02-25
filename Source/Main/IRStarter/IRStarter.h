//
//  IRStarter.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 16/01/2019.
//

#ifndef IRStarter_h
#define IRStarter_h

#include "JuceHeader.h"
#include "Menus.h"
#include "IRImageViewer.h"

#include <random>

/*
    IRMAIN
    |
    IRStartWindow
    |
    IRStarter
 
 */
class IRStarter : public Component,
public ApplicationCommandTarget,
public MenuBarModel,
public ChangeListener,
public ChangeBroadcaster,
private EditCommandTarget::Listener
{
public:
    enum class MenuBarPosition
    {
        window, // for windows
        global, // for mac
        burger
    };
    
    enum MenuActionStatus
    {
        CreateNewProjectAction,
        OpenProjectAction
    };
    
    IRStarter(Rectangle<int> frameRect)
    {
        // setup systme colour
        IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
        SYSTEMCOLOUR.set(IR::darkBrownSet());
        
        int w = frameRect.getWidth();
        int h = frameRect.getHeight();
        
        setBounds (w/2, h/2, w, h);

        // menubar
        this->menuBar.reset(new MenuBarComponent(this));
        
        // for mac only. Windows takes window menu
#if JUCE_MAC
        setMenuBarPosition(MenuBarPosition::global);
#endif
        addAndMakeVisible(this->menuBar.get());
        setApplicationCommandManagerToWatch (&this->commandManager);
        
        this->commandManager.registerAllCommandsForTarget (this);
        
        addKeyListener(this->commandManager.getKeyMappings());
        this->editCommandTarget = new EditCommandTarget( commandManager );
        this->editCommandTarget->addListener(this);
        
        addAndMakeVisible(this->editCommandTarget);
        
        init();
        setWantsKeyboardFocus(true);
        
    }
    
    ~IRStarter()
    {
        
#if JUCE_MAC
        MenuBarModel::setMacMainMenu (nullptr);
#endif
    }
    
    // =======================================================
    void paint(Graphics& g)override
    {
        g.fillAll(SYSTEMCOLOUR.background);
        g.drawImage(this->logo,this->logoPos);
    }
    // =======================================================

    void init()
    {
        // randomly chose a logo from 15 variations.
        std::random_device rnd;
        String index = String(rnd()%15 + 1);
        
        std::cout << "selected image logo = " << index << std::endl;
#if JUCE_MAC
        String url = "/Contents/Resources/materials/Images/Logo/logo_" + index + ".png";
        this->logo = loadImage(url);
#elif JUCE_IOS
        loadClef("/materials/scores/clefs/F-clef.png");
#endif
        
        if(this->logo.getWidth() == 0 || this->logo.getHeight() == 0)
        {
            std::cout << "Error : could not load image data of " << url << std::endl;
        }
        std::cout << "logo width = " << this->logo.getWidth() << std::endl;
        
        
        
        float w = 150;
        float h = w * ((float)this->logo.getHeight() / (float)this->logo.getWidth());
        float x = (float)(getWidth()) / 2.0 - w/2.0;
        float y = (float)(getHeight()) / 2.0 - h/2.0;
        this->logoPos = Rectangle<float>(x,y,w,h);
        
        
        
    }
    
    Image loadImage(String url)
    {
        File file = File(File::getSpecialLocation(File::currentApplicationFile).getFullPathName() + url);
        return ImageFileFormat::loadFrom(file);
        
    }
    
    
    // ========================================
    StringArray getMenuBarNames() override
    {
        return { "File", "Edit" , "Window", "Help" };
    }
    
    
    PopupMenu getMenuForIndex(int menuIndex, const String& menuName) override
    {
        PopupMenu menu;
        
        if(menuIndex == 0)
        {
            menu.addCommandItem(&commandManager, CommandIDs::NewProject);
            menu.addCommandItem(&commandManager, CommandIDs::OpenProject);
            menu.addSeparator();
            menu.addCommandItem(&commandManager, CommandIDs::CloseProject);
            menu.addCommandItem(&commandManager, CommandIDs::SaveProject);
            menu.addCommandItem(&commandManager, CommandIDs::RenameProject);
            menu.addSeparator();
            menu.addCommandItem(&commandManager, CommandIDs::NewWorkspace);
            
        }else if(menuIndex == 1)
        {
            menu.addCommandItem(&commandManager, CommandIDs::EditMode);
            menu.addSeparator();
            menu.addCommandItem(&commandManager, CommandIDs::Undo);
            menu.addCommandItem(&commandManager, CommandIDs::Redo);
            menu.addSeparator();
            menu.addCommandItem(&commandManager, CommandIDs::Cut);
            menu.addCommandItem(&commandManager, CommandIDs::Copy);
            menu.addCommandItem(&commandManager, CommandIDs::Paste);
            menu.addCommandItem(&commandManager, CommandIDs::Duplicate);
            
        }else if(menuIndex == 2)
        {
            menu.addCommandItem(&commandManager, CommandIDs::menuPreferenceWindow);
            
        }else if(menuIndex == 3)
        {
            
        }
        
        return menu;
    }
    
    void menuItemSelected (int /*menuItemID*/, int /*topLevelMenuIndex*/) override {}
    
    
    MenuActionStatus getMenuActionStatus() const { return this->menu_action_status; }
    
    
private:
    
    void createNewProject()
    {
        printf("Creating new project... in Starter\n");
        this->menu_action_status = MenuActionStatus::CreateNewProjectAction;
        sendChangeMessage();
    }
    
    void openProject()
    {
        printf("Opening a project...\n");
        this->menu_action_status = MenuActionStatus::OpenProjectAction;
        sendChangeMessage();
    }
    
    //==============================================================================

    void changeListenerCallback (ChangeBroadcaster* source) override
    {
    }
    //==============================================================================
    
    void setMenuBarPosition ( MenuBarPosition newPosition )
    {
        if(menuBarPosition != newPosition)
        {
            menuBarPosition = newPosition;
            if(menuBarPosition != MenuBarPosition::burger)
                sidePanel.showOrHide(false);
            
#if JUCE_MAC
            MenuBarModel::setMacMainMenu (menuBarPosition == MenuBarPosition::global ? this : nullptr);
#endif
            menuBar->setVisible   (menuBarPosition == MenuBarPosition::window);
            menuItemsChanged();
            resized();
        }
    }
    
    ApplicationCommandTarget* getNextCommandTarget() override
    {
        return editCommandTarget;
    }
    
    void getAllCommands(Array<CommandID>&c) override
    {
        Array<CommandID> commands { CommandIDs::NewProject,
            CommandIDs::OpenProject,
            CommandIDs::CloseProject,
            CommandIDs::SaveProject,
            CommandIDs::RenameProject,
            CommandIDs::NewWorkspace };
        c.addArray (commands);
    }
    void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override
    {
        switch (commandID)
        {
            case CommandIDs::NewProject:
                result.setInfo ("New Project", "Sets the outer colour to red", "File", 0);
                result.addDefaultKeypress ('n', ModifierKeys::shiftModifier);
                result.setActive(true);
                break;
            case CommandIDs::OpenProject:
                result.setInfo ("Open Project", "Sets the outer colour to red", "File", 0);
                result.addDefaultKeypress ('o', ModifierKeys::commandModifier);
                result.setActive(true);
                break;
            case CommandIDs::CloseProject:
                result.setInfo ("Close Project", "Sets the outer colour to red", "File", 0);
                result.addDefaultKeypress ('w', ModifierKeys::commandModifier);
                result.setActive(false);
                break;
                
            case CommandIDs::SaveProject:
                result.setInfo ("Save Project", "Sets the outer colour to red", "File", 0);
                result.addDefaultKeypress ('s', ModifierKeys::commandModifier);
                result.setActive(false);
                break;
            case CommandIDs::RenameProject:
                result.setInfo ("Rename Project", "Sets the outer colour to red", "File", 0);
                result.addDefaultKeypress ('r', ModifierKeys::commandModifier);
                result.setActive(false);
                break;
                
            case CommandIDs::NewWorkspace:
                result.setInfo ("New Workspace", "Sets the outer colour to red", "File", 0);
                result.addDefaultKeypress ('n', ModifierKeys::commandModifier);
                result.setActive(false);
                break;
                
            default:
                break;
        }
    }
    
    bool perform(const InvocationInfo& info) override
    {
        switch(info.commandID)
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
    
    //==============================================================================
    
    Image logo;
    Rectangle<float> logoPos;
    Image title;
    
    
    IRImageViwer imageViewer;
    //======================================================================
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    
    // menubar
    ApplicationCommandManager commandManager;
    std::unique_ptr<MenuBarComponent> menuBar;
    MenuBarPosition menuBarPosition = MenuBarPosition::window;
    EditCommandTarget* editCommandTarget;
    SidePanel sidePanel { "Menu", 300, false };
    
    MenuActionStatus menu_action_status;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRStarter);

    
};

class IRStartWindow : public DocumentWindow,
public ChangeBroadcaster,
public ChangeListener
{
public:
    IRStartWindow(String name, Rectangle<int> frameRect)  :
    DocumentWindow (name,
                    Desktop::getInstance().getDefaultLookAndFeel()
                    .findColour (ResizableWindow::backgroundColourId),
                    DocumentWindow::allButtons)
    {
        // do not show a title bar
        setUsingNativeTitleBar (false);
        setTitleBarHeight(0);

        // setup systme colour
        IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();
        SYSTEMCOLOUR.set(IR::darkBrownSet());
        
        Rectangle<int> r = Desktop::getInstance().getDisplays().getMainDisplay().userArea;
        int x = r.getWidth();
        int y = r.getHeight();
        setBounds(0, 0, frameRect.getWidth(), frameRect.getHeight());
        setCentrePosition(x/2, y/2);
        setResizable(false, false);
        //setName(this->workspace->getTitle());
        
        setVisible (true);
        
        this->starter = new IRStarter(frameRect);
        this->starter->addChangeListener(this);
    
        setContentOwned(this->starter, true);
        
    }
    
    ~IRStartWindow()
    {
        delete starter;
    }
    
    IRStarter::MenuActionStatus getMenuActionStatus() const { return this->starter->getMenuActionStatus(); }
    
    void closeButtonPressed() override
    {
    }
    
private:
    
    // ==================================================
    
    void changeListenerCallback (ChangeBroadcaster* source) override
    {
        if(this->starter == source)
        {
            IRStarter::MenuActionStatus status = this->starter->getMenuActionStatus();
            if(status == IRStarter::MenuActionStatus::CreateNewProjectAction)
            {
                sendChangeMessage();
            }else if(status == IRStarter::MenuActionStatus::OpenProjectAction)
            {
                sendChangeMessage();
            }
        }
    }

    
    IRStarter* starter;
    
    
 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRStartWindow);
    
    
};


#endif /* IRStarter_h */
