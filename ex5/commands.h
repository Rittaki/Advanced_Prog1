/*
 * commands.h
 *
 * Author: 337904619 Margarita Kilinkarov 201283033 Gitit Shapira
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
	// you may add additional methods here
	void createCSV(string name) {
		string end = "done", line = read();
		ofstream myFile(name);
		while(line != end){
			myFile << line << "\n";
			line = read();
		}
		myFile.close();
	}
};

// you may add here helper classes
struct Helper
{
	float threshold;
	vector<AnomalyReport> report;
	Helper(){
		threshold = 0.9;
	}
};

struct Report
{
	int start;
	int end;
	string description;
};

// you may edit this class
class Command{
protected:
	DefaultIO* dio;
public:
	Command(DefaultIO* dio):dio(dio){}
	virtual void execute(Helper* info)=0;
	virtual ~Command(){}
};

// implement here your command classes
class Upload:public Command{
public:
	Upload(DefaultIO* dio): Command(dio){}
	void execute(Helper* info) {
		dio->write("Please upload your local train CSV file.\n");
		dio->createCSV("train.csv");
		dio->write("Upload complete.\n");
		dio->write("Please upload your local test CSV file.\n");
		dio->createCSV("test.csv");
		dio->write("Upload complete.\n");
	}
};

class Settings:public Command{
public:
	Settings(DefaultIO* dio): Command(dio){}
	void execute(Helper* info) {
		dio->write("The current correlation threshold is ");
		dio->write(info->threshold);
		dio->write("\nType a new threshold\n");
		float new_treshold;
		dio->read(&new_treshold);
		if (new_treshold <= 1 && new_treshold >= 0)
		{
			info->threshold = new_treshold;
			return;
		}
		dio->write("please choose a value between 0 and 1.\n");
		execute(info);
	}
};

class Detect:public Command{
public:
	Detect(DefaultIO* dio): Command(dio){}
	void execute(Helper* info) {
		TimeSeries train("train.csv");
		TimeSeries test("test.csv");
		HybridAnomalyDetector detector;
		detector.setThreshold(info->threshold);
		detector.learnNormal(train);
		info->report = detector.detect(test);
		dio->write("anomaly detection complete.\n");
	}
};

class Display:public Command{
public:
	Display(DefaultIO* dio): Command(dio){}
	void execute(Helper* info) {
		for (auto itr = info->report.begin(); itr < info->report.end(); itr++)
		{
			dio->write(itr->timeStep);
			dio->write("\t");
			dio->write(itr->description);
			dio->write("\n");
		}		
		dio->write("Done.\n");
	}
};

class Results:public Command{
public:
	Results(DefaultIO* dio): Command(dio){}
	void execute(Helper* info) {
		map<int, int> anomalyMap;
        vector<pair<long, long>> unifedReports;
        string start, end;
        string input;
        TimeSeries test = TimeSeries("test.csv");
        float n = test.numOfFlights();
        float truePositive = 0, falsePositive = 0;
		dio->write("Please upload your local anomalies file.\n");
		vector<Report> user_reports;
		
		string line = dio->read();
		string delim = ",";
		while (line != "done")
		{
			int start = stoi(line.substr(0, line.find(delim)));
			int end = stoi(line.substr(line.find(delim) + 1, line.size()));
			Report report = {start, end, ""};
			anomalyMap.insert({start, end});
			user_reports.push_back(report);
			line = dio->read();
		}
		
		dio->write("Upload complete.\n");
		float positive = anomalyMap.size();
        for (pair<int, int> anomaly : anomalyMap) {
            n -= (float) (anomaly.second - anomaly.first + 1);
        }
        int anomalyReportMaxIndex = info->report.size() - 1;
        for (int i = 0; i < anomalyReportMaxIndex; i++) {
            long firstTimeStep = info->report.at(i).timeStep;
            long lastTimeStep = info->report.at(i).timeStep;
            while (i < anomalyReportMaxIndex &&
                   info->report.at(i).timeStep + 1 == info->report.at(i + 1).timeStep &&
                   info->report.at(i).description == info->report.at(i + 1).description) {
                lastTimeStep = info->report.at(++i).timeStep;
            }
            unifedReports.emplace_back(firstTimeStep, lastTimeStep);
        }
        for (pair<int, int> anomaly : anomalyMap) {
            for (pair<long, long> report: unifedReports) {
                if (report.second >= anomaly.first && anomaly.second >= report.first) {
                    truePositive++;
                    break;
                }
            }
        }
        for (pair<long, long> report: unifedReports) {
            bool tpDetected = false;
            for (pair<int, int> anomaly : anomalyMap) {
                if (report.second >= anomaly.first && anomaly.second >= report.first) {
                    tpDetected = true;
                }
            }
            if (!tpDetected) {
                falsePositive++;
            }
        }
        if (positive == 0) {
            dio->write("True Positive Rate: 0\n");
        } else {
            float positiveRate = truePositive / positive;
            positiveRate = floorf(1000 * positiveRate) / 1000;
            dio->write("True Positive Rate: ");
            dio->write(positiveRate);
            dio->write("\n");
        }
        float falseAlaramRate = falsePositive / n;
        falseAlaramRate = floorf(1000 * falseAlaramRate) / 1000;
        dio->write("False Positive Rate: ");
        dio->write(falseAlaramRate);
        dio->write("\n");
	}
};
#endif /* COMMANDS_H_ */
