/*
 * CLI.h
 *
 * Author: 337904619 Margarita Kilinkarov 201283033 Gitit Shapira
 */
#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	map<string, Command*> commands;
	// you can add data members
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

#endif /* CLI_H_ */
