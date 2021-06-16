#include "lightpathSettingup.h"
#include <iostream>
#include <vector>
#include <sstream>
#include "graph.h"

using namespace std;


/************  Begining of the lightpathNodeID class  ************/

/*void lightpathNodeID :: setID(int ID)
{
	id = ID;
}*/




/************  End of the lightpathNodeID class  ************/




/************  Begining of the lightpathSetup class  ************/
/*
lightpathSetup::lightpathSetup()
{
	numberOfLighpaths = 0;
}

void lightpathSetup::establishLightpath(vector<int> nodeList, string wavelengthSt, lightpathSetup &Obj1)
{
	LightPathDetails newLightPath;
	
	
	newLightPath.LPnodeVector = {nodeList.begin(), nodeList.end()}; //Shortestpath for lightpath given by dijkstra's algorithm is stored
	int numberOfNodes = newLightPath.LPnodeVector.size();           //Number of nodes in the shortespath for the lightpath
	lightpathNodeID sourceAndDestNodes[2];                          //Object array of size 2 for source node and destination node
	


	
	sourceAndDestNodes[0].setID(newLightPath.LPnodeVector[0]);                   //Assigning the id of the source node
	//sourceAndDestNodes[0].setTheListOfLightpaths(newLightPath.LPnodeVector);     //Assigning the list of nodes in LP for source node
	setSearchVector(newLightPath.LPnodeVector[0], nodeList, Obj1);

	sourceAndDestNodes[1].setID(newLightPath.LPnodeVector[numberOfNodes - 1]);   //Assigning the id of the destination node
	//sourceAndDestNodes[1].setTheListOfLightpaths(newLightPath.LPnodeVector);     //Assigning the list of nodes in LP for destination node
	
	//sourceAndDestNodes[0].printTheListOfLP();                       //Print the list of lightpaths for the source node
	//sourceAndDestNodes[1].printTheListOfLP();                       //Print the list of lightpaths for the destination node
	
	newLightPath.sourceNode = &sourceAndDestNodes[0];               //Assining the address of source node
	newLightPath.destinationNode = &sourceAndDestNodes[1];          //Assining the address of destination node
	
	
	

	stringstream geek(wavelengthSt);
	geek >> newLightPath.wavelength;                                //Convert the string value of wavelength to integer

	lightPaths.push_back(newLightPath);                             //Push back the lightpath to the lightpath vector
	numberOfLighpaths++;                                            //Increment the lighpath counter
}

void lightpathSetup :: setSearchVector(int id, vector<int> temp, lightpathSetup &Obj1)
{
	bool check = true;
	for (int i = 0; i < searchVector.size(); i++)
	{
		if (searchVector[i].id == id)
		{
			Obj1.searchVector[i].v1.push_back(temp);
			check = false;
			break;
		}
	}
	if (check)
	{
		lightpathsOfANode tempObj2;
		tempObj2.id = id;
		tempObj2.v1.push_back(temp);
		Obj1.searchVector.push_back(tempObj2);
	}
	searchVector;
}*/
/************  End of the lightpathSetup class  ************/

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

void lightpathNetwork::setANewLighpath(vector<int> shortestPath, string wavelengthSt)
{
	vector<int> shortestpathVec = shortestPath;
	stringstream geek(wavelengthSt);

	int lamda;
	geek >> lamda;


	int vecSize = shortestPath.size();
	int bandwidth = 50;
	int check1 = 0, check2 = 0;
	//int check1 = checkForAvaialableNodes(shortestPath[0]);
	//int check2 = checkForAvaialableNodes(shortestPath[vecSize-1]);

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

		}

		

	}

}