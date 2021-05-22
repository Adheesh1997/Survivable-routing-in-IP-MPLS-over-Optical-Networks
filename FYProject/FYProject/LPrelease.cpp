#include "LPrelease.h"
#include <iostream>
#include <vector>
#include <sstream>
#include "graph.h"

using namespace std;


void lightpathSetup::establishLightpath(vector<int> interNodeList, string wavelengthSt)
{
	LightPathDetails newLightPath;
	

	int numOfNodes = 14;////////////////////////////////
	vector<node> nodes;////////////////////////////////

	int numberOfNodes = newLightPath.intermediateNodes.size();	//Number of nodes in the shortespath for lightpath



	bool check1 = false, check2 = false;						//Boolean values to break the for loop

	for (int i = 0; i < numOfNodes; i++)						//This for loop gets the addresses of the source and destination nodes
	{															//of the lightpath
		cout << endl << endl << "node " << nodes[i].getId() << endl;
		nodes[i].printLinks();
		
		if (nodes[i].getId() == newLightPath.intermediateNodes[0])
		{
			newLightPath.sourceNode = &nodes[i];
			check1 = true;
		}

		if (nodes[i].getId() == newLightPath.intermediateNodes[numberOfNodes-1])
		{
			newLightPath.destinationNode = &nodes[i];
			check2 = true;
		}

		if (check1 && check2)
			break;

	}


	if (numberOfNodes > 2)
		newLightPath.intermediateNodes = { interNodeList.begin() + 1, interNodeList.end() - 1 };

	stringstream geek(wavelengthSt);
	geek >> newLightPath.wavelength;

	lightPaths.push_back(newLightPath);
	numberOfLighpaths++;
}
