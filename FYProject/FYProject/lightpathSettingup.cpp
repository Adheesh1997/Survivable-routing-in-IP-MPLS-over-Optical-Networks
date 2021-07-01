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

void lightNode::setSelfAddress(lightNode* temp)
{
	selfAddress = temp;
}

lightNode* lightNode::returnSelfAddress()
{
	return selfAddress;
}

void lightNode::addLPlink(vector<int> pathVec, int wavelengthVal, int bandwidthVal, int ID, lightNode* tempDesObj)
{
	linkDetails temporLink;
	temporLink.wavelength = wavelengthVal;
	temporLink.initialBandwidth = bandwidthVal;
	temporLink.availableBandwidth = bandwidthVal;
	temporLink.path = pathVec;
	temporLink.destinationID = ID;
	temporLink.destAddress = tempDesObj;
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

lightNode* lightNode::searchAddress(int temp)
{
	for (int i = 0; i < linkVector.size(); i++)
		if (linkVector[i].destinationID == temp)
			return linkVector[i].destAddress;
}

//////////////////////////////////


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
		lightNode* addr1 = &lpNodeS;
		lpNodeS.setSelfAddress(addr1);

		if (check2 == -1)
		{
			lightNode lpNodeD(shortestPath[vecSize-1]);
			lightNode* addr2 = &lpNodeD;
			lpNodeD.setSelfAddress(addr2);

			lpNodeD.addLPlink(shortestpathVec, lamda, bandwidth, lpNodeS.returnId(), addr1);
			lpNodeS.addLPlink(shortestpathVec, lamda, bandwidth, lpNodeD.returnId(), addr2);

			lighpaths.push_back(lpNodeS);
			lighpaths.push_back(lpNodeD);
			
			totalnumOfLighpaths++;
		}

		else
		{
			lighpaths[check2].addLPlink(shortestpathVec, lamda, bandwidth, lpNodeS.returnId(), addr1);
			lpNodeS.addLPlink(shortestpathVec, lamda, bandwidth, lighpaths[check2].returnId(), lighpaths[check2].returnSelfAddress());

			lighpaths.push_back(lpNodeS);
			
			totalnumOfLighpaths++;
		}

	}

	else
	{

		if (check2 == -1)
		{
			lightNode lpNodeD(shortestPath[vecSize - 1]);
			lightNode* addr2 = &lpNodeD;
			lpNodeD.setSelfAddress(addr2);

			lpNodeD.addLPlink(shortestpathVec, lamda, bandwidth, lighpaths[check1].returnId(), lighpaths[check1].returnSelfAddress());
			lighpaths[check1].addLPlink(shortestpathVec, lamda, bandwidth, lpNodeD.returnId(), addr2);

			lighpaths.push_back(lpNodeD);
			
			totalnumOfLighpaths++;
		}

		else
		{
			lighpaths[check1].addLPlink(shortestpathVec, lamda, bandwidth, lighpaths[check2].returnId(), lighpaths[check2].returnSelfAddress());
			lighpaths[check2].addLPlink(shortestpathVec, lamda, bandwidth, lighpaths[check1].returnId(), lighpaths[check2].returnSelfAddress());

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

void lightpathNetwork::setANewLSP(vector<int> shortestPathLSP, string wavelengthLSPstr, lightpathNetwork obj)
{
	int LSPbandwidth = 10;
	stringstream change(wavelengthLSPstr);
	int intLSPwavelength;
	vector<int> v1 = shortestPathLSP;

	change >> intLSPwavelength;

	//LSP ObjLSP;
	//ObjLSP.establishANewLSP(v1, intLSPwavelength, LSPbandwidth, obj);
}