//
//  VideoAnotaterWorkspace.hpp
//  IRiMaS
//
//  Created by Keitaro on 01/03/2020.
//

#ifndef VideoAnotaterWorkspace_hpp
#define VideoAnotaterWorkspace_hpp

#include "IRWorkspaceComponent.hpp"

class VideoAnnotaterWorkspace : public IRWorkspaceComponent
{
public:
    VideoAnnotaterWorkspace(String title, Rectangle<int> draggableMargin, IRStr* str);
    ~VideoAnnotaterWorkspace();
    
    void onPaint(Graphics& g) override;
    
    
    void setFixObjectSizeRatioWithOriginalSize(bool flag, Rectangle<float> originalSize);
private:
    
    
};

#endif /* VideoAnotaterWorkspace_hpp */
