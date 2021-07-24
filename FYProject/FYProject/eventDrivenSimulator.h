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

class requestCreation
{
private:
	vector<request> requestVector;

public:
	void requestGenerator();
};

#endif