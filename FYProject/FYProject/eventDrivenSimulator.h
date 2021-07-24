#pragma once
#ifndef EDS_H
#define EDS_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct request 
{
	int identifier;
	int sourceNode;
	int destinationNode;
	int bandwidth;
	double holdingTime;
	double remainingTime;
	double interArrivalTime;
};

struct event
{
	int identifier;
	int sourceNode;
	int destinationNode;
	int bandwidth;
	bool action;
};

class requestCreation
{
private:
	vector<request> requestVector;
	vector<event> eventVector;

public:
	void requestGenerator();
	void eventCreation();
};

#endif