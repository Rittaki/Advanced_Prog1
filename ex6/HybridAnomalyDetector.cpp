/*
 * HybridAnomalyDetector.cpp
 *
 * Author: 337904619 Margarita Kilinkarov 201283033 Gitit Shapira
 */

#include "HybridAnomalyDetector.h"
#include <vector>

HybridAnomalyDetector::HybridAnomalyDetector() {

}

HybridAnomalyDetector::~HybridAnomalyDetector() {

}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> anomalyReports = SimpleAnomalyDetector :: detect(ts);
    auto map = ts.getMap();
    int feature1Index, feature2Index, size = map.begin()->second.size();
    string feat1, feat2;
    for (int i = 0; i < this->_cf.size(); ++i) {
        if (this->_cf[i].corrlation < _threshold) {
            for (int j = 0; j < ts.getFeatures().size(); ++j) {
                if (ts.getFeatures().at(j) == _cf[i].feature1) {
                    feature1Index = j;
                }
                if (ts.getFeatures().at(j) == _cf[i].feature2) {
                    feature2Index = j;
                }
            }
            for (int j = 0; j < size; ++j) {
                feat1 = ts.getFeatures().at(feature1Index);
                feat2 = ts.getFeatures().at(feature2Index);
                Point p = Point(map.find(feat1)->second[j], map.find(feat2)->second[j]);
                if (!pointInCircle(p,_cf[i].minCirle)){
                    AnomalyReport ar = AnomalyReport(_cf[i].feature1 + "-" + _cf[i].feature2, j + 1);
                    anomalyReports.push_back(ar);
                }
            }
        }

    }
    return anomalyReports;
}