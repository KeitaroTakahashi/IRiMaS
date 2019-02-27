//
//  IRWorkspaceList.cpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 12/11/2018.
//

#include "IRWorkspaceList.hpp"



bool IRWorkspaceList::keyPressed (const KeyPress& key,
                 Component* originatingComponent)
{
    std::cout << "IRWorkspaceList keyPressed : " << key.getKeyCode() << std::endl;
    if(key.getKeyCode() == key.deleteKey || key.getKeyCode() == key.backspaceKey)
    {
        
        IRWorkSpace* space = dynamic_cast<IRWorkSpace*>(this->selectedComponent);
        
        if(space != nullptr){
            
            showSnap* removeSnap = this->currentlySelectedSnap;
            int removeIndex = removeSnap->getIndex();
            
            // set new selectedSnap
            if(this->workspaces.size() == 1){
                this->currentlySelectedSnap = nullptr;
            }else{
                if(removeIndex == 1){
                    // set one higher index
                    this->currentlySelectedSnap = this->snapComponents[removeIndex];
                }else{
                    // set one lower index
                    this->currentlySelectedSnap = this->snapComponents[removeIndex-2];
                }
            }
            
            removeWorkspace(removeSnap);
            
            
            
        }
        
        return true;
    }
    
    // select neighbor slides
   else if(key.getKeyCode() == key.upKey)
   {
       showSnap* snap = this->currentlySelectedSnap;
       int snapIndex = snap->getIndex() - 1;
       // if selected snap is not the first one, then move to the forward
       if(snapIndex > 0){
           snapSelectionChange(this->snapComponents[snapIndex-1]);
       }
       return true;
    }else if(key.getKeyCode() == key.downKey){

        showSnap* snap = this->currentlySelectedSnap;
        int snapIndex = snap->getIndex();

        // if selected snap is not the last one, then move to the backward
        if(snapIndex < this->snapComponents.size()){
            snapSelectionChange(this->snapComponents[snapIndex]);
        }
        return true;
    }
    
   
    
    if(key.getTextDescription() == "command + E")
    {
        
        
    }
    return false;
}
