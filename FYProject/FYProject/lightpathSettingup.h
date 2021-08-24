#ifndef LIGHTPATH_H
#define LIGHTPATH_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "graph.h"
#include "LSP.h"
#include "files.h"
#include "Dijkstra.h"

using namespace std;

class lightNode;                    //Classes declaration
class lightpathNetwork;
class LSP;

struct thresholds
{
	float bandwidthThreshold;
	int numLSPthreshold;
	int numOfNodesOfTheNetwork;
};

struct backupStruct
{
	int theWavelength;
	vector<int> intermediateNodes;
	int pos1;
	int pos2;
	int i;
	int j;
	bool protectionType;
	thresholds thresholdVals;
};

class lighpathWavelength
{
private:
	friend class lightNode;          //Creating friend classes
	friend class LSP;
	friend class lightpathNetwork;
	long int LPidentifier;
	int initialBandwidth;            //Initial badwdth of the lightpath
	int availableBandwidth;          //Available bandwidth of the lightpath
	int primaryLSPbandwidth;         //Total bandwidth of the primary LSPs within the lightpath
	vector<int> path;                //Vector to store the intermediate nodes which the lighpath passes
	string lightpathType;            //Define whether a primary or backup lightpath
	bool havingBackup;               //A boolean to identify whether having a backup lightpath
	int wavelength;                  //Wavelength of the lightpath
	int numOfLSPsInLightpath;        //Number of LSPs per a lightnode
	int numOfPrimaryLSPsInLightpath; //Number of primary LSPs per a lightnode
	vector<LSP> LSPvec;              //Details of LSPs are stored

public:
	lighpathWavelength()             //Constructor
	{
		initialBandwidth = 0;
		availableBandwidth = 0;
		primaryLSPbandwidth = 0;
		havingBackup = false;
		numOfLSPsInLightpath = 0;
		numOfPrimaryLSPsInLightpath = 0;
		LSPvec.reserve(1000);
	}
};

class lightNode                          //A node is created when establishing lightpaths
{
private:
	friend class LSP;                     //Creating friend classes
	friend class lightpathNetwork;

	int id;                               //ID of the established node
	lightNode* selfAddress;               //Stores its own address
	struct linkDetails                    //This structure has all the details of a lightpath
	{
		int destinationID;                //Desination ID of the lightpath
		lightNode* destAddress;           //Address of the destination ID
		lighpathWavelength vecObj;        //Object to be used for adding elements to wavelengthAndLSP vector

		vector<lighpathWavelength> wavelengthAndLSP;  //Vector whih stores all the wavelengths and LSPs
	};
	int numOfLPLinksPerNode;              //Counter to record the number of lightpaths connected per a node
	vector<linkDetails> linkVector;       //Vector to hold the details of the lighpaths


public:
	lightNode(int ID);                    //This constructor gives the ID value for a node
	void setSelfAddress(lightNode* temp); //Set the address of the source node of the lightpath
	lightNode* returnSelfAddress();       //Return the source node address
	void addLPlink(vector<int> pathVec, int wavelengthVal, int bandwidthVal, int ID, lightNode* tempDesObj, string type, int LPidentifier);//Lightpaths are added to the nodes
	void addWavelengthToLink(vector<int> pathVec, int destId, int wavelengthToBeAdded, int bandwidthVal, string type, int LPidentifier);
	int returnId();                       //Returns the ID of the node
	void viewLPlinks();                   //View the lightpaths connected for a node
	bool serachLighpathNode(int nodeID);  //Search whether an node is connected to the current node
	lightNode* searchAddress(int val);    //Getting the address of a node which is connected to current node
	
	/**********************************/
	void checkDestinationAndWavelength(int, int);
	int verifyDestinationNode(int);
	void deleteLpLink(long int, int ,int, int, vector<waveLengthNetworks>&, int, string);		// this can be used to  remove item from wavelengthAndLSP vector
	~lightNode() {}
};

class lightpathNetwork                    //Object of this class has to be created in main function
{
protected:
	friend class LSP;
	vector<lightNode> lighpaths;          //This vector stores all the lightpaths 
	int totalnumOfLighpaths;              //The total number of lightpaths within the network
	bool isLinkDisjoint(vector<int> primaryPath, vector<int> testPath, int numOfNodes);

	vector<waveLengthNetworks>& waveLengthNetwork; //parameter for createLightPathBackup function

public:
	lightpathNetwork(vector<waveLengthNetworks>& vw):waveLengthNetwork(vw)
	{
		totalnumOfLighpaths = 0;                             //The count of total number of lightpaths at the begining is zero
		lighpaths.reserve(1000);
	}                                        //The number of lightpaths before creating the network is zero
	void viewAllLighpaths();                                           //Print all the existing lightpaths
	void setANewLighpath(vector<int> shortestPath, int wavelengthSt, string type, int LPidentifier); //Establish a new lighpath
	int checkForAvaialableLPNode(int val);                             //Check whether a node is available within the network
	bool checkForAvilableLightpath(int node1id, int node2id);          //Check whether a lightpath is established
	void checkHeavilyLoadLP(vector<int> posVec, vector<vector<int>> partitionedPath, vector<int> wavelngthVec, bool protectionType, thresholds thresholdVals, bool addOrRemoveLP);//Check whether a lightpath is heavily loaded
	void establishBackupLightpath(backupStruct tempbackupObj);
	void setANewLSP(vector<int> shortestPathLSP, vector<int> wholePath, vector<int> wavelengthVec, lightpathNetwork &obj, string type, int identifier, bool protectionType, thresholds thresholdVals); //Call a function of LSP class to establish a LSP

	vector<vector<int>> lpPAdjacencyMetrix(int bandwidth, int numOfNodes);
	vector<vector<int>> lpBAdjacencyMetrix(vector<int> primaryPath, int numOfNodes);
	vector<vector<int>> getWaveNumbers(int source, int dst,int numOfNodes, int bandwidth, int pathSize = 14);
	vector<int> getBWaveNumber(int source, int dst,int numOfNodes, int bandwidth,vector<int> primaryPath);

	map<int, vector<vector<int>>> mapFromsource(int src, int numOfNodes, int bandwidth, int numOfWaves);

	map<int, map<int, vector<vector<int>>>> mapFromLpGraph(int numOfWaves, int numOfNodes, int bandwidth);


	/*********************************************/
	void releaseEshtablishedLighpath(int, int, long int ,int ,string);	// remove item from lightpath vector
	void getTotalNumberOfLightpaths();				// to get total number of lightpaths (used for testing)

	~lightpathNetwork() {}
};


#endif