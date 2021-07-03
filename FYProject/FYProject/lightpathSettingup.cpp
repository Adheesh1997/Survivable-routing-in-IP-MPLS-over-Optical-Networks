#include "lightpathSettingup.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include "LSP.h"
#include "graph.h"

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
	temporLink.initialBandwidth = bandwidthVal;
	temporLink.availableBandwidth = bandwidthVal;
	temporLink.path = pathVec;
	temporLink.destinationID = ID;
	temporLink.destAddress = tempDesObj;

	temporLink.vecObj.wavelength = wavelengthVal;
	temporLink.vecObj.numberOfLSPs = 0;
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
		int pathSize = linkVector[i].path.size();

		for (size_t j = 0; j < (pathSize - 1); j++)
			cout << linkVector[i].path[j] << " -> ";
		cout << linkVector[i].path[pathSize - 1] << endl;
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

/*void lightNode::insertLSPtoVec(LSP temp, int id)
{
	for (size_t i = 0; i < linkVector.size(); i++)
	{
		if (id == linkVector[i].destinationID)
			linkVector[i].vecLSP.push_back(temp);
	}

}*/

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

auto lightpathNetwork::checkForAvilableLightpath(int node1id, int node2id)
{
	for (size_t i = 0; i < lighpaths.size(); i++)
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

void lightpathNetwork::setANewLSP(vector<int> shortestPathLSP, string wavelengthLSPstr, lightpathNetwork &obj)
{
	int LSPbandwidth = 10;
	stringstream change(wavelengthLSPstr);
	int intLSPwavelength;
	vector<int> v1 = shortestPathLSP;

	change >> intLSPwavelength;

	if (shortestPathLSP.size() == 2)
	{
		LSP tempObject;

		int pos1 = checkForAvaialableLPNode(v1[0]);
		int pos2 = checkForAvaialableLPNode(v1[1]);
		for (size_t i = 0; i < obj.lighpaths[pos1].linkVector.size(); i++)
		{
			if (obj.lighpaths[pos1].linkVector[i].destinationID == v1[1])
			{
				tempObject.bandwidthOfLSP = LSPbandwidth;
				tempObject.LSPpath = shortestPathLSP;
				tempObject.id = obj.lighpaths[pos1].returnId();
				tempObject.prev = NULL;
				tempObject.next = obj.lighpaths[pos1].linkVector[i].destAddress;


				for (size_t j = 0; j < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++)

					if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == intLSPwavelength)
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject);
			}

			if (obj.lighpaths[pos2].linkVector[i].destinationID == v1[0])
			{
				tempObject.bandwidthOfLSP = LSPbandwidth;
				tempObject.LSPpath = shortestPathLSP;
				tempObject.id = obj.lighpaths[pos2].returnId();
				tempObject.prev = NULL;
				tempObject.next = obj.lighpaths[pos2].linkVector[i].destAddress;


				for (size_t j = 0; j < obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP.size(); j++)

					if (obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].wavelength == intLSPwavelength)
						obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject);
			}
		}

	}

	else if (shortestPathLSP.size() > 2)
	{
		int pathSize = shortestPathLSP.size();
		int numOfIntermediate = pathSize - 2;


		//LSPnode tempNode1;
		LSP tempObject;

		int pos1 = obj.checkForAvaialableLPNode(v1[0]);
		for (size_t i = 0; i < obj.lighpaths[pos1].linkVector.size(); i++)
		{
			if (obj.lighpaths[pos1].linkVector[i].destinationID == v1[1])
			{
				tempObject.bandwidthOfLSP = LSPbandwidth;
				tempObject.LSPpath = shortestPathLSP;
				tempObject.id = obj.lighpaths[pos1].returnId();
				tempObject.prev = NULL;
				tempObject.next = obj.lighpaths[pos1].linkVector[i].destAddress;

				for (size_t j = 0; j < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++)

					if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == intLSPwavelength)
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject);

			}
		}


		for (size_t i = 1; i <= numOfIntermediate; i++)
		{
			int pos2 = obj.checkForAvaialableLPNode(v1[i]);

			size_t k = 0;
			bool switch1 = false, switch2 = false;
			for (; k < obj.lighpaths[pos2].linkVector.size(); k++)
			{
				if (obj.lighpaths[pos2].linkVector[k].destinationID == v1[i - 1])
				{
					tempObject.bandwidthOfLSP = LSPbandwidth;
					tempObject.LSPpath = shortestPathLSP;
					tempObject.id = obj.lighpaths[pos2].returnId();
					tempObject.prev = obj.lighpaths[pos1].returnSelfAddress();
					//tempObject.next = obj.lighpaths[pos3].linkVector[i].destAddress;
					switch1 = true;
				}
				if (obj.lighpaths[pos2].linkVector[k].destinationID == v1[i + 1])
				{
					//tempObject.bandwidthOfLSP = LSPbandwidth;
					//tempObject.LSPidentifier = ;
					//tempObject.id = obj.lighpaths[pos2].returnId();
					//tempObject.prev = obj.lighpaths[pos2].returnSelfAddress();
					tempObject.next = obj.lighpaths[pos2].linkVector[k].destAddress;
					switch2 = true;
				}

				if (switch1 && switch2)
					break;
			}

			int limit = obj.lighpaths[pos2].linkVector[k].wavelengthAndLSP.size();

			for (size_t j = 0; j < limit; j++)

				if (obj.lighpaths[pos2].linkVector[k].wavelengthAndLSP[j].wavelength == intLSPwavelength)
					obj.lighpaths[pos2].linkVector[k].wavelengthAndLSP[j].LSPvec.push_back(tempObject);

			//lighpaths[pos2].linkVector[k].LSPvec.push_back(tempObject);
			pos1 = pos2;
		}

		pos1 = obj.checkForAvaialableLPNode(v1[pathSize - 1]);
		for (size_t i = 0; i < obj.lighpaths[pos1].linkVector.size(); i++)
		{
			if (obj.lighpaths[pos1].linkVector[i].destinationID == v1[pathSize - 2])
			{
				tempObject.bandwidthOfLSP = LSPbandwidth;
				tempObject.LSPpath = shortestPathLSP;
				tempObject.id = obj.lighpaths[pos1].returnId();
				tempObject.prev = obj.lighpaths[pos1].linkVector[i].destAddress;
				tempObject.next = NULL;

				for (size_t j = 0; j < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++)

					if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == intLSPwavelength)
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject);

				//lighpaths[pos1].linkVector[i].LSPvec.push_back(tempObject);
			}
		}

	}


	/*LSP ObjLSP;
	for (size_t i = 0; i < v1.size(); i++)
	{
		ObjLSP.establishANewLSP(v1, intLSPwavelength, obj);
	}

	ObjLSP.bandwidthOfLSP = LSPbandwidth;

	int pos1 = checkForAvaialableLPNode(v1[0]);
	int pos2 = checkForAvaialableLPNode(v1[v1.size() - 1]);

	int k = obj.lighpaths[pos1].linkVector[0].vecLSP.size();


	////////////////////////
	obj.lighpaths[pos1].insertLSPtoVec(ObjLSP, v1[v1.size() - 1]);
	obj.lighpaths[pos2].insertLSPtoVec(ObjLSP, v1[0]);
	*/
	//
}