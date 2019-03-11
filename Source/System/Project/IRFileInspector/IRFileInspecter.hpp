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
/*
 
    IRFileInspecter shows a list of files being used on the current project.
    It shows what files are loaded on the project and what NodeObjects are using them.
    This lets us manage files easily we are using.
 
 */


class IRFileInspecter : Component
{
public:
    IRFileInspecter();
    ~IRFileInspecter();
    
private:
    IRFileManager& FILEMANAGER = singleton<IRFileManager>::get_instance();

};


#endif /* IRFileInspecter_hpp */
