#pragma once
#ifndef EDS_H
#define EDS_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct requestDetails 
{
	int identifier;
	int sourceNode;
	int destinationNode;
	int bandwidth;
	double holdingTime;
	double remainingTime;
	double interArrivalTime;
};

struct events
{
	int identifier;
	int sourceNode;
	int destinationNode;
	int bandwidth;
	int action;
};

class requestCreation
{
private:
	int reqID;
	vector<requestDetails> requestVector;
	vector<events> eventVector;

public:
	requestCreation() :reqID(0) {}
	void requestGenerator(int numberOfLSPrequests, double erlang, double meanHoldingTime, int numOfNodes, int lowestLSPcapacity, int highestLSPcapacity);
	void printLSPrequests();
	vector<events> eventCreation();
	void printEvents();
};

#endif