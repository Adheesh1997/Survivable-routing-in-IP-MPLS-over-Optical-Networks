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
	temporLink.destinationID = ID;
	temporLink.destAddress = tempDesObj;

	temporLink.vecObj.initialBandwidth = bandwidthVal;
	temporLink.vecObj.availableBandwidth = bandwidthVal;
	temporLink.vecObj.path = pathVec;
	temporLink.vecObj.wavelength = wavelengthVal;
	temporLink.vecObj.numOfLSPsInLightpath = 0;
	temporLink.wavelengthAndLSP.push_back(temporLink.vecObj);

	linkVector.push_back(temporLink);
	numOfLPLinksPerNode++;
}



int lightNode::returnId()
{
	return id;
}

void lightNode::viewLPlinks()
{
	for (size_t i = 0; i < linkVector.size(); i++)
	{
		for (size_t j = 0; j < linkVector[i].wavelengthAndLSP.size(); j++)
		{
			int pathSize = linkVector[i].wavelengthAndLSP[j].path.size();

			for (size_t k = 0; k < (pathSize - 1); k++)
				cout << linkVector[i].wavelengthAndLSP[j].path[k] << " -> ";
			cout << linkVector[i].wavelengthAndLSP[j].path[pathSize - 1] << endl;
		}
	}
}

bool lightNode::serachLighpathNode(int nodeID)
{
	for (size_t i = 0; i < linkVector.size(); i++)
		if (linkVector[i].destinationID == nodeID)
			return true;
	return false;
}

lightNode* lightNode::searchAddress(int temp)
{
	for (size_t i = 0; i < linkVector.size(); i++)
		if (linkVector[i].destinationID == temp)
			return linkVector[i].destAddress;
}


//////////////////////////////////


void lightpathNetwork::viewAllLighpaths()
{
	for (size_t i = 0; i < lighpaths.size(); i++)
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
	int check2 = checkForAvaialableLPNode(shortestPath[vecSize - 1]);

	if (check1 == -1)
	{
		lightNode lpNodeS(shortestpathVec[0]);
		lightNode* addr1 = &lpNodeS;
		lpNodeS.setSelfAddress(addr1);

		if (check2 == -1)
		{
			lightNode lpNodeD(shortestPath[vecSize - 1]);
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
	for (size_t i = 0; i < lighpaths.size(); i++)
		if (lighpaths[i].returnId() == val)
			return i;

	return -1;
}

bool lightpathNetwork::checkForAvilableLightpath(int node1id, int node2id)
{
	for (size_t i = 0; i < lighpaths.size(); i++)
	{
		if (lighpaths[i].returnId() == node1id)
		{
			bool check = lighpaths[i].serachLighpathNode(node2id);
			/*if (check)
				cout << "Lightpath between " << node1id << " to " << node2id << " exists\n";
			else*/
			if (!check)
				cout << "Lightpath between " << node1id << " to " << node2id << " doesn't exist\n";
			return check;
		}
		if (lighpaths[i].returnId() == node2id)
		{
			bool check = lighpaths[i].serachLighpathNode(node1id);
			/*if (check)
				cout << "Lightpath between " << node1id << " to " << node2id << " exists\n";
			else*/
			if (!check)
				cout << "Lightpath between " << node1id << " to " << node2id << " doesn't exist\n";
			return check;
		}
	}
	cout << "Lightpath between " << node1id << " to " << node2id << " doesn't exist\n";
	return false;
}

void lightpathNetwork::setANewLSP(vector<int> shortestPathLSP, string wavelengthLSPstr, lightpathNetwork &obj)
{
	LSP Object;
	Object.makeLSP(shortestPathLSP, wavelengthLSPstr, obj);
}

// Generate adajacency metrix from LP network
vector<vector<int>> lightpathNetwork::lpPAdjacencyMetrix(int bandwidth, int numOfNodes)
{
	/*
	Parameters_
		1.bandwidth - bandwith of LSP request
		2.numOfNodes - number of nodes in the network (14 for NFSNet)

	return
		*generated adajacency metrix that need to find shortest path for primary LSP
	*/
	
    vector<vector<int>> arr(numOfNodes,vector<int>(numOfNodes,0));
	
	for (int i = 0; i < lighpaths.size(); i++)
	{
		for(int j = 0; j < lighpaths[i].linkVector.size(); j++)
		{
			for(int h = 0; h < lighpaths[i].linkVector[j].wavelengthAndLSP.size(); h++)
			{
				if(lighpaths[i].linkVector[j].wavelengthAndLSP[h].availableBandwidth >= bandwidth)
				{
					int sourceLP = lighpaths[i].returnId();
					int dstLP = lighpaths[i].linkVector[j].destinationID;
	
					arr[sourceLP][dstLP] = 40;
				}
			}
			
		}
	}

	return arr;
}

// Find given 2 paths are link disjoint or not
bool lightpathNetwork::isLinkDisjoint(vector<int> primaryPath, vector<int> testPath, int numOfNodes)
{
	/*
	Parameters_
		1.primaryPath - path that primary lsp going through
		2.testPath - path need to be checked with primaryPath link disjoint or not
		3.numOfNodes - number of nodes in the network (14 for NFSNet)

	return_
		* true - if 2 path linkdisjoint (2 paths never meet in a link)
		* flase - if 2 paths not link disjoint (2 paths meet atleat in one link)
	*/
    vector<vector<int>> arr(numOfNodes,vector<int>(numOfNodes,0));

	for(int i = 0; i < primaryPath.size()-1; i++)
	{
		arr[primaryPath[i]][primaryPath[i+1]] = 1;
	}

	for(int i = 0; i < testPath.size()-1; i++)
	{
		if(arr[testPath[i]][testPath[i+1]])
			return false;
	}

	return true;
}

//Generate adjacency metrix to find shortest path for backup LSP
vector<vector<int>> lightpathNetwork::lpBAdjacencyMetrix(vector<int> primaryPath, int numOfNodes)
{
    vector<vector<int>> arr(numOfNodes,vector<int>(numOfNodes,0));

	for (int i = 0; i < lighpaths.size(); i++)
	{
		for(int j = 0; j < lighpaths[i].linkVector.size(); j++)
		{
			for(int h = 0; h < lighpaths[i].linkVector[j].wavelengthAndLSP.size(); h++)
			{
				if(isLinkDisjoint(primaryPath,lighpaths[i].linkVector[j].wavelengthAndLSP[h].path, numOfNodes))
				{
					int sourceLP = lighpaths[i].returnId();
					int dstLP = lighpaths[i].linkVector[j].destinationID;
	
					arr[sourceLP][dstLP] = 40;
				}
			}
			
		}
	}

	return arr;

	
}
