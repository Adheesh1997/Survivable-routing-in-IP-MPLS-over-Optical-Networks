#include "lightpathSettingup.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <string>

#include "graph.h"
#include "LSP.h"
#include "files.h"

using namespace std;


lightNode::lightNode(int ID)
{
	/*
	* Parameter
		int ID - An id value for the nodes being created
	*/
	id = ID;                        
	numOfLPLinksPerNode = 0;
	selfAddress = NULL;
	linkVector.reserve(1000);
}

void lightNode::setSelfAddress(lightNode* temp)
{
	/*
	* Parameter
	   lightNode* temp - Adrress of the current node is saved
	*/
	selfAddress = temp;             
}

lightNode* lightNode::returnSelfAddress()
{
	/*
	* return
		Address of the current node
	*/
	return selfAddress;             
}

void lightNode::addLPlink(vector<int> pathVec, int wavelengthVal, int bandwidthVal, int ID, lightNode* tempDesObj, string type, int LPidentifier)
{
	/*
	* Parameters
		vector<int> pathVec - Path of the lightpath
		int wavelengthVal - Wavelength of the ligthpath
		int bandwidthVal - Bandwidth of the lightpath
		int ID - id of the destination node of the ligthpath
		lightNode* tempDesObj - Address of the destination node of the ligthpath
		string type - primary or backup
	*/

	linkDetails temporLink;
	temporLink.destinationID = ID;                                 //Adding the id of the destination node of the lightpath
	temporLink.destAddress = tempDesObj;                           //Adding the address of the destination node of the lightpath

	temporLink.vecObj.LPidentifier = LPidentifier;                 //The id for the lightpath
	temporLink.vecObj.initialBandwidth = bandwidthVal;             //Initializing the bandwidth for the lightpath
	temporLink.vecObj.availableBandwidth = bandwidthVal;           //Initializing the bandwidth for the lightpath
	temporLink.vecObj.path = pathVec;                              //The lightpath passes these nodes of the network
	temporLink.vecObj.lightpathType = type;                        //Define whether the lightpath is a primary or a backup path
	temporLink.vecObj.wavelength = wavelengthVal;                  //Wavelength of the lightpath
	temporLink.vecObj.numOfLSPsInLightpath = 0;                    //Counts the number of LSPs in the lightpath
	temporLink.vecObj.numOfPrimaryLSPsInLightpath = 0;             //Counts the number of primary LSPs in the lightpath
	temporLink.wavelengthAndLSP.push_back(temporLink.vecObj);      //Push the above details to wavelengthAndLSP vector

	linkVector.push_back(temporLink);                              //The lightpath is created
	int temp = linkVector.size();                                 
	linkVector[temp - 1].wavelengthAndLSP.reserve(1000);
	numOfLPLinksPerNode++;                                         //The counter for lightpath is incremented
}

void lightNode::addWavelengthToLink(vector<int> pathVec, int destId, int wavelengthToBeAdded, int bandwidthVal, string type, int LPidentifier)
{
	/*
	* Parameters
		vector<int> pathVec - Path of the lightpath
		int destId - id of the destination node of the ligthpath
		int wavelengthToBeAdded - Wavelength of the ligthpath
		int bandwidthVal - Bandwidth of the lightpath
		string type - primary or backup
	*/
	for (size_t i = 0; i < linkVector.size(); i++)
	{
		if (linkVector[i].destinationID == destId)                          //If the destination node is already inserted to the linkVector of source node
		{
			linkVector[i].vecObj.LPidentifier = LPidentifier;               //The id for the lightpath
			linkVector[i].vecObj.initialBandwidth = bandwidthVal;           //Initialzing the bandwidth of the new lightpath
			linkVector[i].vecObj.availableBandwidth = bandwidthVal;         //Initialzing the bandwidth of the new lightpath
			linkVector[i].vecObj.path = pathVec;                            //The lightpath passes these nodes of the network
			linkVector[i].vecObj.lightpathType = type;                      //Define whether the lightpath is a primary or a backup path
			linkVector[i].vecObj.wavelength = wavelengthToBeAdded;          //Wavelength of the lightpath
			linkVector[i].vecObj.numOfLSPsInLightpath = 0;                  //Counts the number of LSPs in the lightpath
			linkVector[i].vecObj.numOfPrimaryLSPsInLightpath = 0;           //Counts the number of primary LSPs in the lightpath

			linkVector[i].wavelengthAndLSP.push_back(linkVector[i].vecObj); //A lightpath from a new wavlength is created
		}
	}
}

int lightNode::returnId()
{
	/*
	* return
		The id of the current source node
	*/
	return id;                
}

void lightNode::viewLPlinks()
{
	for (size_t i = 0; i < linkVector.size(); i++)
	{
		for (size_t j = 0; j < linkVector[i].wavelengthAndLSP.size(); j++)
		{
			int pathSize = linkVector[i].wavelengthAndLSP[j].path.size();
			cout << "LP type = " << linkVector[i].wavelengthAndLSP[j].lightpathType << endl;
			cout << "Initial BD : " << linkVector[i].wavelengthAndLSP[j].initialBandwidth << "\tAvailable BD : " << linkVector[i].wavelengthAndLSP[j].availableBandwidth << endl;
			cout << "Wavelength = " << linkVector[i].wavelengthAndLSP[j].wavelength << endl;
			cout << "IDentifier : " << linkVector[i].wavelengthAndLSP[j].LPidentifier << endl;
			for (size_t k = 0; k < (pathSize - 1); k++)
				cout << linkVector[i].wavelengthAndLSP[j].path[k] << " -> ";         //Print the path of the lightpath
			cout << linkVector[i].wavelengthAndLSP[j].path[pathSize - 1] << endl;
		}
	}
}

bool lightNode::serachLighpathNode(int nodeID)
{
	/*
	* Parameter
		int nodeID - id of the destination node of the ligthpath to be searched

	* return
		true or false
	*/
	for (size_t i = 0; i < linkVector.size(); i++)       //Check whether a node is made as a destination node for the current source node
		if (linkVector[i].destinationID == nodeID)
			return true;
	return false;
}

lightNode* lightNode::searchAddress(int temp)
{
	/*
	* Parameter
		int nodeID - id of the destination node of the ligthpath

	* return
		The address of the above destination node of the current source node
	*/
	for (size_t i = 0; i < linkVector.size(); i++)
		if (linkVector[i].destinationID == temp)
			return linkVector[i].destAddress;            
}


//////////////////////////////////

