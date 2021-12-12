/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: 337904619 Margarita Kilinkarov 201283033 Gitit Shapira
 */

#include "SimpleAnomalyDetector.h"
#include <iostream>
#include <vector>


SimpleAnomalyDetector::~SimpleAnomalyDetector() {

}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts) {
    map<string, vector<float>>::iterator itr1, itr2;
	auto map = ts.getMap();
    int numOfFeature = ts.numOfElements();
    int numOfFlights = ts.numOfFlights();
    Point **points = nullptr;
    correlatedFeatures cf;
    for (itr1 = map.begin(); itr1 != map.end(); itr1++) {
        cf.feature1 = itr1->first.data();
        cf.corrlation = 0;
        float maxOffset = 0;
        for (itr2 = next(itr1, 1); itr2 != map.end(); itr2++) {
            if (itr1->first.data() != itr2->first.data()) {
                string feature2 = itr2->first.data();
                float newCorrlation = fabs(
                        pearson(itr1->second.data(), itr2->second.data(), numOfFlights));

                if (cf.corrlation < newCorrlation) {
                    freeArray(points, numOfFlights);
                    cf.corrlation = newCorrlation;
                    cf.feature2 = feature2;
                    points = recordsToPoints(itr1->second, itr2->second, numOfFlights);
                    cf.lin_reg = linear_reg(points, numOfFlights);
                }
            }
        }
        for (int j = 0; j < numOfFlights; ++j) {
            float offset = fabs(dev(*points[j], cf.lin_reg));
            if (offset > maxOffset) {
                maxOffset = offset;
            }
        }

        cf.maxOffset = 1.1 * maxOffset;
        if(cf.corrlation > 0.5){
            if(cf.corrlation < _threshold){
                cf.minCirle = findMinCircle(points,numOfFlights);
                cf.minCirle.radius *= 1.1;
            }
            _cf.push_back(cf);
        }
    }
    for (int i = 0; i < _cf.size(); ++i) {
        for (int j = 0; j < _cf.size(); ++j) {
            if (_cf.at(i).feature1 == _cf.at(j).feature2 && _cf.at(i).feature2 == _cf.at(j).feature1) {
                _cf.erase(_cf.begin() + j);
                break;
            }
        }
    }
    freeArray(points, numOfFlights);
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> anomalyReports;
    auto map = ts.getMap();
    string feat1, feat2;
    int feature1Index, feature2Index, size = map.begin()->second.size();
    for (int i = 0; i < this->_cf.size(); ++i) {
        if (this->_cf[i].corrlation >= _threshold) {
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
                if (dev(p, this->_cf[i].lin_reg) > _cf[i].maxOffset) {
                    AnomalyReport ar = AnomalyReport(_cf[i].feature1 + "-" + _cf[i].feature2, j + 1);
                    anomalyReports.push_back(ar);
                }
            }


        }

    }
    return anomalyReports;
}

Point **SimpleAnomalyDetector::recordsToPoints(vector<float> feature1Records, vector<float> feature2Records, int size) {
    Point **array = new Point *[size];
    for (int i = 0; i < size; ++i) {
        array[i] = new Point(feature1Records[i], feature2Records[i]);
    }
    return array;
}

void SimpleAnomalyDetector::freeArray(Point **array, int sizeOfArray) {
    if (array != nullptr) {
        for (int i = 0; i < sizeOfArray; i++) {
            delete[] array[i];
        }

        delete[] array;
        array = nullptr;
    }
}

float SimpleAnomalyDetector::getThreshold(){
    return _threshold;
}

void SimpleAnomalyDetector::setThreshold(float newThreshold){
    this->_threshold = newThreshold;
}