#include "lightpathSettingup.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "graph.h"

using namespace std;


lightNode::lightNode(int ID)
{
	id = ID;
	numOfLPLinksPerNode = 0;
}

void lightNode::addLPlink(vector<int> pathVec, int wavelengthVal, int bandwidthVal, lightNode &tempDesObj)
{
	linkDetails temporLink;
	temporLink.wavelength = wavelengthVal;
	temporLink.initialBandwidth = bandwidthVal;
	temporLink.availableBandwidth = bandwidthVal;
	temporLink.path = pathVec;
	temporLink.destinationID = tempDesObj.returnId();
	temporLink.destAddress = &tempDesObj;

	linkVector.push_back(temporLink);
	numOfLPLinksPerNode++;
}

int lightNode::returnId()
{
	return id;
}

void lightNode::viewLPlinks()
{
	for (int i = 0; i < linkVector.size(); i++)
	{
		int pathSize = linkVector[i].path.size();

		for (int j = 0; j < (pathSize - 1); j++)
			cout << linkVector[i].path[j] << " -> ";
		cout << linkVector[i].path[pathSize - 1] << endl;
	}
}



void lightpathNetwork::viewAllLighpaths()
{
	for (int i = 0; i < lighpaths.size(); i++)
	{
		cout << "Lighpath node id - " << lighpaths[i].returnId() << endl;
		lighpaths[i].viewLPlinks();
		cout << endl << endl;
	}
}

void lightpathNetwork::setANewLighpath(vector<int> shortestPath, string wavelengthSt)
{
	vector<int> shortestpathVec = shortestPath;
	stringstream geek(wavelengthSt);

	int lamda;
	geek >> lamda;


	int vecSize = shortestPath.size();
	int bandwidth = 50;
	
	int check1 = checkForAvaialableNode(shortestPath[0]);
	int check2 = checkForAvaialableNode(shortestPath[vecSize-1]);

	if (check1 == -1)
	{
		lightNode lpNodeS(shortestpathVec[0]);
		

		if (check2 == -1)
		{
			lightNode lpNodeD(shortestPath[vecSize-1]);

			lpNodeD.addLPlink(shortestpathVec, lamda, bandwidth, lpNodeS);
			lpNodeS.addLPlink(shortestpathVec, lamda, bandwidth, lpNodeD);

			lighpaths.push_back(lpNodeS);
			lighpaths.push_back(lpNodeD);
			//sort() lightpaths

			totalnumOfLighpaths++;
		}

		else
		{
			lighpaths[check2].addLPlink(shortestpathVec, lamda, bandwidth, lpNodeS);
			lpNodeS.addLPlink(shortestpathVec, lamda, bandwidth, lighpaths[check2]);

			lighpaths.push_back(lpNodeS);
			//sort() lightpaths

			totalnumOfLighpaths++;
		}

	}

	else
	{
		if (check2 == -1)
		{
			lightNode lpNodeD(shortestPath[vecSize - 1]);

			lpNodeD.addLPlink(shortestpathVec, lamda, bandwidth, lighpaths[check1]);
			lighpaths[check1].addLPlink(shortestpathVec, lamda, bandwidth, lpNodeD);

			lighpaths.push_back(lpNodeD);
			//sort() lightpaths

			totalnumOfLighpaths++;
		}

		else
		{
			lighpaths[check1].addLPlink(shortestpathVec, lamda, bandwidth, lighpaths[check2]);
			lighpaths[check2].addLPlink(shortestpathVec, lamda, bandwidth, lighpaths[check1]);

			totalnumOfLighpaths++;
		}
	}
}

int lightpathNetwork::checkForAvaialableNode(int val)
{
	for (int i = 0; i < lighpaths.size(); i++)
		if (lighpaths[i].returnId() == val)
			return i;

	return -1;
}