/* lightpathNetwork::lightpathNetwork(vector<waveLengthNetworks>& wv)
{
	waveLengthNetwork = wv;
	totalnumOfLighpaths = 0;                             //The count of total number of lightpaths at the begining is zero
	lighpaths.reserve(100);
} */

void lightpathNetwork::viewAllLighpaths()                //The function to print the all lighpaths
{
	for (size_t i = 0; i < lighpaths.size(); i++)
	{
		cout << "Lighpath node id - " << lighpaths[i].returnId() << endl;
		lighpaths[i].viewLPlinks();
		cout << endl << endl;
	}
}

void lightpathNetwork::setANewLighpath(vector<int> shortestPath, int wavelengthSt, string type, int LPidentifier, int bandwidthCap)
{
	/*
	* Parameters
		vector<int> shortestPath - The vector which consists the path of the lightpath
		int wavelengthSt - Wavelength of the lightpath
		string type - primary(pp) or backup(bp)
	*/
	vector<int> shortestpathVec = shortestPath;
	
	/* cout << "\nNew LP creating. "<<type<<" >>";
	for (int zz : shortestPath) cout << zz << "> "; 
	cout << endl;*/

	int vecSize = shortestPath.size();
	int bandwidth = bandwidthCap;

	int check1 = checkForAvaialableLPNode(shortestPath[0]);            //Check whether the source node, destination node of the lightpath is available in the
	int check2 = checkForAvaialableLPNode(shortestPath[vecSize - 1]);  //lightpath network(If it is available, the position in the lightpaths vector is returned.
	                                                                   //Else -1 is returned)

	if (check1 == -1)         //Source node is not available in the lightpath network
	{
		lightNode lpNodeS(shortestpathVec[0]);
		lightNode* addr1 = &lpNodeS;
		lpNodeS.setSelfAddress(addr1);

		if (check2 == -1)     //Destination node is not available in the lightpath network
		{
			lightNode lpNodeD(shortestPath[vecSize - 1]);
			lightNode* addr2 = &lpNodeD;
			lpNodeD.setSelfAddress(addr2);

			lpNodeD.addLPlink(shortestpathVec, wavelengthSt, bandwidth, lpNodeS.returnId(), addr1, type, LPidentifier);
			lpNodeS.addLPlink(shortestpathVec, wavelengthSt, bandwidth, lpNodeD.returnId(), addr2, type, LPidentifier);

			lighpaths.push_back(lpNodeS);
			lighpaths.push_back(lpNodeD);

			totalnumOfLighpaths++;
			if (type == "pp")
				primaryLightpathCount++;
		}

		else                   //Destination node is available in the lightpath network
		{
			lighpaths[check2].addLPlink(shortestpathVec, wavelengthSt, bandwidth, lpNodeS.returnId(), addr1, type, LPidentifier);
			//lighpaths[check2].addWavelengthToLink(shortestpathVec, lpNodeS.returnId(), lamda, bandwidth);
			lpNodeS.addLPlink(shortestpathVec, wavelengthSt, bandwidth, lighpaths[check2].returnId(), lighpaths[check2].returnSelfAddress(), type, LPidentifier);

			lighpaths.push_back(lpNodeS);

			totalnumOfLighpaths++;
			if (type == "pp")
				primaryLightpathCount++;
		}

	}

	else                       //Source node is available in the lightpath network
	{

		if (check2 == -1)      //Destination node is not available in the lightpath network
		{
			lightNode lpNodeD(shortestPath[vecSize - 1]);
			lightNode* addr2 = &lpNodeD;
			lpNodeD.setSelfAddress(addr2);

			lpNodeD.addLPlink(shortestpathVec, wavelengthSt, bandwidth, lighpaths[check1].returnId(), lighpaths[check1].returnSelfAddress(), type, LPidentifier);
			lighpaths[check1].addLPlink(shortestpathVec, wavelengthSt, bandwidth, lpNodeD.returnId(), addr2, type, LPidentifier);
			//lighpaths[check1].addWavelengthToLink(shortestpathVec, lpNodeD.returnId(), lamda, bandwidth);

			lighpaths.push_back(lpNodeD);

			totalnumOfLighpaths++;
			if (type == "pp")
				primaryLightpathCount++;
		}

		else                    //Destination node is available in the lightpath network
		{
			bool flag = false; //Since both the nodes are available in the lightpath network, it has to be verifed whether there is a link between those two nodes
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
				lighpaths[check1].addWavelengthToLink(shortestpathVec, lighpaths[check2].returnId(), wavelengthSt, bandwidth, type, LPidentifier);
				lighpaths[check2].addWavelengthToLink(shortestpathVec, lighpaths[check1].returnId(), wavelengthSt, bandwidth, type, LPidentifier);
			}
			else
			{
				lighpaths[check1].addLPlink(shortestpathVec, wavelengthSt, bandwidth, lighpaths[check2].returnId(), lighpaths[check2].returnSelfAddress(), type, LPidentifier);
				lighpaths[check2].addLPlink(shortestpathVec, wavelengthSt, bandwidth, lighpaths[check1].returnId(), lighpaths[check2].returnSelfAddress(), type, LPidentifier);
			}

			totalnumOfLighpaths++;
			if (type == "pp")
				primaryLightpathCount++;
		}
	}
}

int lightpathNetwork::checkForAvaialableLPNode(int val)
{
	/*
	* Parameter
		int val - the id of the node to searched

	* return
		If avaialbel, he position of the node in lightpaths vector. Else -1.
	*/
	for (size_t i = 0; i < lighpaths.size(); i++)
		if (lighpaths[i].returnId() == val)
			return i;

	return -1;
}

