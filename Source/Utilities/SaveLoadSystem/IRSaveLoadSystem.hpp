//
//  IRSaveLoadSystem.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 06/11/2018.
//

#ifndef IRSaveLoadSystem_hpp
#define IRSaveLoadSystem_hpp

#include <stdio.h>
#include <fstream>
#include "JuceHeader.h"
#include "KLib.h"
#include "json11.hpp"

typedef json11::Json t_json;

class IRSaveLoadSystem: public Component
{
public:
    
    struct headerStr
    {
        std::string author;
        Rectangle<int> bounds;
        std::string date;
        std::string osType;
        std::string osVersion;
        std::string IRVersion;

        std::string projectName;
    };

    struct dataStr
    {
        headerStr header;
    };
    
    
    IRSaveLoadSystem()
    {
        
    }
    
    ~IRSaveLoadSystem()
    {
        
    }
    // ========================================
    
    // create header of the save data including project information
    void createHeader();
    // load header
    void loadHeader();
    
    // create workspaces save data
    void createWorkspaces(json11::Json workspaceSaveData);
    // load workspaces
    void loadWorkspaces();
    
    // create all necessary project directories
    bool createProjectDirectory(std::string projectPath);
    bool createDirectoryWithPath(std::string path, std::string text);
    // ========================================
    // export save data into a file
    void writeSaveData(std::string filePath)
    {
        std::ofstream myFile;
        // trunc mode : discard previous data and write new save data
        myFile.open(filePath, std::ofstream::trunc);
        myFile << this->saveData.dump();
        myFile.close();
    }
    
    // read save data from a file
    void readSaveData(std::string filePath)
    {
        
        //init
        this->readData.clear();
        
        std::ifstream myFile(filePath);
        
        if(!myFile){
            std::cout << "Error : Could not open file " << filePath << std::endl;
            return;
        }
        
        std::string buf;
        while (!myFile.eof())
        {
            std::getline(myFile, buf);
            this->readData += buf;
        }
        
        myFile.close();
        
        std::string err;
        this->saveData = json11::Json::parse(this->readData,err);

        std::cout << "Header" << "\n";
        
        loadHeader();

        std::cout << "Worksapce" << "\n";
      
        loadWorkspaces();
     
        for(auto it = this->saveData["Workspaces"].object_items().cbegin(); it != this->saveData["Workspaces"].object_items().cend(); ++it)
        {
            std::cout << " - " << it->first << "\n";
            
            for (auto item2 : this->saveData["Workspaces"][it->first].object_items())
            {
                std::cout << " - - " << item2.first << std::endl;
                
            }
            
        }
        
        for (auto item : this->saveData["Workspaces"].object_items())
        {
            std::cout << item.first << std::endl;

        }
        
        std::cout << "workspace size = " << this->saveData["Workspaces"].object_items().size() << std::endl;
        
        
        
    }
    // ========================================
    json11::Json getSaveData() const
    {
        return this->saveData;
    }
    
    dataStr getSaveDataStr() const
    {
        return this->saveDataStr;
    }
    
    void setHeader(json11::Json newHeader)
    {
        this->header = newHeader;
    }
    // ========================================
    // Listener
    class Listener
    {
    public:
        virtual ~Listener() {}
        
        virtual void createNewProjectFromSaveData();
        virtual void createNewWorkspaceFromSaveData();
    };
    
    virtual void addListener(Listener* newListener) { this->listeners.add(newListener); }
    virtual void removeListener(Listener* listener) { this->listeners.remove(listener);}
    
    ListenerList<Listener> listeners;
    
    std::function<void()> createNewProjectFromSaveDataComplete;
    std::function<void()> createNewWorkspaceFromSaveDataComplete;

    // fire createNewProjectFromSaveData() method in the listener
    void callCreateNewProjectFromSaveData();
    // fire
    void callCreateNewWorkspaceFromSaveData();
    
    
private:
    
    std::string fileName;
    
    json11::Json header;
    
    json11::Json saveData;
    std::string readData;
    
    dataStr saveDataStr;
    
    // --------------------------------------------------
    
    std::string projectDirectory;
    std::string imageDirectory = "/images";
    std::string movieDirectory = "/movies";
    std::string documentDirectory = "/documents";
    std::string audioDirectory = "/audio";
    std::string resourceDirectory = "/Resources";

};


#endif /* IRSaveLoadSystem_hpp */




