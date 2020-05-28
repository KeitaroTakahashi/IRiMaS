//
//  IRQuillTextEditor.hpp
//  IRiMaS
//
//  Created by Keitaro on 21/05/2020.
//

#ifndef IRQuillTextEditor_hpp
#define IRQuillTextEditor_hpp

#include "IRUIFoundation.hpp"

class IRQuillTextEditor : public IRUIFoundation
{
public:
    IRQuillTextEditor(IRNodeObject* parent, IRStr* str);
    ~IRQuillTextEditor();
    
    
    void resized() override;
    
private:
    std::shared_ptr<WebBrowserComponent> browser;
    
    
};

#endif /* IRQuillTextEditor_hpp */
