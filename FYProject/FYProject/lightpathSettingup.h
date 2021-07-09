#ifndef LIGHTPATH_H
#define LIGHTPATH_H

#include <iostream>
#include <string>
#include <vector>
#include "graph.h"
#include "LSP.h"
using namespace std;

class lightNode;
class lightpathNetwork;
class LSP;

struct lighpathWavelength
{
	int initialBandwidth;            //Initial badwdth of the lightpath
	int availableBandwidth;          //Available bandwidth of the lightpath
	vector<int> path;                //Vector to store the intermediate nodes which the lighpath passes
	int wavelength;                  //Wavelength of the lightpath
	int numOfLSPsInLightpath;
	vector<LSP> LSPvec;
};

class lightNode                            //A node is created when establishing lightpaths
{
private:
	friend class LSP;
	friend class lightpathNetwork;

	int id;                              //ID of the established node
	lightNode* selfAddress;              //Stores its own address
	struct linkDetails                   //This structure has all the details of a lightpath
	{
		int destinationID;               //Desination ID of the lightpath
		lightNode* destAddress;          //Address of the destination ID
		lighpathWavelength vecObj;

		vector<lighpathWavelength> wavelengthAndLSP;
	};
	int numOfLPLinksPerNode;             //Counter to record the number of lightpaths connected per a node
	vector<linkDetails> linkVector;      //Vector to hold the details of the lighpaths


public:
	lightNode(int ID);                   //This constructor gives the ID value for a node
	void setSelfAddress(lightNode* temp);//Set the address of the source node of the lightpath
	lightNode* returnSelfAddress();      //Return the source node address
	void addLPlink(vector<int> pathVec, int wavelengthVal, int bandwidthVal, int ID, lightNode* tempDesObj);//Lightpaths are added to the nodes
	int returnId();                      //Returns the ID of the node
	void viewLPlinks();                  //View the lightpaths connected for a node
	bool serachLighpathNode(int nodeID); //Search whether an node is connected to the current node
	lightNode* searchAddress(int val);
	//void removeLPlink();
};

class lightpathNetwork                       //Object of this class has to be created in main function
{
protected:
	friend class LSP;
	vector<lightNode> lighpaths;         //This vector stores all the lightpaths 
	int totalnumOfLighpaths;             //The total number of lightpaths within the network
	bool isLinkDisjoint(vector<int> primaryPath, vector<int> testPath, int numOfNodes);

public:
	lightpathNetwork() : totalnumOfLighpaths(0) {}                      //The number of lightpaths before creating the network is zero
	void viewAllLighpaths();                                           //Print all the existing lightpaths
	void setANewLighpath(vector<int> shortestPath, string wavelength); //Establish a lighpath
	int checkForAvaialableLPNode(int val);                             //Check whether a node is available within the network
	bool checkForAvilableLightpath(int node1id, int node2id);          //Check whether a lightpath is established

	void setANewLSP(vector<int> shortestPathLSP, string wavelengthLSPstr, lightpathNetwork &obj);

	vector<vector<int>> lpPAdjacencyMetrix(int bandwidth, int numOfNodes);
	vector<vector<int>> lpBAdjacencyMetrix(vector<int> primaryPath, int numOfNodes);
};


#endif