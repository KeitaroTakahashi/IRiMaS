//
//  KeLib.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 23/11/2018.
//

#ifndef KeLib_h
#define KeLib_h


#include "KRandomStringGenerator.h"
#include "KDictionary.h"


class KLib
{
public:
    
    //separator
    std::vector<std::string> StringSplit(const std::string &str, char sep)
    {
        std::vector<std::string> v;
        std::stringstream ss(str);
        std::string buffer;
        while( std::getline(ss, buffer, sep) ) {
            v.push_back(buffer);
        }
        return v;
    }
    
    void showConnectionErrorMessage (const String& messageText)
    {
        AlertWindow::showMessageBoxAsync (AlertWindow::WarningIcon,
                                          "Connection error",
                                          messageText,
                                          "OK");
    }
    
    template<typename T>
    void showRectangle(Rectangle<T> rect){
        std::cout << "x = " << rect.getX() << " : y = " << rect.getY() << " : w = " << rect.getWidth() << " : h = " << rect.getHeight() << std::endl;
    }
};


#endif /* KeLib_h */
