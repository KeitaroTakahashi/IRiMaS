
#ifndef IRStarter_h
#define IRStarter_h

#include "JuceHeader.h"

#include "Menus.h"
#include "EditCommandTarget.hpp"





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
    
    IRStarter(Rectangle<int> frameRect);
    ~IRStarter();
    
    void paint(Graphics& g) override;

    void init();
    
    Image loadImage(String url);
    
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex(int menuIndex, const String& menuName) override;
    void menuItemSelected (int /*menuItemID*/, int /*topLevelMenuIndex*/) override;
    MenuActionStatus getMenuActionStatus() const;
    
    
private:
    
    void createNewProject();
    void openProject();
    
    void changeListenerCallback(ChangeBroadcaster* source) override;
    
    void setMenuBarPosition(MenuBarPosition newPosition);
    
    ApplicationCommandTarget* getNextCommandTarget() override;
    
    void getAllCommands(Array<CommandID>&c) override;
    void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
    
    bool perform(const InvocationInfo& info) override;
    
    
    Image logo;
    Rectangle<float> logoPos;
    Image title;
    
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    // menubar
    ApplicationCommandManager commandManager;
    std::unique_ptr<MenuBarComponent> menuBar;
    MenuBarPosition menuBarPosition = MenuBarPosition::window;
    // EditCommandTarget* editCommandTarget;
    std::unique_ptr<EditCommandTarget> editCommandTarget;
    SidePanel sidePanel { "Menu", 300, false };
    
    MenuActionStatus menu_action_status;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRStarter)
};




#endif /* IRStarter_h */
