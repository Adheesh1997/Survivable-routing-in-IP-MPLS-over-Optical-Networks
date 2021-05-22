#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "graph.h"
using namespace std;

struct LightPathDetails
{
	node* sourceNode;
	node* destinationNode;
	vector<int> intermediateNodes;		
	int wavelength;
	int allocatedBandwidh;
	int availableBandwidth;
	int numberOfLSPs;
};

class lightpathSetup
{
	private:
		vector<LightPathDetails> lightPaths;

	public:
		void settingUpLightPaths(node* sourceNodeAddress, node* destinationNodeAddress, vector<int> interNodeList, string wavelengthSt);

};





//void LightPaths();
//void searchFreeLightPaths();