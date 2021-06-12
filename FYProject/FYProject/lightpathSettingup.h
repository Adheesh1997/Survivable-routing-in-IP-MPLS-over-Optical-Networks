#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "graph.h"
using namespace std;

class lightpathNodeID
{
	private:
		int id;                                          //id of the node
		//vector<vector<int>> lightpathsForTheNode;        //List of lightpaths connected the above node

	public:
		void setID(int ID);                              //Give and ID value for id
		//void setTheListOfLightpaths(vector<int> v1);     //Establish the lighpath
		//void printTheListOfLP();                         //Print the list of lightpaths
};

struct LightPathDetails
{
	lightpathNodeID* sourceNode;            //Source node of the lightpath
	lightpathNodeID* destinationNode;       //Destination node of the lightpath
	vector<int> LPnodeVector;               //A vector to store the nodes of the lightpath
	int wavelength;                         //Wavlength of the lightpath
	int allocatedBandwidh;                  //Total capacity of the lightpath
	int availableBandwidth;                 //Available capacity of the lightpath
	int numberOfLSPs;                       //Total number of LSPs in the lightpath
};

struct lightpathsOfANode
{
	int id;
	vector<vector<int>> v1;
};


class lightpathSetup
{
	private:
		vector<LightPathDetails> lightPaths;    //All the lighpaths are stored in this vector
		vector<lightpathsOfANode> searchVector;
		int numberOfLighpaths;                  //Counter to count the number of established lightpaths
		lightpathsOfANode obj1;

	public:
		lightpathSetup();                           //Constructor to set the above lighpath counter to zero
		void establishLightpath(vector<int> nodeList, string wavelengthSt, lightpathSetup &Obj1);     //function which establishes a lightpath
		void setSearchVector(int id, vector<int> temp, lightpathSetup &Obj1);
};
