//
//  IRFileManager.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 05/02/2019.
//

#ifndef IRFileManager_hpp
#define IRFileManager_hpp

#include <map>

#include "JuceHeader.h"
#include "DataType.h"

/*
 
    IRFileManager should be constructed as singleton.
    This class manages all files imported on the software and avoid import the same file more than twice unless the files are modified by the program.
 
 
 juce::Image
 audioDataClass
 IRVideoPlayer or juce::VideoComponent
 TextDataClass : .txt, .csv, .cpp, .c
 
 
 
 */

class IRFileManager
{
public:
    IRFileManager()
    {
        
    }
    
    ~IRFileManager()
    {
        
    }
    //==================================================

    // check if the new file is already imported or not
    bool isFileAlreadyHeld(FILE* newFile);
    bool isObjectAlreadyExist(IRObjectPtr p);
    // -------------------------------------------------
    IRObjectPtr retrieveAlreadyHeldFile(FILE* file);
    FILE* retrieveAlreadyHeldFILE(IRObjectPtr p);
    // -------------------------------------------------
    void registerNewFile(FILE file, IRObjectPtr obj);
    // -------------------------------------------------

    //==================================================
    void clear() { this->list.clear(); }
    //==================================================
    
    
private:
    
    class FILEMAP
    {
    private:
        Array<FILE*> fileList;
        Array<IRObjectPtr> pList;
    public:
        void clear()
        {
            this->fileList.clear();
            this->pList.clear();
        }
        
        void add(FILE* f, IRObjectPtr p)
        {
            this->fileList.add(f);
            this->pList.add(p);
        }
        
        FILE* find(FILE* f)
        {
            int index = this->fileList.indexOf(f);
            if (index > -1) return this->fileList[index];
            else return nullptr;
        }
        
        IRObjectPtr find(IRObjectPtr p)
        {
            int index = this->pList.indexOf(p);
            if(index > -1) return this->pList[index];
            else return nullptr;
        }
  
    };
    
    FILEMAP list;
};


#endif /* IRFileManager_hpp */
