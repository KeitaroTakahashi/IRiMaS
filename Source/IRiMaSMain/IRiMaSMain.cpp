//
//  IRiMaSMain.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 15/01/2019.
//

#include "IRiMaSMain.hpp"


void IRMAIN::createNewProjectFromSaveData(std::string path)
{
    printf("========== createNewProjectFromSaveData ==========\n");
    IRSaveLoadSystem::dataStr data = this->saveLoadClass.getSaveDataStr();
    IRSaveLoadSystem::headerStr header = data.header;
    
    t_json saveData = this->saveLoadClass.getSaveData();
    
    IRProjectWindow* project = new IRProjectWindow(applicationName,this->preferenceWindow);
    
    
    Rectangle<int>bounds = header.bounds;
    
    project->setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight());
    
    project->getProjectComponent()->addListener(this);
    project->getProjectComponent()->setProjectPath(path);
    project->setWindowTitle(header.projectName);
    
    this->projectLib.push_back(project);
    
    // bring the new window in front of all other windows.
    project->toFront(true);
    
    //remove startWindow when a project window opens.
    this->startWindow->setVisible(false);
    
    printf("========== loadWorkspaces ==========\n");
    
    for(auto it = saveData["Workspaces"].object_items().cbegin(); it != saveData["Workspaces"].object_items().cend(); ++it)
    {
        std::string id = static_cast<std::string>(it->first);
        std::cout << id << std::endl;
        
        // ===== create worksapce =====
        project->getProjectComponent()->createNewWorkspace();
        
        // get created workspace
        IRWorkSpace* currentSpace = project->getProjectComponent()->getTopWorkspace();
        
        // retrieve save data of the workspace
        json11::Json data = saveData["Workspaces"][id].object_items();
        
        // retrieve save data of worksapce appearance
        json11::Json appearance = data["Appearance"];
        std::cout << "Appearance : backgroundColour = " << appearance["backgroundColour"].string_value() << std::endl;
        
        // retrieve save data of objects on the workspace
        json11::Json Objects = data["Objects"];
        // the object data is stored in array
        json11::Json::array objectArray = Objects.array_items();
        
        std::cout << "array count = " << objectArray.size() << std::endl;
        
        
        for(int i =0; i< objectArray.size(); i++) // for each item of the array...
        {
            for(auto it = objectArray[i].object_items().cbegin(); it != objectArray[i].object_items().cend(); ++it)
            {
                
                std::cout << " ===== "<< it->first << " ===== "<< std::endl;
                std::cout << "object type= "<< it->second["objectType"].string_value() << std::endl;
                std::cout << "object uniqueID= "<< it->second["objectUniqueID"].string_value() << std::endl;
                std::cout << "object status= "<< it->second["status"].string_value() << std::endl;
                
                
                // ===== create object =====
                std::string objectTypeId = it->second["objectType"].string_value();
                auto* obj = IRFactory.createObject(objectTypeId, currentSpace);
                
                json11::Json::array b = it->second["bounds"].array_items();
                obj->setBounds(b[0].int_value(), b[1].int_value(),
                               b[2].int_value(), b[3].int_value());
                
                std::cout << "object bounds = " << obj->getWidth() << ", " << obj->getHeight() << std::endl;
                
                obj->setUniqueID(it->second["objectUniqueID"].string_value());
                //
                
                // currently no status implemented!
                
                currentSpace->createObject(obj);
                
                // load save dada
                obj->loadThisFromSaveData(it->second["ObjectDefined"]);
                
                // ===== END =====
                /*
                 for(auto it2 = it->second["ObjectDefined"].object_items().cbegin(); it2 != it->second.object_items().cend(); ++it2)
                 {
                 std::cout << it2->first << std::endl;
                 
                 }*/
                
                //std::cout << it->first["ObjectType"].string_value() << std::endl;
                std::cout << objectArray[i][it->first]["ObjectType"].string_value() << std::endl;
                
                //currentSpace->createObject()
                
            }
            
        }
        
        //as default, a workspace is in control mode
        //currentSpace->setEditMode(false);
        
    }
    
    // initialize this Project
    project->getProjectComponent()->initProjectAfterLoading();
    // set project save path
    project->getProjectComponent()->setProjectPath(path);
    
}

