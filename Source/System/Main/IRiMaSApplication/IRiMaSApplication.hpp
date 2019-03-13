
#include "JuceHeader.h"

#include "IRiMaSMain.hpp"





class IRiMaSApplication  : public JUCEApplication
{
    
public:
    
    IRiMaSApplication();
    
    const String getApplicationName() override;
    const String getApplicationVersion() override;
    bool moreThanOneInstanceAllowed() override;
    
    void initialise(const String& commandLine) override;
    void shutdown() override;
    
    void systemRequestedQuit() override;
    
    void anotherInstanceStarted(const String& commandLine) override;
    
    
private:
    
    std::unique_ptr<IRMAIN> mainWindow;
    
};




