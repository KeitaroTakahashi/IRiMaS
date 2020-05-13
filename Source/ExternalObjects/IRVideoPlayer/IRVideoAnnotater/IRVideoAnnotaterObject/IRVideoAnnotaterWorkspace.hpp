//
//  IRVideoAnnotaterWorkspace.hpp
//  IRiMaS
//
//  Created by Keitaro on 09/05/2020.
//

#ifndef IRVideoAnnotaterWorkspace_hpp
#define IRVideoAnnotaterWorkspace_hpp

#include "IRNodeObjectWorkspace.hpp"
#include "IRVideoPlayerObject2.hpp"

class IRVideoAnnotaterWorkspace : public IRNodeObjectWorkspace
{
public:
    IRVideoAnnotaterWorkspace(String title, Rectangle<int> draggableMargin, IRStr* str);
    ~IRVideoAnnotaterWorkspace();
    
    void onResized() override;
    void onPaint(Graphics& g) override;
    
    // ==============================

    void addAnnotationObject(IRNodeObject* obj, Rectangle<int> bounds);
    
    // ==============================

    // ==============================
    IRVideoPlayerObject2* getVideoPlayerObject();
    
private:
    
    
    std::shared_ptr<IRVideoPlayerObject2> videoPlayerObj;

    
};
#endif /* IRVideoAnnotaterWorkspace_hpp */