bool lightpathNetwork::checkForAvilableLightpath(int node1id, int node2id)
{
	/*
	* Parameters
		int node1id - The source/destination node of the lightpath
		int node2id - The destination/source node of the lightpath

	* return
		true or false
	*/
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

void lightpathNetwork::checkHeavilyLoadLP(vector<int> posVec, vector<vector<int>> partitionedPath, vector<int> wavelngthVec, bool protectionType, thresholds thresholdVals, bool hybrid)
{
	/*
	* Parameters
		vector<int> posVec - The path of the newly established primary LSP
		vector<int> wavelngthVec - Wavelengths relevant to the lightpaths
		bool protectionType - Relevant protection scheme(Based on the bandwidth/Based on the # of LSPs)
	*/
	for (size_t itr = 0; itr < (posVec.size() - 1); itr++)
	{
		int pos1 = posVec[itr];
		int pos2 = posVec[itr + 1];

		for (size_t i = 0; i < lighpaths[pos1].linkVector.size(); i++)
		{
			if (lighpaths[pos1].linkVector[i].destinationID == lighpaths[pos2].returnId())
				for (size_t j = 0; j < lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++)
				{
					vector<int> path2vec = partitionedPath[itr];
					reverse(path2vec.begin(), path2vec.end());
					if (lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == wavelngthVec[itr]
						&& lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].lightpathType == "pp"
						&& (lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].path == partitionedPath[itr] || lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].path == path2vec))
					{
						backupStruct tempbackupObj;
						tempbackupObj.intermediateNodes = lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].path;
						tempbackupObj.theWavelength = wavelngthVec[itr];
						tempbackupObj.pos1 = pos1;
						tempbackupObj.pos2 = pos2;
						tempbackupObj.i = i;
						tempbackupObj.j = j;
						tempbackupObj.protectionType = protectionType;
						tempbackupObj.thresholdVals = thresholdVals;
						tempbackupObj.hybrid = hybrid;
						establishBackupLightpath(tempbackupObj);
					}
				}
		}
	}
}

void lightpathNetwork::establishBackupLightpath(backupStruct tempbackupObj)
{
	int theWavelength = tempbackupObj.theWavelength;
	vector<int> intermediateNodes = tempbackupObj.intermediateNodes;
	int pos1 = tempbackupObj.pos1;
	int pos2 = tempbackupObj.pos2;
	int i = tempbackupObj.i;
	int j = tempbackupObj.j;
	bool protectionType = tempbackupObj.protectionType;
	bool hybrid = tempbackupObj.hybrid;
	thresholds thresholdVals = tempbackupObj.thresholdVals;

	if (!(lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].havingBackup))
	{
		float initBandwidth = static_cast<float>(lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].initialBandwidth);
		float usedPrimBand = static_cast<float>(lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth);

		float usedBandProportion = usedPrimBand / initBandwidth;

		float bandwidthThreshold = thresholdVals.bandwidthThreshold;
		int numLSPthreshold = thresholdVals.numLSPthreshold;

		if (!hybrid)
		{
			if (protectionType) //Bandwidth based protection
			{
				if (usedBandProportion > bandwidthThreshold)
				{
					vector<int> primaryLPpath = lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].path;

					forBackupLightpath backupObj;
					backupObj = createLightPathBackup(thresholdVals.numOfNodesOfTheNetwork, primaryLPpath, waveLengthNetwork, primaryLPpath.front(), primaryLPpath.back());
					bool isBackupLPpossible = backupObj.canCreatBLPath;

					if (isBackupLPpossible)
					{
						backUpLPavaialablecount++;
						lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].havingBackup = true;
						for (size_t ii = 0; ii < lighpaths[pos2].linkVector.size(); ii++)
						{
							if (lighpaths[pos2].linkVector[ii].destinationID == lighpaths[pos1].returnId())
							{
								for (size_t jj = 0; jj < lighpaths[pos2].linkVector[ii].wavelengthAndLSP.size(); jj++)
								{
									vector<int> path2vec = intermediateNodes;
									reverse(path2vec.begin(), path2vec.end());

									if (lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].wavelength == theWavelength
										&& lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].lightpathType == "pp"
										&& (lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].path == intermediateNodes || lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].path == path2vec))
									{
										forNoLSPaverage.push_back(lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].numOfPrimaryLSPsInLightpath);
										forLSPbandAverage.push_back(lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].primaryLSPbandwidth);

										int identifierForBLP = lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].LPidentifier;
										lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].havingBackup = true;
										//Call for lightpath establishment
										setANewLighpath(backupObj.wavelengthBLPath, backupObj.wavelengthBLPNo, "bp", identifierForBLP, thresholdVals.bandwidthCap);
									}
								}
							}
						}
					}
					else rejectedAvailbackupLPcount++;
				}
			}

			else if (!protectionType) //# of LSP based protection
			{
				if (lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath > numLSPthreshold)
				{
					vector<int> primaryLPpath = lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].path;

					forBackupLightpath backupObj;
					backupObj = createLightPathBackup(thresholdVals.numOfNodesOfTheNetwork, primaryLPpath, waveLengthNetwork, primaryLPpath.front(), primaryLPpath.back());
					bool isBackupLPpossible = backupObj.canCreatBLPath;


					if (isBackupLPpossible)
					{
						backUpLPavaialablecount++;
						lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].havingBackup = true;
						for (size_t ii = 0; ii < lighpaths[pos2].linkVector.size(); ii++)
						{
							if (lighpaths[pos2].linkVector[ii].destinationID == lighpaths[pos1].returnId())
							{
								for (size_t jj = 0; jj < lighpaths[pos2].linkVector[ii].wavelengthAndLSP.size(); jj++)
								{
									vector<int> path2vec = intermediateNodes;
									reverse(path2vec.begin(), path2vec.end());
									if (lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].wavelength == theWavelength
										&& lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].lightpathType == "pp"
										&& (lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].path == intermediateNodes || lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].path == path2vec))
									{
										forNoLSPaverage.push_back(lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].numOfPrimaryLSPsInLightpath);
										forLSPbandAverage.push_back(lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].primaryLSPbandwidth);

										int identifierForBLP = lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].LPidentifier;
										lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].havingBackup = true;
										//Call for lightpath establishment
										setANewLighpath(backupObj.wavelengthBLPath, backupObj.wavelengthBLPNo, "bp", identifierForBLP, thresholdVals.bandwidthCap);
									}
								}
							}
						}
					}
					else rejectedAvailbackupLPcount++;
				}
			}
		}
		else
		{
			if ((usedBandProportion > bandwidthThreshold) && (lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath > numLSPthreshold))
			{
				//cout << "\nHybrid Check\n";
				vector<int> primaryLPpath = lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].path;

				forBackupLightpath backupObj;
				backupObj = createLightPathBackup(thresholdVals.numOfNodesOfTheNetwork, primaryLPpath, waveLengthNetwork, primaryLPpath.front(), primaryLPpath.back());
				bool isBackupLPpossible = backupObj.canCreatBLPath;

				if (isBackupLPpossible)
				{
					backUpLPavaialablecount++;
					lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].havingBackup = true;
					for (size_t ii = 0; ii < lighpaths[pos2].linkVector.size(); ii++)
					{
						if (lighpaths[pos2].linkVector[ii].destinationID == lighpaths[pos1].returnId())
						{
							for (size_t jj = 0; jj < lighpaths[pos2].linkVector[ii].wavelengthAndLSP.size(); jj++)
							{
								vector<int> path2vec = intermediateNodes;
								reverse(path2vec.begin(), path2vec.end());

								if (lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].wavelength == theWavelength
									&& lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].lightpathType == "pp"
									&& (lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].path == intermediateNodes || lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].path == path2vec))
								{
									forNoLSPaverage.push_back(lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].numOfPrimaryLSPsInLightpath);
									forLSPbandAverage.push_back(lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].primaryLSPbandwidth);

									int identifierForBLP = lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].LPidentifier;
									lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].havingBackup = true;
									//Call for lightpath establishment
									setANewLighpath(backupObj.wavelengthBLPath, backupObj.wavelengthBLPNo, "bp", identifierForBLP, thresholdVals.bandwidthCap);
								}
							}
						}
					}
				}
				else rejectedAvailbackupLPcount++;
			}
		}
	}

}

