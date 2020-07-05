//
//  AnnotationChart.h
//  IRiMaS
//
//  Created by Keitaro on 02/07/2020.
//

#ifndef AnnotationChart_h
#define AnnotationChart_h


class AnnotationChart : public Component,
public IRStrComponent
{
public:
    
    struct annotationData
    {
        float beginTime = 0;
        float endTime = 0;
        float duration = 0;
        
        annotationData(float beginTime, float endTime)
        {
            this->beginTime = beginTime;
            this->endTime = endTime;
            this->duration = this->endTime - this->beginTime;
        }
        void setBeginTime(float val)
        {
            this->beginTime = val;
            this->duration = this->endTime - this->beginTime;
        }
        
        void setEndTime(float val)
        {
            this->endTime = val;
            this->duration = this->endTime - this->beginTime;
        }
    };

    struct annotationGraphStr
    {
        int size = 0;
        int maxCount = 0;
        std::vector<int> count;
        
        void clear()
        {
            count.clear();
            size = 0;
        }
        
        void allocateWith(int n, int val)
        {
            clear();
            size = n;
            maxCount = 0;
            std::vector<int> v(n, val);
            count = v;
        }
    };

    AnnotationChart(IRStr* str) : IRStrComponent(str)
    {
        
    }
    
    ~AnnotationChart()
    {
        this->graph.clear();
    }
    // ==============================

    void resized() override
    {
        reCalc();
    }
    
    void paint(Graphics& g) override
    {
        int max = this->graph.maxCount;
        
        g.setColour(Colour(255, 0, 0));

        for(int i = 0; i < this->graph.size; i ++)
        {
            int norm = floor(((float)this->graph.count[i] / (float)max) * getHeight());

            g.fillRect(i, norm, i, norm);
        }
    }
    
    // ==============================
    void init()
    {
        this->graph.clear();
        this->annotations.clear();
    }
    
    // ==============================
    
    void addEvent(float beginTime, float endTime)
    {
        this->annotations.push_back(annotationData(beginTime, endTime));
        
        float w = (float) getWidth();
        float h = (float) getHeight();
        
        // calc seconds per pixel
        float SecPerPix = this->videoLength / w;
        
        int beginIndex = floor(beginTime / SecPerPix);
        int duration = floor((endTime - beginTime) / SecPerPix);
        
        for(int i=beginIndex;i<=beginIndex+duration;i++)
        {
            this->graph.count[i] ++;
            
            if(this->graph.maxCount < this->graph.count[i])
            {
                this->graph.maxCount = this->graph.count[i];
            }
        }
    }
    
    void reCalc()
    {
        this->graph.clear();
        this->graph.allocateWith(getWidth(), 0);
        
        float w = (float) getWidth();
        float h = (float) getHeight();
        
        // calc seconds per pixel
        float SecPerPix = this->videoLength / w;
        
        for(auto a : this->annotations)
        {
            int beginIndex = floor(a.beginTime / SecPerPix);
            int duration = floor((a.endTime - a.beginTime) / SecPerPix);
            
            for(int i=beginIndex;i<=beginIndex+duration;i++)
            {
                this->graph.count[i] ++;
                
                if(this->graph.maxCount < this->graph.count[i])
                {
                    this->graph.maxCount = this->graph.count[i];
                }
            }
        }
        
    }
    
    // ==============================
    
    void setVideoLength(float length)
    {
        this->videoLength = length;
    }

    // ==============================

private:
    
    std::vector<annotationData> annotations;
    float videoLength = 0.0;
    
    annotationGraphStr graph;
};


#endif /* AnnotationChart_h */
