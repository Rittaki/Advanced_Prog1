/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: 337904619 Margarita Kilinkarov 201283033 Gitit Shapira
 */
#include <iostream>
#include <iterator>
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
	// TODO Auto-generated constructor stub
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}
/**
 * @brief gets objects of timeseries, check evwry pair of features
 * and find the correlative ones
 * @param ts TimeSeries file
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	map<string, vector<float>>::iterator itr1, itr2;
	auto map = ts.getMap();
	float pearsResult, maxThreshold = 0.9, maxPearson, maxDev;
	auto featuresNames = ts.getFeatures();
	int size = featuresNames.size(), vecSize = map.begin()->second.size();
	string feature1, feature2, maxCorFeature;
	Line line;

	for (itr1 = map.begin(); itr1 != map.end(); itr1++)
	{
		pearsResult = 0;
		maxPearson = 0;
		for (itr2 = next(itr1, 1); itr2 != map.end(); itr2++)
		{
			pearsResult = fabs(pearson(itr1->second.data(), itr2->second.data(), 
										itr1->second.size()));
			if (pearsResult > maxPearson)
			{
				maxPearson = pearsResult;
				maxCorFeature = itr2->first;
				line = linear_reg(itr1->second.data(), itr2->second.data(), itr1->second.size());
				maxDev = max_dev(itr1->second.data(), itr2->second.data(), itr1->second.size(), line);
			}
		}
		
		if (maxPearson > maxThreshold)
		{
			correlatedFeatures c;
			c.feature1 = itr1->first;
			c.feature2 = maxCorFeature;
			c.corrlation = maxPearson;
			c.lin_reg = line;
			c.threshold = maxDev * 1.1;
			cf.push_back(c);
		}
	}
}
/**
 * @brief detects anomalies and adds an anomaly report
 * @param ts Timeseries file
 * @return vector<AnomalyReport>
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	vector<AnomalyReport> ar;
	auto map = ts.getMap();
	int size;
	string description, feat1, feat2;
	Line line;
	float maxDev;
	for (auto itr = cf.begin(); itr != cf.end(); itr++){
		feat1 = itr->feature1;
		feat2 = itr->feature2;
		line = itr->lin_reg;
		maxDev = itr->threshold;
		size = map.begin()->second.size();
		for (int i = 0; i < size; i++)
		{
			if (dev(Point(map.find(feat1)->second[i], map.find(feat2)->second[i]), line) > maxDev)
			{
				description = feat1 + "-" + feat2;
				ar.push_back(AnomalyReport(description, i + 1));
			}
			
		}
		
	}
	return ar;
}
