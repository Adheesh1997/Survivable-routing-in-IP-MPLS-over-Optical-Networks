#include "LPrelease.h"
#include <iostream>
#include <vector>
#include <sstream>
#include "graph.h"

using namespace std;

void lightpathSetup::settingUpLightPaths(node* sourceNodeAddress, node* destinationNodeAddress, vector<int> interNodeList, string wavelengthSt)
{
	stringstream geek(wavelengthSt);

	LightPathDetails newLightPath;
	newLightPath.sourceNode = sourceNodeAddress;
	newLightPath.destinationNode = destinationNodeAddress;
	newLightPath.intermediateNodes = interNodeList;
	geek >> newLightPath.wavelength;


	lightPaths.push_back(newLightPath);

}
/*void LightPaths()
{
	cout << "\nHeloooooooo\n";
}*/

