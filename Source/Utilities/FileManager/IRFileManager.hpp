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
#include "KLib.h"

/*
 
    IRFileManager should be constructed as singleton.
    This class manages all files imported on the software and avoid import the same file more than twice unless the files are modified by the program.
 
 juce::Image
 audioDataClass
 juce::VideoComponent
 TextDataClass : .txt, .csv, .cpp, .c


 */

class IRFileManager
{
public:
    IRFileManager()
    {
        clear();
    }
    
    ~IRFileManager()
    {
        
    }
    //==================================================

    // check if the new file is already imported or not
    bool isFileAlreadyRegistered(File* newFile);
    bool isObjectAlreadyRegistered(IRObjectPtr p);
    // -------------------------------------------------
    // return nullptr if object does not exist
    IRObjectPtr retrievePtrByFile(File* file);
    File* retrieveFileByPtr(IRObjectPtr p);
    // -------------------------------------------------
    void registerNewFile(File* file, IRObjectPtr obj);
    // -------------------------------------------------

    //==================================================
    void clear() { this->list.clear(); }
    //==================================================
    
    
private:
    
    class FILEMAP
    {
    private:
        Array<File*> fileList;
        Array<IRObjectPtr> pList;
    public:
        void clear()
        {
            this->fileList.clear();
            this->pList.clear();
        }
        
        bool add(File* f, IRObjectPtr p)
        {
            this->fileList.add(f);
            this->pList.add(p);
            return true;
            
            // if any confliction occurs
            if(this->fileList.size() != this->pList.size()) return false;
        }
        
        bool remove(File* f)
        {
            int index = this->fileList.indexOf(f);
            this->fileList.remove(index);
            this->pList.remove(index);
            return true;
            
            // if any confliction occurs
            if(this->fileList.size() != this->pList.size()) return false;
        }
        
        bool remove(IRObjectPtr p)
        {
            int index = this->pList.indexOf(p);
            this->fileList.remove(index);
            this->pList.remove(index);
            return true;
            
            // if any confliction occurs
            if(this->fileList.size() != this->pList.size()) return false;
        }
        
        File* findFile(File* f)
        {
            int index = this->fileList.indexOf(f);
            if (index > -1) return this->fileList[index];
            else return nullptr;
        }
        
        IRObjectPtr findPtr(IRObjectPtr p)
        {
            int index = this->pList.indexOf(p);
            if (index > -1) return this->pList[index];
            else return nullptr;
        }
        
        IRObjectPtr findPtrByFile(File* f)
        {
            int index = this->pList.indexOf(f);
            if (index > -1) return this->pList[index];
            else return nullptr;
        }
        
        File* findFileByPtr(IRObjectPtr p)
        {
            int index = this->pList.indexOf(p);
            if(index > -1) return this->fileList[index];
            else return nullptr;
        }
  
    };
    
    FILEMAP list;
};


#endif /* IRFileManager_hpp */