void lightpathNetwork::setANewLSP(vector<int> shortestPathLSP, vector<int> wholePath, vector<int> wavelengthVec, lightpathNetwork &obj, string type, int identifier, bool protectionType, thresholds thresholdVals)
{
	/*
	* Parameters
		vector<int> shortestPathLSP - Path for the LSP
		vector<int> wavelengthVec - List of wavelengths
		lightpathNetwork &obj - The one object of lightpathNetwork class
		string type - Primary(pLSP) or Backup(bLSP)
		int identifier - The id value of the LSP
		bool protectionType - Relevant protection scheme(Based on the bandwidth/Based on the # of LSPs)
	*/
	LSP Object;
	//Object.makeLSP(shortestPathLSP, wholePath, wavelengthVec, obj, type, identifier, protectionType, thresholdVals);
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
				if(lighpaths[i].linkVector[j].wavelengthAndLSP[h].availableBandwidth >= bandwidth && 
					lighpaths[i].linkVector[j].wavelengthAndLSP[h].lightpathType == "pp")
				{
					int bndwdth = lighpaths[i].linkVector[j].wavelengthAndLSP[h].availableBandwidth;
					int sourceLP = lighpaths[i].returnId();
					int dstLP = lighpaths[i].linkVector[j].destinationID;

					/* cout<<"\n*\nSource = "<<sourceLP;
					cout<<"\nPath = ";
					for(int X:lighpaths[i].linkVector[j].wavelengthAndLSP[h].path)
					{
						cout<<X<<"-> ";
					}cout<<endl;
					cout<<"\nav. bnadwidth = "<<lighpaths[i].linkVector[j].wavelengthAndLSP[h].availableBandwidth <<" req.bandwidth = "<<bandwidth;
					cout<<"\n wave no : "<<lighpaths[i].linkVector[j].wavelengthAndLSP[h].wavelength; */

					arr[sourceLP][dstLP] = 40;
				}
			}
			
		}
	}

	return arr;
}

vector<float> lightpathNetwork::averageCalculation()
{
	float primLSPbandAverage = 0, primLSPnoAverage = 0;

	for (size_t i = 0; i < forLSPbandAverage.size(); i++)
	{
		primLSPbandAverage += forLSPbandAverage[i];
		primLSPnoAverage += forNoLSPaverage[i];
	}

	primLSPbandAverage = primLSPbandAverage / forLSPbandAverage.size();
	primLSPnoAverage = primLSPnoAverage / forNoLSPaverage.size();

	vector<float> allvalues;
	allvalues.push_back(primLSPbandAverage);
	allvalues.push_back(primLSPnoAverage);
	allvalues.push_back(backUpLPavaialablecount);
	allvalues.push_back(rejectedAvailbackupLPcount);
	allvalues.push_back(forLSPbandAverage.size());

	return allvalues;
}

void lightpathNetwork::heavilyLPprotectionCount(thresholds thresholdVals)
{
	int protectedCount = 0;
	int heavyLPcount = 0;
	int primLSPcountInHeavyLP = 0;
	int primLSPbandInHeavyLP = 0;
	int counterCount1 = 0;
	int primLSPbandInLP = 0;
	int counterCount2 = 0;
	for (size_t i = 0; i < lighpaths.size(); i++)
	{
		for (size_t j = 0; j < lighpaths[i].linkVector.size(); j++)
		{
			for (size_t k = 0; k < lighpaths[i].linkVector[j].wavelengthAndLSP.size(); k++)
			{
				if (lighpaths[i].linkVector[j].wavelengthAndLSP[k].lightpathType == "pp")
				{
					primLSPbandInLP += lighpaths[i].linkVector[j].wavelengthAndLSP[k].primaryLSPbandwidth;
					counterCount2++;
					primaryLightpathCount++;

					float primLSPband = static_cast<float>(lighpaths[i].linkVector[j].wavelengthAndLSP[k].primaryLSPbandwidth);
					float initLPband = static_cast<float>(lighpaths[i].linkVector[j].wavelengthAndLSP[k].initialBandwidth);
					int primLSPcount = lighpaths[i].linkVector[j].wavelengthAndLSP[k].numOfPrimaryLSPsInLightpath;
					if ((primLSPcount > thresholdVals.numLSPthreshold) && ((primLSPband / initLPband) > thresholdVals.bandwidthThreshold) && (thresholdVals.hybrid == true))
					{
						if (lighpaths[i].linkVector[j].wavelengthAndLSP[k].havingBackup)
							protectedCount++;
						heavyLPcount++;
						primLSPcountInHeavyLP += lighpaths[i].linkVector[j].wavelengthAndLSP[k].numOfPrimaryLSPsInLightpath;
						primLSPbandInHeavyLP += lighpaths[i].linkVector[j].wavelengthAndLSP[k].primaryLSPbandwidth;
						counterCount1++;
					}
					else if (((primLSPband / initLPband) > thresholdVals.bandwidthThreshold) && (thresholdVals.protectionType == true) && (thresholdVals.hybrid == false))
					{
						if (lighpaths[i].linkVector[j].wavelengthAndLSP[k].havingBackup)
							protectedCount++;
						heavyLPcount++;
						primLSPcountInHeavyLP += lighpaths[i].linkVector[j].wavelengthAndLSP[k].numOfPrimaryLSPsInLightpath;
						primLSPbandInHeavyLP += lighpaths[i].linkVector[j].wavelengthAndLSP[k].primaryLSPbandwidth;
						counterCount1++;
					}
					else if ((primLSPcount > thresholdVals.numLSPthreshold) && (thresholdVals.protectionType == false) && (thresholdVals.hybrid == false))
					{
						if (lighpaths[i].linkVector[j].wavelengthAndLSP[k].havingBackup)
							protectedCount++;
						heavyLPcount++;
						primLSPcountInHeavyLP += lighpaths[i].linkVector[j].wavelengthAndLSP[k].numOfPrimaryLSPsInLightpath;
						primLSPbandInHeavyLP += lighpaths[i].linkVector[j].wavelengthAndLSP[k].primaryLSPbandwidth;
						counterCount1++;
					}
					
				}
			}
		}
	}
	
	float forHLLPprobability = static_cast<float>(protectedCount) / static_cast<float>(heavyLPcount);
	if (heavyLPcount > 0)
		forAvrgHLLPprobability.push_back(forHLLPprobability);

	if (primaryLightpathCount > 0)
	{
		float forGettingHeavyLPpercentage = static_cast<float>(heavyLPcount) / static_cast<float>(primaryLightpathCount);
		forHeavyLPpercentage.push_back(forGettingHeavyLPpercentage);
		primaryLightpathCount = 0;
	}
	
	//cout << endl << protectedCount << " / " << heavyLPcount << " = " << forHLLPprobability << endl;

	float forPrimLSPcountAvrgHLLP1 = static_cast<float>(primLSPcountInHeavyLP) / static_cast<float>(counterCount1);
	if (counterCount1 > 0)
		forAvrgPrimLSPcountInHLLP.push_back(forPrimLSPcountAvrgHLLP1);

	float forPrimLSPbandAvrgHLLP1 = static_cast<float>(primLSPbandInHeavyLP) / static_cast<float>(counterCount1);
	if (counterCount1 > 0)
		forAvrgPrimLSPbandInHLLP.push_back(forPrimLSPbandAvrgHLLP1);

	float forPrimLSPbandAvrgLP1 = static_cast<float>(primLSPbandInLP) / static_cast<float>(counterCount2);
	if (counterCount2 > 0)
		forAvrgPrimLSPbandInAllLP.push_back(forPrimLSPbandAvrgLP1);
}

