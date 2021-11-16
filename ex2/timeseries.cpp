/*
 * animaly_detection_util.cpp
 *
 * Author: 337904619 Margarita Kilinkarov 201283033 Gitit Shapira
 */

#include <iostream>
#include "timeseries.h"
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
/**
 * @brief returns the vector of feature's names
 * 
 * @return vector<string>
 */
vector<string> TimeSeries::getFeatures() const{  
    return this->features;
}
/**
 * @brief reads the data from csv file and create the map so
 * that keys are the features and the values are vector of
 * float numbers
 * @return map<string, vector<float>> 
 */
map<string, vector<float>> TimeSeries::CSVData() {
    // get first line of features
    string line, csvItem;
    ifstream myFile(this->filename);
    int lineNumber = 0;
    int firstLine = 1;
    if (myFile.is_open()) {
        while (getline(myFile, line))
        {
            lineNumber++;
            if (lineNumber == firstLine)
            {
                istringstream myline(line);
                int i = 0;
                while (getline(myline, csvItem, ','))
                {
                    this->features.push_back(csvItem);
                }
                
            }
            
        }
        
    }
    // other lines
    map<string, vector<float>> fin;
    string lines;
    float item;
    int i = 0;
    int sizeOfRow = this->features.size();
    for (int j = 0; j < sizeOfRow; j++)
    {
        fin[this->features.at(j)] = vector<float>{};
    }
    ifstream myWholeFile(this->filename);
    while (getline(myWholeFile, lines)) {
        i = 0;
        istringstream myLine(lines);
        while (myLine >> item)
        {
            fin[this->features[i]].push_back(item);
             if (myLine.peek() == ',') {
                 myLine.ignore();
             }
            i++;
        }
    }
    return fin;
}

/**
 * @brief returns the map that was created in CSVData()
 * @return map<string, vector<float>>
 */
map<string, vector<float>> TimeSeries::getMap() const{
    return this->data;
}