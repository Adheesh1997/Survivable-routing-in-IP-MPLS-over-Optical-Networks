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
	vector<node*> intermediateNodes;		

};

class lightpathSetup
{
	private:
		vector<LightPathDetails> lightPaths;

	public:
		void settingUpLightPaths(node* sourceNodeAddress, node* destinationNodeAddress, vector<int> interNodeList);

};








//void LightPaths();
//void searchFreeLightPaths();