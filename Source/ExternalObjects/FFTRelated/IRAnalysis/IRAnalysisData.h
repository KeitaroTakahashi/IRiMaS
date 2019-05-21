//
//  IRAnalysisData.h
//  NodeComponentObject_Study
//
//  Created by Keitaro on 21/05/2019.
//

#ifndef IRAnalysisData_h
#define IRAnalysisData_h

struct IRAnalysisDataStr
{
  
    int fftsize;
    int nframe;
    
    //float* data;
    std::vector<float> data;
};

#endif /* IRAnalysisData_h */
