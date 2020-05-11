//
//  IRVideoAnnotaterObject2.hpp
//  IRiMaS
//
//  Created by Keitaro on 10/05/2020.
//

#ifndef IRVideoAnnotaterObject2_hpp
#define IRVideoAnnotaterObject2_hpp

#include "IRVideoAnnotaterWorkspace.hpp"
#include "IRVATextEditorObject.h"
#include "IRObjectCreater.hpp"

class IRVideoAnnotaterObject2 : public IRNodeObject
{
public:
    IRVideoAnnotaterObject2(Component* parent, IRStr* str, bool withOpenButton = true);

    ~IRVideoAnnotaterObject2();
    
    void resized() override;
    
    void paint(Graphics& g) override;
    
private:
    
    
    void createNodeObjectOnWorkspace(IRNodeObject* obj);
    void createTextObject();
    
    
    std::shared_ptr<IRVideoAnnotaterWorkspace> workspace;
    
};
#endif /* IRVideoAnnotaterObject2_hpp */
