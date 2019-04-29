//
//  KTextLoader.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 28/04/2019.
//

#ifndef KTextLoader_h
#define KTextLoader_h

#include <fstream>
#include <string>
#include "../JuceLibraryCode/JuceHeader.h"

class IRTextLoader
{
public:
    IRTextLoader(std::string url)
    {
        std::ifstream myFile (url);
        std::string line;
        if(myFile.is_open())
        {
            while (getline(myFile, line))
            {
                this->txtData += line;
            }
        }
        
        myFile.close();
    }
    ~IRTextLoader() {}
    
    const char* getConstChar()
    {
        return this->txtData.c_str();
    }
    
    std::string getStdString() const
    {
        return this->txtData;
    }
    
private:
    
    std::string txtData;
};

#endif /* KTextLoader_h */