vector<float> lightpathNetwork::heavilyLPAndOtherAvgCalc()
{
	float HLLPprobability = accumulate(forAvrgHLLPprobability.begin(), forAvrgHLLPprobability.end(), 0.0) / static_cast<float>(forAvrgHLLPprobability.size());
	float primLSPcountAvrgHLLP = accumulate(forAvrgPrimLSPcountInHLLP.begin(), forAvrgPrimLSPcountInHLLP.end(), 0.0) / static_cast<float> (forAvrgPrimLSPcountInHLLP.size());
	float primLSPbandAvrgHLLP1 = accumulate(forAvrgPrimLSPbandInHLLP.begin(), forAvrgPrimLSPbandInHLLP.end(), 0.0) / static_cast<float> (forAvrgPrimLSPbandInHLLP.size());
	float primLSPbandAvrgInAllLP = accumulate(forAvrgPrimLSPbandInAllLP.begin(), forAvrgPrimLSPbandInAllLP.end(), 0.0) / static_cast<float> (forAvrgPrimLSPbandInAllLP.size());
	float percentageOfHLlightpaths = accumulate(forHeavyLPpercentage.begin(), forHeavyLPpercentage.end(), 0.0) / static_cast<float> (forHeavyLPpercentage.size());

	vector<float> calculatedVals{ HLLPprobability, primLSPcountAvrgHLLP, primLSPbandAvrgHLLP1, primLSPbandAvrgInAllLP, percentageOfHLlightpaths };

	return calculatedVals;
}

