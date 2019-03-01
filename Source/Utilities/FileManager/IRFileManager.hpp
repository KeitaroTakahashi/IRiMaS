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
#include "DataAllocationManager.hpp"
#include "DataType.h"
#include "IRImage.hpp"
#include "IRVideo.hpp"
#include "KLib.h"

/*
 
    IRFileManager should be constructed as singleton.
    This class manages all files imported on the software and avoid import the same file more than twice unless the files are modified by the program.
 
 juce::Image
 audioDataClass
 juce::VideoComponent
 TextDataClass : .txt, .csv


 */

class IRFileManager
{
public:
    
    template<class T>
    struct FileManagerStr
    {
        DataAllocationManager<T> dataManager;
        File file;
        
        FileManagerStr(File file,
                       DataAllocationManager<T> dataManager)
        {
            this->file = file;
            this->dataManager = dataManager;
        }
        
    };

    IRFileManager()
    {
        clear();
    }
    
    ~IRFileManager()
    {
        
    }

    //==================================================
    // get a file pointer from IRFileManager
    // if file has not yet been loaded, then this method loads the file first and returns its pointer
    IRObjectPtr getFilePtr(IRFileType type, File file);
    //
    IRObjectPtr discardFilePtr(IRObjectPtr owner, File file);
    
    
    //==================================================

private:
    
    //==================================================
    // allocate data by a given FileType
    IRObjectPtr createFileData(IRFileType type, File file);
    IRObjectPtr createImageFileData(File file);
    
    
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
    
    class FILEMAP
    {
    private:
        Array<File*> fileList;
        Array<IRObjectPtr> pList;
        
        Array<IRObjectPtr> list;
    public:
        void clear()
        {
            this->fileList.clear();
            this->pList.clear();
            this->list.clear();
        }
        
        template<class T>
        bool add(File& f, T& p)
        {
            this->list.add(FileManagerStr<T>(f, DataAllocationManager<T>()));
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
