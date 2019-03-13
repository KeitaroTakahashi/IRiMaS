
#ifndef MenuEnumeratedTypes_hpp
#define MenuEnumeratedTypes_hpp


// this used to be in Menus.h
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
    
    // Window
    fileInspecterWindow,
    menuPreferenceWindow
    
};


// this used to be in MenusListBox.h
enum MenuObjectStatus
{
    MenuOpened,
    MenuClosed,
    MenuComplete
};


// stuff added from class declarations by FD



#endif /* MenuEnumeratedTypes_hpp */
