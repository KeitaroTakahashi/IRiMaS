//
//  IRFileInspecter.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 11/03/2019.
//

#include "IRFileInspecter.hpp"

IRFileInspecter::IRFileInspecter():
panel("panel test")
{
    setSize(480, 640);
    
    makePropertyPanel();
    
    addAndMakeVisible(panel);
}

IRFileInspecter::~IRFileInspecter()
{
    
}


void IRFileInspecter::resized()
{    
    this->panel.setBounds(10, 120, 200, 300);

}

void IRFileInspecter::makePropertyPanel()
{
    this->panelContents.clear();
    this->panel.clear();
    
    FILEMAP* list = FILEMANAGER.getObjctList();
    
    std::vector<File> fileList = list->getFileList();
    std::vector<IRObjectPtr> pList = list->getPList();
    
    
    Array<PropertyComponent* > components;
    for(int i = 0; i < fileList.size(); i++)
    {
       
        DataAllocationManager<IRImage>* obj = static_cast<DataAllocationManager<IRImage>*>(pList[i]);
        
        for(int j = 0; j < obj->getOwnerCount(); j++)
        {
            IRNodeObject* node = static_cast<IRNodeObject* >(obj->getOwnerList()[j]);
            
            components.add(new IRPropertyComponent(node->name, Point<int>(40,0)));
        }

        this->panel.addSection(fileList[i].getFileName(), components);

    }
    

    
  
}

void IRFileInspecter::updatePropertyPanel()
{
    makePropertyPanel();
}
