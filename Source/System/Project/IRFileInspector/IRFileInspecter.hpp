//
//  IRFileInspecter.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 11/03/2019.
//

#ifndef IRFileInspecter_hpp
#define IRFileInspecter_hpp

#include "JuceHeader.h"
#include "IRFileManager.hpp"
#include "KLib.h"
#include "IRImage.hpp"
#include "IRPropertyComponent.hpp"
#include "IRNodeObject.hpp"
/*
 
    IRFileInspecter shows a list of files being used on the current project.
    It shows what files are loaded on the project and what NodeObjects are using them.
    This lets us manage files easily we are using.
 
 */


class IRFileInspecter : public Component
{
public:
    IRFileInspecter();
    ~IRFileInspecter();
    
    void resized() override;
    void paint(Graphics& g) override;
    
    void makePropertyPanel();
    
    void updatePropertyPanel();
    
private:
        
    PropertyPanel panel;
    
    Array<PropertyComponent*> panelContents;
    
    IRFileManager& FILEMANAGER = singleton<IRFileManager>::get_instance();
    
    // system colour
    IR::IRColours& SYSTEMCOLOUR = singleton<IR::IRColours>::get_instance();

    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IRFileInspecter)

};


#endif /* IRFileInspecter_hpp */
