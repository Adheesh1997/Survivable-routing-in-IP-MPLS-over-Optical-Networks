#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "graph.h"
using namespace std;


class lightNode
{
	private:
		int id;
		struct linkDetails
		{
			int wavelength;
			int initialBandwidth;
			int availableBandwidth;
			vector<int> path;
			int destinationID;
			lightNode* destAddress;
		};
		int numOfLPLinksPerNode;
		
		vector<linkDetails> linkVector;

	public:
		lightNode(int ID);
		void addLPlink(vector<int> pathVec, int wavelengthVal, int bandwidthVal, lightNode &tempDesObj);
		int returnId();
		void viewLPlinks();
		//void removeLPlink();
};

class lightpathNetwork
{
	private:
		vector<lightNode> lighpaths;
		int totalnumOfLighpaths;

	public:
		lightpathNetwork() : totalnumOfLighpaths(0){}
		void viewAllLighpaths();
		void setANewLighpath(vector<int> shortestPath, string wavelength);
		int checkForAvaialableNode(int val);
};