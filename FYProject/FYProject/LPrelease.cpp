#include "LPrelease.h"
#include <iostream>
#include "graph.h"
using namespace std;

struct establishedLightPaths
{
	int sourceNode;
	int destinationNode;
	int wavelength;
	int allocatedBandwidh;
	int availableBandwidth;
};

void LightPaths()
{
	cout << "\nHeloooooooo\n";
}

void searchFreeLightPaths()
{
	for (int i = 0; i < numOfNodes; i++)
	{
		cout << endl << endl << "node " << nodes[i].getId() << endl;
		nodes[i].printLinks();

	}
}