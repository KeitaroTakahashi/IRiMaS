//
//  IRGraphSelector.hpp
//  NodeComponentObject_Study - App
//
//  Created by Keitaro on 27/08/2018.
//

#ifndef IRGraphSelector_hpp
#define IRGraphSelector_hpp

#include "IRObjectSelection.hpp"
#include "IRMultiPurposeObject.hpp"


class IRGraphSelector : public IRObjectSelection<Component *>
{
    // ============================================================

public:
    IRGraphSelector()
    {
        
    }
    // ------------------------------------------------------------
    ~IRGraphSelector() {}
    // ------------------------------------------------------------
    void mouseDownHandler(const MouseEvent& e) override
    {
        beginSelection(e);
        this->multiSelectionFlag = true;
    }
    // ------------------------------------------------------------
    void mouseDragHandler(const MouseEvent& e) override
    {
        if(this->multiSelectionFlag)
        {
            dragSelection(e);
        }
    }
    // ------------------------------------------------------------
    void mouseUpHandler(const MouseEvent& e) override
    {
        if(this->multiSelectionFlag){
            endSelection(e);
            this->multiSelectionFlag = false;
        }
    }
    
    // ------------------------------------------------------------
    void setMakeSquareObjectAfterSelection(bool flag) { this->makeSquareObjectAfterSelection = flag; }
    bool isMakeSquareObjectAfterSelection() const { return this->makeSquareObjectAfterSelection; }
    // ------------------------------------------------------------

private:
    bool multiSelectionFlag = false;
    
    // make Component object at exactly the same place selection square is made.
    bool makeSquareObjectAfterSelection = false;

};

#endif /* IRGraphSelector_hpp */
