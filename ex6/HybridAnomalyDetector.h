/*
 * HybridAnomalyDetector.cpp
 *
 * Author: 337904619 Margarita Kilinkarov 201283033 Gitit Shapira
 */

#ifndef EX4_HYBRIDANOMALYDETECTOR_H
#define EX4_HYBRIDANOMALYDETECTOR_H


#include "SimpleAnomalyDetector.h"
#include "minCircle.h"
#include <vector>
class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
    HybridAnomalyDetector();
    virtual ~HybridAnomalyDetector();
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);

};

#endif //EX4_HYBRIDANOMALYDETECTOR_H
