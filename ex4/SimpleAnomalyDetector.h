/*
 * SimpleAnomalyDetector.h
 *
 * Author: 337904619 Margarita Kilinkarov 201283033 Gitit Shapira
 */

#ifndef SIMPLEANOMALYDETECTOR_H
#define SIMPLEANOMALYDETECTOR_H

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"

#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    Circle minCirle;
    float maxOffset;
    float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    protected:
    vector<correlatedFeatures> _cf;
    float _threshold;
public:
    SimpleAnomalyDetector():_threshold(0.9){}
    virtual ~SimpleAnomalyDetector();

    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    Point** recordsToPoints(vector<float> feature1Records ,vector<float> feature2Records,int size);
    float* vecToArray(vector<float> vec);
    vector<correlatedFeatures> getNormalModel(){
        return _cf;
    }

    virtual float getThreshold();
    virtual void setThreshold(float newThreshold);
    void freeArray(Point **array, int sizeOfArray);
};




#endif //SIMPLEANOMALYDETECTOR_H
