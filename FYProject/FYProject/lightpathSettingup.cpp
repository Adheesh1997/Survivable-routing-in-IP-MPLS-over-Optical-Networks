#include "lightpathSettingup.h"
#include <iostream>
#include <vector>
#include <sstream>
#include "graph.h"

using namespace std;


void lightpathSetup::establishLightpath(vector<int> interNodeList, string wavelengthSt)
{
	LightPathDetails newLightPath;
	

	int numberOfNodes = newLightPath.intermediateNodes.size();		//Number of nodes in the shortespath for the lightpath
	lightpathNode sourceAndDestNodes[2];							//Object array of size 2 for source node and destination node
	

	sourceAndDestNodes[0].id = newLightPath.intermediateNodes[0];					//Assigning the id of the source node
	sourceAndDestNodes[1].id = newLightPath.intermediateNodes[numberOfNodes - 1];	//Assigning the id of the destination node


	newLightPath.sourceNode = &sourceAndDestNodes[0];				//Assining the address of source node
	newLightPath.destinationNode = &sourceAndDestNodes[1];			//Assining the address of destination node


	if (numberOfNodes > 2)
		newLightPath.intermediateNodes = { interNodeList.begin() + 1, interNodeList.end() - 1 };


	stringstream geek(wavelengthSt);
	geek >> newLightPath.wavelength;								//Convert the string value of wavelength to integer

	lightPaths.push_back(newLightPath);								//Push back the lightpath to the lightpath vector
	numberOfLighpaths++;											//Increment the lighpath counter
}
