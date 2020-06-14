//
//  IRVideoAnnotaterSRTFile.hpp
//  IRiMaS
//
//  Created by Keitaro on 10/06/2020.
//

#ifndef IRVideoAnnotaterSRTFile_hpp
#define IRVideoAnnotaterSRTFile_hpp

#include "IRStrComponent.hpp"
#include "json11.hpp"
#include "srtWriter.h"
#include "srtLoader.h"


class IRVideoAnnotaterSRTFile :
public ChangeBroadcaster
{
public:
    IRVideoAnnotaterSRTFile();
    ~IRVideoAnnotaterSRTFile();
    
    void openFile();
    void openFile(String path);
    
    void loadFile(String path);

    String getPath() const { return this->path; }

    std::vector<SubtitleItem*> getSrtData() const { return this->srtData; }
private:
    String path;
    srtWriter srt;
    srtLoader srtL;
    
    void interpretLoadedData(std::vector<SubtitleItem*> list);
    
    std::vector<SubtitleItem*> srtData;

};
#endif /* IRVideoAnnotaterSRTFile_hpp */
