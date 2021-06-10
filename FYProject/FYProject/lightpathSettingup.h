#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "graph.h"
using namespace std;

struct lightpathNode
{
	int id;
};


struct LightPathDetails
{
	lightpathNode* sourceNode;				//Source node of the lightpath
	lightpathNode* destinationNode;			//Destination node of the lightpath
	vector<int> intermediateNodes;			//A vector to store the intermediated nodes of the lightpath
	int wavelength;							//Wavlength of the lightpath
	int allocatedBandwidh;					//Total capacity of the lightpath
	int availableBandwidth;					//Available capacity of the lightpath
	int numberOfLSPs;						//Total number of LSPs in the lightpath
};


class lightpathSetup
{
	private:
		vector<LightPathDetails> lightPaths;	//All the lighpaths are stored in this vector
		int numberOfLighpaths;					//Counter to count the number of established lightpaths

	public:
		lightpathSetup() : numberOfLighpaths(0) {};								//Constructor to set the above lighpath counter to zero
		void establishLightpath(vector<int> interNodeList, string wavelengthSt);//function which establishes a lightpath
};