// Find given 2 paths are link disjoint or not
bool lightpathNetwork::isLinkDisjoint(vector<int> primaryPath, vector<int> testPath, int numOfNodes)
{
	//cout<<"primary = "<<primaryPath.size()<<endl;
	//cout<<"test = "<<testPath.size()<<endl;
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

vector<vector<int>> lightpathNetwork::getWaveNumbers(int source, int dst,int numOfNodes, int bandwidth, int pathSize)
//vector<int> lightpathNetwork::getWaveNumbers(int source, int dst, int bandwidth)
{
	vector<vector<int>> temp(2,vector<int>(1,-1));// {{-1},{-1}}
	int primaryWaveNo = -1;
	vector<int> primaryPath;
	
	//Find primary LP with minimum physical links
	for (int i = 0; i < lighpaths.size(); i++)
	{
		if(lighpaths[i].id == source)
		{
			for(int j = 0; j < lighpaths[i].linkVector.size(); j++)
			{
				if(lighpaths[i].linkVector[j].destinationID == dst)
				{
					for(int h = 0; h < lighpaths[i].linkVector[j].wavelengthAndLSP.size(); h++)
					{
						int band = 	lighpaths[i].linkVector[j].wavelengthAndLSP[h].availableBandwidth;
						string lpType = lighpaths[i].linkVector[j].wavelengthAndLSP[h].lightpathType;
						
						if(lighpaths[i].linkVector[j].wavelengthAndLSP[h].availableBandwidth >= bandwidth && 
							lighpaths[i].linkVector[j].wavelengthAndLSP[h].lightpathType == "pp" )
						{
							if(lighpaths[i].linkVector[j].wavelengthAndLSP[h].path.size() <= pathSize)
							{
								pathSize = lighpaths[i].linkVector[j].wavelengthAndLSP[h].path.size();
								primaryWaveNo = lighpaths[i].linkVector[j].wavelengthAndLSP[h].wavelength;
								primaryPath = lighpaths[i].linkVector[j].wavelengthAndLSP[h].path;
							}
						}
					}
				}
				
			}
		}
	}

	temp[0][0] = primaryWaveNo;

	if(temp[0][0] == -1) return temp;
	temp[0].insert(temp[0].end(), primaryPath.begin(), primaryPath.end());

	//Find LP for backup path
	for (int i = 0; i < lighpaths.size(); i++)
	{
		if(lighpaths[i].id == source)
		{
			for(int j = 0; j < lighpaths[i].linkVector.size(); j++)
			{
				if(lighpaths[i].linkVector[j].destinationID == dst)
				{
					for(int h = 0; h < lighpaths[i].linkVector[j].wavelengthAndLSP.size(); h++)
					{						
						if(lighpaths[i].linkVector[j].wavelengthAndLSP[h].availableBandwidth >= bandwidth && 
							lighpaths[i].linkVector[j].wavelengthAndLSP[h].lightpathType == "pp" )
						{
							if(isLinkDisjoint(primaryPath,lighpaths[i].linkVector[j].wavelengthAndLSP[h].path,numOfNodes))
							{
								temp[1][0] = lighpaths[i].linkVector[j].wavelengthAndLSP[h].wavelength;
								temp[1].insert(temp[1].end(), lighpaths[i].linkVector[j].wavelengthAndLSP[h].path.begin(), lighpaths[i].linkVector[j].wavelengthAndLSP[h].path.end());
								return temp;
							}
						}
					}
				}
				
			}
		}
	}

	return temp;
}

vector<int> lightpathNetwork::getBWaveNumber(int source, int dst,int numOfNodes, int bandwidth,vector<int> primaryPath)
{
	vector<int> temp;
	for (int i = 0; i < lighpaths.size(); i++)
	{
		if(lighpaths[i].id == source)
		{
			for(int j = 0; j < lighpaths[i].linkVector.size(); j++)
			{
				if(lighpaths[i].linkVector[j].destinationID == dst)
				{
					for(int h = 0; h < lighpaths[i].linkVector[j].wavelengthAndLSP.size(); h++)
					{						
						if(lighpaths[i].linkVector[j].wavelengthAndLSP[h].availableBandwidth >= bandwidth && 
							lighpaths[i].linkVector[j].wavelengthAndLSP[h].lightpathType == "pp" )
						{
							if(isLinkDisjoint(primaryPath,lighpaths[i].linkVector[j].wavelengthAndLSP[h].path,numOfNodes))
							{
								temp.push_back(lighpaths[i].linkVector[j].wavelengthAndLSP[h].wavelength);
								temp.insert(temp.end(), lighpaths[i].linkVector[j].wavelengthAndLSP[h].path.begin(), lighpaths[i].linkVector[j].wavelengthAndLSP[h].path.end());
								return temp;
							}
						}
					}
				}
				
			}
		}
	}

	return vector<int> {-1};
}


//Generate adjacency metrix to find shortest path for backup LSP
/* vector<vector<int>> lightpathNetwork::lpBAdjacencyMetrix(vector<int> primaryPath, int numOfNodes)
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

	
} */


map<int, vector<vector<int>>> lightpathNetwork::mapFromsource(int src, int numOfNodes, int bandwidth, int numOfWaves)
{
	map<int, vector<vector<int>>> temp;

	for(int i = numOfWaves; i < 2*numOfWaves; i++)
	{
		temp[i] = vector<vector<int>> (numOfNodes,vector<int>(1,0));
	}

	for (int i = 0; i < lighpaths.size(); i++)
	{
		if(lighpaths[i].id == src)
		{
			for(int j = 0; j < lighpaths[i].linkVector.size(); j++)
			{
				int dst = lighpaths[i].linkVector[j].destinationID;

				for(int h = 0; h < lighpaths[i].linkVector[j].wavelengthAndLSP.size(); h++)
				{	
					if(lighpaths[i].linkVector[j].wavelengthAndLSP[h].availableBandwidth >= bandwidth && 
						lighpaths[i].linkVector[j].wavelengthAndLSP[h].lightpathType == "pp" )
					{
						int wave = 	lighpaths[i].linkVector[j].wavelengthAndLSP[h].wavelength;
						vector<int> path = lighpaths[i].linkVector[j].wavelengthAndLSP[h].path;
						
						if(path[0] != src)
						{
							reverse(path.begin(),path.end());
						}
						
						if(temp[numOfWaves + wave][dst][0] == 0)
						{
							temp[numOfWaves + wave][dst][0] = 1;

							for(int k :path)
							{
								temp[numOfWaves + wave][dst].push_back(k);
							}
						}

						else
						{
							if(path.size() < temp[numOfWaves + wave][dst].size()-1)
							{
								temp[numOfWaves + wave][dst].clear();
								temp[numOfWaves + wave][dst].push_back(1);

								for(int k :path)
								{
									temp[numOfWaves + wave][dst].push_back(k);
								}
							}
						}
					}
				}
				
			}
		}
	}

	return temp;
}


map<int, map<int, vector<vector<int>>>> lightpathNetwork::mapFromLpGraph(int numofwaves, int numofnodes, int bandwidth)
{
   if(numofwaves <= 0 || numofnodes <= 0)
   {
      cerr<< "err:lightpathsettingup.cpp mapfromlpgraph parameters are negetavie.";
   }
	map<int,vector<vector<int>>> maptoreturnmap;

	for(int i = 0; i < numofnodes; i++)
	{
		maptoreturnmap[i] = vector<vector<int>> (numofnodes,vector<int> (1,0));
	}

   map<int,map<int,vector<vector<int>>>> maptoreturn;

   for(int i = 0; i < numofwaves; i++)
   {
      maptoreturn[i+numofwaves] = maptoreturnmap;
   }

   for(int i = 0; i < lighpaths.size(); i++)
   {
	   int source = lighpaths[i].id;

	   for(int j = 0; j < lighpaths[i].linkVector.size(); j++)
	   {
		   int destination = lighpaths[i].linkVector[j].destinationID;
		   
		   for(int k = 0; k < lighpaths[i].linkVector[j].wavelengthAndLSP.size(); k++)
		   {
			   if(lighpaths[i].linkVector[j].wavelengthAndLSP[k].availableBandwidth >= bandwidth && lighpaths[i].linkVector[j].wavelengthAndLSP[k].lightpathType == "pp" )
			   {
				   int wave = lighpaths[i].linkVector[j].wavelengthAndLSP[k].wavelength;
					vector<int> path = lighpaths[i].linkVector[j].wavelengthAndLSP[k].path;

					if(path[0] != source)
					{
						reverse(path.begin(),path.end());
					}

					if(maptoreturn[wave+numofwaves][source][destination][0] == 0)
					{
						maptoreturn[wave+numofwaves][source][destination][0] = 1;

						maptoreturn[wave+numofwaves][source][destination].insert(maptoreturn[wave+numofwaves][source][destination].end(),path.begin(), path.end());
					}
					else
					{
						if(path.size() < maptoreturn[wave+numofwaves][source][destination].size()-1)
						{
							maptoreturn[wave+numofwaves][source][destination].clear();
							maptoreturn[wave+numofwaves][source][destination].push_back(1);

							maptoreturn[wave+numofwaves][source][destination].insert(maptoreturn[wave+numofwaves][source][destination].end(),path.begin(), path.end());
						}
					}
			   }
		   }
	   }
   }

   return maptoreturn;

   /* for(vector<int> i:maptoreturn[1+numofwaves][2])
   {
      for(int j:i)
      {
         cout<<j<<" ";
      }cout<<endl;
   }
   cout<< "\nnext...\n";
   for(vector<int> i:maptoreturn[39+numofwaves][13])
   {
      for(int j:i)
      {
         cout<<j<<" ";
      }cout<<endl;
   } */

   

}

/***************************** REMOVE LIGHTPAH ******************************/


void lightNode::checkDestinationAndWavelength(int destination, int wavelength) {

}

int lightNode::verifyDestinationNode(int node2)
{
	for (int i = 0; i < linkVector.size(); i++)
	{
		if (linkVector[i].destinationID == node2)
		{
			return i;
		}
	}
}

void lightNode::deleteLpLink(long int LPidentifier, int source, int destination, int wavelength, vector<waveLengthNetworks>& waveLengthNetwork, int LinkID, string typeOfLP) {
	//for (int i = 0; i < linkVector.size(); i++) {
		//int sourceID =
		//if (linkVector[i].destinationID == lighpaths[destination].id) {
	bool linkIsFound = false;
	vector<int> lightpathPath;
			for (int j = 0; j < linkVector[LinkID].wavelengthAndLSP.size(); j++) {
				//cout << "identifier : " << LPidentifier << endl;
				//cout << "LPidentifier : " << linkVector[LinkID].wavelengthAndLSP[j].LPidentifier<<endl;
				if (linkVector[LinkID].wavelengthAndLSP[j].LPidentifier == LPidentifier ) {
					/*cout << "available bandwidht: " << linkVector[LinkID].wavelengthAndLSP[j].availableBandwidth << endl;
					cout << "initialBandwidth" << linkVector[LinkID].wavelengthAndLSP[j].initialBandwidth<<endl;
					cout << " typeOfLP : " << typeOfLP << endl;*/
					if (linkVector[LinkID].wavelengthAndLSP[j].availableBandwidth == linkVector[LinkID].wavelengthAndLSP[j].initialBandwidth) {
						//cout << " lightpath Type : " << linkVector[LinkID].wavelengthAndLSP[j].lightpathType << "     typeOfLP : " << typeOfLP << endl;
						if (linkVector[LinkID].wavelengthAndLSP[j].lightpathType == typeOfLP) {							
							lightpathPath = linkVector[LinkID].wavelengthAndLSP[j].path;
							linkIsFound = true;
							if (typeOfLP == "bp") {
								long int tempID = linkVector[LinkID].wavelengthAndLSP[j].LPidentifier;
								for (int m = 0; m < linkVector.size(); m++) {
									for (int n = 0; n < linkVector[m].wavelengthAndLSP.size(); n++) {
										if (linkVector[m].wavelengthAndLSP[n].LPidentifier == tempID && linkVector[m].wavelengthAndLSP[n].havingBackup == true && linkVector[m].wavelengthAndLSP[n].lightpathType == "pp") {
											linkVector[m].wavelengthAndLSP[n].havingBackup == false;
										}
									}
								}

								linkVector[LinkID].wavelengthAndLSP.erase(linkVector[LinkID].wavelengthAndLSP.begin() + j);
							}else {
								if (linkVector[LinkID].wavelengthAndLSP[j].havingBackup) {
									long int tempID = linkVector[LinkID].wavelengthAndLSP[j].LPidentifier;
									for (int m = 0; m < linkVector.size(); m++) {
										for (int n = 0; n < linkVector[m].wavelengthAndLSP.size(); n++) {
											if (linkVector[m].wavelengthAndLSP[n].LPidentifier == tempID && linkVector[m].wavelengthAndLSP[n].lightpathType == "bp") {
												linkVector[m].wavelengthAndLSP.erase(linkVector[m].wavelengthAndLSP.begin() + n);
											}
										}
									}
								}
								linkVector[LinkID].wavelengthAndLSP.erase(linkVector[LinkID].wavelengthAndLSP.begin() + j);
							}															
						}
					}
					else {
						cout << "\n******the lightpath is in use******" << endl;
					}
				}		
			}

			if (linkIsFound)
				for (int z = 0; z < lightpathPath.size() - 1; z++) {
					waveLengthNetwork[wavelength].addLInk(lightpathPath[z], lightpathPath[z + 1]);
				}
}

void lightNode::newDeleteLPlink(int LinkID, int j, string typeOfLP, vector<waveLengthNetworks>& waveLengthNetwork)
{
	if (linkVector[LinkID].wavelengthAndLSP[j].availableBandwidth == linkVector[LinkID].wavelengthAndLSP[j].initialBandwidth) 
	{
		int wave = linkVector[LinkID].wavelengthAndLSP[j].wavelength;
		if ((linkVector[LinkID].wavelengthAndLSP[j].lightpathType == typeOfLP) && (typeOfLP == "bp"))
		{
			long int tempID = linkVector[LinkID].wavelengthAndLSP[j].LPidentifier;
			for (int m = 0; m < linkVector.size(); m++)
			{
				for (int n = 0; n < linkVector[m].wavelengthAndLSP.size(); n++)
				{
					if ((linkVector[m].wavelengthAndLSP[n].LPidentifier == tempID) && (linkVector[m].wavelengthAndLSP[n].havingBackup == true) && (linkVector[m].wavelengthAndLSP[n].lightpathType == "pp"))
					{
						linkVector[m].wavelengthAndLSP[n].havingBackup == false;
					}
				}
			}
			//cout << "\nDElete LP link\n";
			vector<int> LPpath = linkVector[LinkID].wavelengthAndLSP[j].path;
			linkVector[LinkID].wavelengthAndLSP.erase(linkVector[LinkID].wavelengthAndLSP.begin() + j);
			for (int i = 0; i < LPpath.size() - 1; i++)
				waveLengthNetwork[wave].addLInk(LPpath[i], LPpath[i + 1]);
		}


		/*if (linkVector[LinkID].wavelengthAndLSP[j].lightpathType == typeOfLP)
		{
			lightpathPath = linkVector[LinkID].wavelengthAndLSP[j].path;
			linkIsFound = true;
			if (typeOfLP == "bp")
			{
				long int tempID = linkVector[LinkID].wavelengthAndLSP[j].LPidentifier;
				for (int m = 0; m < linkVector.size(); m++)
				{
					for (int n = 0; n < linkVector[m].wavelengthAndLSP.size(); n++)
					{
						if (linkVector[m].wavelengthAndLSP[n].LPidentifier == tempID && linkVector[m].wavelengthAndLSP[n].havingBackup == true && linkVector[m].wavelengthAndLSP[n].lightpathType == "pp")
						{
							linkVector[m].wavelengthAndLSP[n].havingBackup == false;
						}
					}
				}

				linkVector[LinkID].wavelengthAndLSP.erase(linkVector[LinkID].wavelengthAndLSP.begin() + j);
			}
			else
			{
				if (linkVector[LinkID].wavelengthAndLSP[j].havingBackup)
				{
					long int tempID = linkVector[LinkID].wavelengthAndLSP[j].LPidentifier;
					for (int m = 0; m < linkVector.size(); m++) {
						for (int n = 0; n < linkVector[m].wavelengthAndLSP.size(); n++) {
							if (linkVector[m].wavelengthAndLSP[n].LPidentifier == tempID && linkVector[m].wavelengthAndLSP[n].lightpathType == "bp") {
								linkVector[m].wavelengthAndLSP.erase(linkVector[m].wavelengthAndLSP.begin() + n);
							}
						}
					}
				}
				linkVector[LinkID].wavelengthAndLSP.erase(linkVector[LinkID].wavelengthAndLSP.begin() + j);
			}
		}*/
	}
}

void lightpathNetwork::getTotalNumberOfLightpaths() {
	cout << totalnumOfLighpaths << endl;
}


/*

void lightpathNetwork::releaseEshtablishedLighpath( int source, int destination, int wavelength) {
	//to delete from source node's LP-link's vector
	int positionOne = checkForAvaialableLPNode(source);
	if (positionOne != -1) {
		int temp = lighpaths[positionOne].verifyDestinationNode(destination);

		if (temp != -1) {
			//call the functions
			lighpaths[positionOne];
			lighpaths[positionOne].deleteLpLink(wavelength);
			lighpaths.erase(lighpaths.begin() + positionOne);

		}
		else {
			cout << "------error!!!!!!-----------" << endl;
		}
	}

	//to delete from destination node's LP-link's vector
	int positionTwo = checkForAvaialableLPNode(destination);
	if (positionTwo != -1) {
		int temp = lighpaths[positionTwo].verifyDestinationNode(source);
		if (temp != -1) {
			//call the functions
			lighpaths[positionTwo].deleteLpLink(wavelength);
			lighpaths.erase(lighpaths.begin() + positionTwo);
			totalnumOfLighpaths = totalnumOfLighpaths - 1;
		}
		else {
			cout << "------error!!!!!!-----------" << endl;
		}
	}

	//to delete from lightpaths vector
}*/

void lightpathNetwork::releaseEshtablishedLighpath(int source, int destination ,long int LightPathIdentifier, int wavelenght, string typeOfLP) {
	//for (int i = 0; i < lighpaths.size(); i++) {

		//cout << "lighpaths[i].id : " << lighpaths[i].id << endl;
		//if (lighpaths[i].id == ligthpaths[source].id) {
		
	for (int j = 0; j < lighpaths[source].linkVector.size(); j++) {
		if (lighpaths[source].linkVector[j].destinationID == lighpaths[destination].id){
			for (int k = 0; k < lighpaths[source].linkVector[j].wavelengthAndLSP.size(); k++) {

				if (lighpaths[source].linkVector[j].wavelengthAndLSP[k].LPidentifier == LightPathIdentifier) {

					lighpaths[source].deleteLpLink(LightPathIdentifier, lighpaths[source].id, lighpaths[destination].id, wavelenght, waveLengthNetwork, j, typeOfLP);
					//lighpaths.erase(lighpaths.begin() + i);
					break;
				}
			}
		}
	}
		//}
		//if (lighpaths[i].id == destination) {
			for (int j = 0; j < lighpaths[destination].linkVector.size(); j++) {
				if (lighpaths[destination].linkVector[j].destinationID == lighpaths[source].id) {
					for (int k = 0; k < lighpaths[destination].linkVector[j].wavelengthAndLSP.size(); k++) {
						if (lighpaths[destination].linkVector[j].wavelengthAndLSP[k].LPidentifier == LightPathIdentifier) {

							lighpaths[destination].deleteLpLink(LightPathIdentifier, lighpaths[destination].id, lighpaths[source].id, wavelenght, waveLengthNetwork, j, typeOfLP);
							//lighpaths.erase(lighpaths.begin() + i);
							totalnumOfLighpaths = totalnumOfLighpaths - 1;
							if (typeOfLP == "pp")
								primaryLightpathCount--;
							break;
						}
					}
				}
			}
		//}

	//}

}

void lightpathNetwork::newReleaseEstablishedLightPath(int source, int destination, long int LightPathIdentifier, int wavelenght, string typeOfLP, vector<waveLengthNetworks>& waveLengthNetwork)
{
	for (int j = 0; j < lighpaths[source].linkVector.size(); j++)
	{
		if (lighpaths[source].linkVector[j].destinationID == lighpaths[destination].id)
		{
			for (int k = 0; k < lighpaths[source].linkVector[j].wavelengthAndLSP.size(); k++)
			{
				if ((lighpaths[source].linkVector[j].wavelengthAndLSP[k].LPidentifier == LightPathIdentifier)&& (lighpaths[source].linkVector[j].wavelengthAndLSP[k].lightpathType == typeOfLP))
				{
					lighpaths[source].newDeleteLPlink(j, k, "bp", waveLengthNetwork);
					goto rLP1;
				}
			}
		}
	}

	rLP1:

	for (int j = 0; j < lighpaths[destination].linkVector.size(); j++)
	{
		if (lighpaths[destination].linkVector[j].destinationID == lighpaths[source].id)
		{
			for (int k = 0; k < lighpaths[destination].linkVector[j].wavelengthAndLSP.size(); k++)
			{
				if ((lighpaths[destination].linkVector[j].wavelengthAndLSP[k].LPidentifier == LightPathIdentifier) && (lighpaths[destination].linkVector[j].wavelengthAndLSP[k].lightpathType == typeOfLP))
				{
					lighpaths[destination].newDeleteLPlink(j, k, "bp", waveLengthNetwork);

					totalnumOfLighpaths = totalnumOfLighpaths - 1;
					if (typeOfLP == "pp")
						primaryLightpathCount--;

					goto rLP2;
				}

			}
		}
	}

	rLP2:
	return;
}