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
	selfAddress = NULL;
	linkVector.reserve(100);
}

void lightNode::setSelfAddress(lightNode* temp)
{
	selfAddress = temp;
}

lightNode* lightNode::returnSelfAddress()
{
	return selfAddress;
}

void lightNode::addLPlink(vector<int> pathVec, int wavelengthVal, int bandwidthVal, int ID, lightNode* tempDesObj, string type)
{
	linkDetails temporLink;
	temporLink.destinationID = ID;
	temporLink.destAddress = tempDesObj;

	temporLink.vecObj.initialBandwidth = bandwidthVal;
	temporLink.vecObj.availableBandwidth = bandwidthVal;
	temporLink.vecObj.path = pathVec;
	temporLink.vecObj.type = type;
	temporLink.vecObj.wavelength = wavelengthVal;
	temporLink.vecObj.numOfLSPsInLightpath = 0;
	temporLink.wavelengthAndLSP.push_back(temporLink.vecObj);

	linkVector.push_back(temporLink);
	int temp = linkVector.size();
	linkVector[temp - 1].wavelengthAndLSP.reserve(100);
	numOfLPLinksPerNode++;
}

void lightNode::addWavelengthToLink(vector<int> pathVec, int destId, int wavelengthToBeAdded, int bandwidthVal, string type)
{
	for (size_t i = 0; i < linkVector.size(); i++)
	{
		if (linkVector[i].destinationID == destId)
		{
			linkVector[i].vecObj.initialBandwidth = bandwidthVal;
			linkVector[i].vecObj.availableBandwidth = bandwidthVal;
			linkVector[i].vecObj.path = pathVec;
			linkVector[i].vecObj.type = type;
			linkVector[i].vecObj.wavelength = wavelengthToBeAdded;
			linkVector[i].vecObj.numOfLSPsInLightpath = 0;

			linkVector[i].wavelengthAndLSP.push_back(linkVector[i].vecObj);
		}
	}
}

int lightNode::returnId()
{
	return id;
}

void lightNode::viewLPlinks()
{
	//cout << "linkVector.size() = " << linkVector.size() << endl;
	for (size_t i = 0; i < linkVector.size(); i++)
	{
		for (size_t j = 0; j < linkVector[i].wavelengthAndLSP.size(); j++)
		{
			int pathSize = linkVector[i].wavelengthAndLSP[j].path.size();
			cout << "Wavelength = " << linkVector[i].wavelengthAndLSP[j].wavelength << endl;
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

lightpathNetwork::lightpathNetwork()
{
	totalnumOfLighpaths = 0;
	lighpaths.reserve(100);
}

void lightpathNetwork::viewAllLighpaths()
{
	for (size_t i = 0; i < lighpaths.size(); i++)
	{
		cout << "Lighpath node id - " << lighpaths[i].returnId() << endl;
		lighpaths[i].viewLPlinks();
		cout << endl << endl;
	}
}

void lightpathNetwork::setANewLighpath(vector<int> shortestPath, string wavelengthSt, string type)
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

			lpNodeD.addLPlink(shortestpathVec, lamda, bandwidth, lpNodeS.returnId(), addr1, type);
			lpNodeS.addLPlink(shortestpathVec, lamda, bandwidth, lpNodeD.returnId(), addr2, type);

			lighpaths.push_back(lpNodeS);
			lighpaths.push_back(lpNodeD);

			totalnumOfLighpaths++;
		}

		else
		{
			lighpaths[check2].addLPlink(shortestpathVec, lamda, bandwidth, lpNodeS.returnId(), addr1, type);
			//lighpaths[check2].addWavelengthToLink(shortestpathVec, lpNodeS.returnId(), lamda, bandwidth);
			lpNodeS.addLPlink(shortestpathVec, lamda, bandwidth, lighpaths[check2].returnId(), lighpaths[check2].returnSelfAddress(), type);

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

			lpNodeD.addLPlink(shortestpathVec, lamda, bandwidth, lighpaths[check1].returnId(), lighpaths[check1].returnSelfAddress(), type);
			lighpaths[check1].addLPlink(shortestpathVec, lamda, bandwidth, lpNodeD.returnId(), addr2, type);
			//lighpaths[check1].addWavelengthToLink(shortestpathVec, lpNodeD.returnId(), lamda, bandwidth);

			lighpaths.push_back(lpNodeD);

			totalnumOfLighpaths++;
		}

		else
		{
			bool flag = false;
			for (size_t i = 0; i < lighpaths[check1].linkVector.size(); i++)
			{
				if (lighpaths[check1].linkVector[i].destinationID == lighpaths[check2].returnId())
				{
					flag = true;
					break;
				}
			}

			if (flag)
			{
				lighpaths[check1].addWavelengthToLink(shortestpathVec, lighpaths[check2].returnId(), lamda, bandwidth, type);
				lighpaths[check2].addWavelengthToLink(shortestpathVec, lighpaths[check1].returnId(), lamda, bandwidth, type);
				if (type == "bp")
				{
					//************************************************** Establishing a identifier for primary lightpaths
				}
			}
			else
			{
				lighpaths[check1].addLPlink(shortestpathVec, lamda, bandwidth, lighpaths[check2].returnId(), lighpaths[check2].returnSelfAddress(), type);
				lighpaths[check2].addLPlink(shortestpathVec, lamda, bandwidth, lighpaths[check1].returnId(), lighpaths[check2].returnSelfAddress(), type);
			}

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

bool lightpathNetwork::checkHeavilyLoadLP(vector<int> posVec, int wavelngth)
{
	if (posVec.size() == 2)
	{
		int pos1 = posVec[0];
		int pos2 = posVec[1];

		for (size_t i = 0; i < lighpaths[pos1].linkVector.size(); i++)
		{
			if(lighpaths[pos1].linkVector[i].destinationID==lighpaths[pos2].returnId())
				for (size_t j = 0; j < lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++)
				{
					if (lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == wavelngth && lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].type == "pp")
					{
						if (!(lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].havingBackup))
						{
							int initBandwidth = lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].initialBandwidth;
							int availBandwidth = lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth;

							float usedBandProportion = (initBandwidth - availBandwidth) / initBandwidth;
							if (usedBandProportion >= 0.75)
							{
								//check whether it has a backup lighpath
								//if(no backup)
								//request for a backup lightpath
							}

							else if (lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath > 2)
							{
								//check whether it has a backup lighpath
								//if(no backup)
								//request for a backup lightpath
							}
						}
					}
				}
		}
	}
	else if (posVec.size() > 2)
	{

	}
	for (size_t i = 0; i < posVec.size(); i++)
	{
		int pos1 = posVec[i];

	}
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
