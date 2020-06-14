//
//  IRVideoAnnotaterSRTFile.cpp
//  IRiMaS
//
//  Created by Keitaro on 10/06/2020.
//

#include "IRVideoAnnotaterSRTFile.hpp"


IRVideoAnnotaterSRTFile::IRVideoAnnotaterSRTFile()
{
    
}

IRVideoAnnotaterSRTFile::~IRVideoAnnotaterSRTFile()
{
    
}



void IRVideoAnnotaterSRTFile::openFile()
{
    FileChooser chooser("Select a SRT file to load...",
                        {},
                        "*.srt", "*.srts", "*.irsrt");
    
    
    if(chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();
        this->path = file.getFullPathName();
        
        loadFile(this->path);
        
    }
}

void IRVideoAnnotaterSRTFile::openFile(String path)
{
    File f(path);
    
    if(f.exists()) loadFile(path);
    else std::cout << "Error : IRVideoAnnotaterSRTFile : file not exist! " << path << std::endl;
}

void IRVideoAnnotaterSRTFile::loadFile(String path)
{
    this->srtL.openFile(path.toStdString());
    auto list = this->srtL.getSubtitleItems();
    
    this->srtData = list;
    //interpretLoadedData(list);
}

void IRVideoAnnotaterSRTFile::interpretLoadedData(std::vector<SubtitleItem*> list)
{
    for(auto item : list)
    {
        std::string startTime = item->getStartTimeString();
        std::string endTime = item->getEndTimeString();
        
        std::string text = item->getText();
        std::cout << "text  = " << item->getText() << std::endl;
        
        std::string err;
        json11::Json saveData = json11::Json::parse(text, err);
        
        std::cout << "get data as json : " << saveData["text"].string_value() << std::endl;
        
        std::string data = saveData["text"].string_value();
        if(data.size() > 0)
        {
            
        }
    }
}
