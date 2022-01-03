/*
 * CLI.cpp
 *
 * Author: 337904619 Margarita Kilinkarov 201283033 Gitit Shapira
 */
#include "CLI.h"
#include "commands.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    commands.insert({"1", new Upload(dio)});
    commands.insert({"2", new Settings(dio)});
    commands.insert({"3", new Detect(dio)});
    commands.insert({"4", new Display(dio)});
    commands.insert({"5", new Results(dio)});
}

void CLI::start(){
    Helper info;
    string option, menu = "Welcome to the Anomaly Detection Server.\n"
                    "Please choose an option:\n"
                    "1.upload a time series csv file\n"
                    "2.algorithm settings\n"
                    "3.detect anomalies\n"
                    "4.display results\n"
                    "5.upload anomalies and analyze results\n"
                    "6.exit\n";
    while (true)
    {
        dio->write(menu);
        option = dio->read();
        if(option == "6") {
            break;
        }
        auto itr = this->commands.find(option)->second;
        itr->execute(&info);  
    }
    return;
}

CLI::~CLI() {
}
