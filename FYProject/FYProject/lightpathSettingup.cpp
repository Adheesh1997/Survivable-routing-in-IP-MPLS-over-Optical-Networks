#include "lightpathSettingup.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "graph.h"
#include "LSP.h"

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
	temporLink.numberOfLSPs = 0;

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

bool lightNode::serachLighpathNode(int nodeID)
{
	for (int i = 0; i < linkVector.size(); i++)
		if (linkVector[i].destinationID == nodeID)
			return true;
	return false;
}

int lightNode::searchDestination()
{
	return linkVector[0].destinationID;
}

lightNode* lightNode::searchAddress(int temp)
{
	for (int i = 0; i < linkVector.size(); i++)
		if (linkVector[i].destinationID == temp)
			return linkVector[i].destAddress;
}

////////////////////////////////// 
lightNode* lightpathNetwork::adrressReturn(int val)
{
	int pos = lighpaths[val].searchDestination();
	int temp = lighpaths[val].returnId();
	lightNode* selfAddress = lighpaths[pos].searchAddress(temp);
	return selfAddress;
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
	
	int check1 = checkForAvaialableLPNode(shortestPath[0]);
	int check2 = checkForAvaialableLPNode(shortestPath[vecSize-1]);

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
			lightNode* temp = adrressReturn(check2);
			lighpaths[check2].addLPlink(shortestpathVec, lamda, bandwidth, lpNodeS);
			lpNodeS.addLPlink(shortestpathVec, lamda, bandwidth, *temp);

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

int lightpathNetwork::checkForAvaialableLPNode(int val)
{
	for (int i = 0; i < lighpaths.size(); i++)
		if (lighpaths[i].returnId() == val)
			return i;

	return -1;
}

bool lightpathNetwork::checkForAvilableLightpath(int node1id, int node2id)
{
	for (int i = 0; i < lighpaths.size(); i++)
	{
		if (lighpaths[i].returnId() == node1id)
		{
			bool check = lighpaths[i].serachLighpathNode(node2id);
			/*if (check)
				cout << "Lightpath between " << node1id << " to " << node2id << " exists\n";
			else
				cout << "Lightpath between " << node1id << " to " << node2id << " doesn't exist\n";*/
			return check;
		}
		if (lighpaths[i].returnId() == node2id)
		{
			bool check = lighpaths[i].serachLighpathNode(node1id);
			/*if (check)
				cout << "Lightpath between " << node1id << " to " << node2id << " exists\n";
			else
				cout << "Lightpath between " << node1id << " to " << node2id << " doesn't exist\n";*/
			return check;
		}
	}
	//cout << "Lightpath between " << node1id << " to " << node2id << " doesn't exist\n";
	return false;
}

void lightpathNetwork::setANewLSP(vector<int> shortestPathLSP, string wavelengthLSPstr)
{
	int LSPbandwidth = 10;
	stringstream change(wavelengthLSPstr);
	int intLSPwavelength;
	vector<int> v1 = shortestPathLSP;

	change >> intLSPwavelength;

	//LSP ObjLSP;
	//ObjLSP.establishANewLSP(v1, intLSPwavelength, LSPbandwidth);
}