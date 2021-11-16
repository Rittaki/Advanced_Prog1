/*
 * timeseries.h
 *
 * Author: 337904619 Margarita Kilinkarov 201283033 Gitit Shapira
 */

#include <map>
#include <vector>

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

using namespace std;

class TimeSeries{
	const char* filename;
	vector<string> features;
	map<string, vector<float>> data;

public:
	TimeSeries(const char* CSVfileName){
		this->filename = CSVfileName;
		this->data = CSVData();
	}
	vector<string> getFeatures() const;
	map<string, vector<float>> CSVData();
	map<string, vector<float>> getMap() const;
};



#endif /* TIMESERIES_H_ */
