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
	linkVector.reserve(100);
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

void lightNode::addLPlink(vector<int> pathVec, int wavelengthVal, int bandwidthVal, int ID, lightNode* tempDesObj, string type)
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
	linkVector[temp - 1].wavelengthAndLSP.reserve(100);
	numOfLPLinksPerNode++;                                         //The counter for lightpath is incremented
}

void lightNode::addWavelengthToLink(vector<int> pathVec, int destId, int wavelengthToBeAdded, int bandwidthVal, string type)
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
			cout << "Wavelength = " << linkVector[i].wavelengthAndLSP[j].wavelength << endl;
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

void lightpathNetwork::setANewLighpath(vector<int> shortestPath, int wavelengthSt, string type)
{
	/*
	* Parameters
		vector<int> shortestPath - The vector which consists the path of the lightpath
		int wavelengthSt - Wavelength of the lightpath
		string type - primary(pp) or backup(bp)
	*/
	vector<int> shortestpathVec = shortestPath;
	

	int vecSize = shortestPath.size();
	int bandwidth = 50;

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

			lpNodeD.addLPlink(shortestpathVec, wavelengthSt, bandwidth, lpNodeS.returnId(), addr1, type);
			lpNodeS.addLPlink(shortestpathVec, wavelengthSt, bandwidth, lpNodeD.returnId(), addr2, type);

			lighpaths.push_back(lpNodeS);
			lighpaths.push_back(lpNodeD);

			totalnumOfLighpaths++;
		}

		else                   //Destination node is available in the lightpath network
		{
			lighpaths[check2].addLPlink(shortestpathVec, wavelengthSt, bandwidth, lpNodeS.returnId(), addr1, type);
			//lighpaths[check2].addWavelengthToLink(shortestpathVec, lpNodeS.returnId(), lamda, bandwidth);
			lpNodeS.addLPlink(shortestpathVec, wavelengthSt, bandwidth, lighpaths[check2].returnId(), lighpaths[check2].returnSelfAddress(), type);

			lighpaths.push_back(lpNodeS);

			totalnumOfLighpaths++;
		}

	}

	else                       //Source node is available in the lightpath network
	{

		if (check2 == -1)      //Destination node is not available in the lightpath network
		{
			lightNode lpNodeD(shortestPath[vecSize - 1]);
			lightNode* addr2 = &lpNodeD;
			lpNodeD.setSelfAddress(addr2);

			lpNodeD.addLPlink(shortestpathVec, wavelengthSt, bandwidth, lighpaths[check1].returnId(), lighpaths[check1].returnSelfAddress(), type);
			lighpaths[check1].addLPlink(shortestpathVec, wavelengthSt, bandwidth, lpNodeD.returnId(), addr2, type);
			//lighpaths[check1].addWavelengthToLink(shortestpathVec, lpNodeD.returnId(), lamda, bandwidth);

			lighpaths.push_back(lpNodeD);

			totalnumOfLighpaths++;
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
				lighpaths[check1].addWavelengthToLink(shortestpathVec, lighpaths[check2].returnId(), wavelengthSt, bandwidth, type);
				lighpaths[check2].addWavelengthToLink(shortestpathVec, lighpaths[check1].returnId(), wavelengthSt, bandwidth, type);
			}
			else
			{
				lighpaths[check1].addLPlink(shortestpathVec, wavelengthSt, bandwidth, lighpaths[check2].returnId(), lighpaths[check2].returnSelfAddress(), type);
				lighpaths[check2].addLPlink(shortestpathVec, wavelengthSt, bandwidth, lighpaths[check1].returnId(), lighpaths[check2].returnSelfAddress(), type);
			}

			totalnumOfLighpaths++;
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

void lightpathNetwork::checkHeavilyLoadLP(vector<int> posVec, vector<int> wavelngthVec, bool protectionType)
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
					if (lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == wavelngthVec[itr] && lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].lightpathType == "pp")
					{
						if (!(lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].havingBackup))
						{
							int initBandwidth = lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].initialBandwidth;
							int usedPrimBand = lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth;

							float usedBandProportion = usedPrimBand / initBandwidth;

							float bandwidthThreshold = 0.75;
							int numLSPthreshold = 1;

							if (protectionType) //Bandwidth based protection
							{
								if (usedBandProportion >= bandwidthThreshold)
								{
									vector<int> primaryLPpath = lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].path;

									bool isBackupLPpossible = false;
									//isBackupLPpossible = backupLPpathRequester(primaryLPpath);
									if (isBackupLPpossible)
									{
										lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].havingBackup = true;
										for (size_t ii = 0; ii < lighpaths[pos2].linkVector.size(); ii++)
										{
											if (lighpaths[pos2].linkVector[ii].destinationID == lighpaths[pos1].returnId())
											{
												for (size_t jj = 0; jj < lighpaths[pos2].linkVector[ii].wavelengthAndLSP.size(); jj++)
												{
													if (lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].wavelength == wavelngthVec[itr] && lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].lightpathType == "pp")
													{
														lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].havingBackup = true;
														//Call for lightpath establishment
														//setANewLighpath(backupPath, givenWavelength, bp)
													}
												}
											}
										}
									}
								}
							}

							else if (!protectionType) //# of LSP based protection
							{
								if (lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath > numLSPthreshold)
								{
									vector<int> primaryLPpath = lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].path;

									bool isBackupLPpossible = false;
									//isBackupLPpossible = backupLPpathRequester(primaryLPpath);
									if (isBackupLPpossible)
									{
										lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].havingBackup = true;
										for (size_t ii = 0; ii < lighpaths[pos2].linkVector.size(); ii++)
										{
											if (lighpaths[pos2].linkVector[ii].destinationID == lighpaths[pos1].returnId())
											{
												for (size_t jj = 0; jj < lighpaths[pos2].linkVector[ii].wavelengthAndLSP.size(); jj++)
												{
													if (lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].wavelength == wavelngthVec[itr] && lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].lightpathType == "pp")
													{
														lighpaths[pos2].linkVector[ii].wavelengthAndLSP[jj].havingBackup = true;
														//Call for lightpath establishment
														//setANewLighpath(backupPath, givenWavelength, bp)
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
		}
	}
}

void lightpathNetwork::setANewLSP(vector<int> shortestPathLSP, vector<int> wavelengthVec, lightpathNetwork &obj, string type, int identifier, bool protectionType)
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
	Object.makeLSP(shortestPathLSP, wavelengthVec, obj, type, identifier, protectionType);
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

vector<int> lightpathNetwork::getWaveNumbers(int source, int dst,int numOfNodes, int bandwidth, int pathSize)
//vector<int> lightpathNetwork::getWaveNumbers(int source, int dst, int bandwidth)
{
	vector<int> temp = {-1,-1};
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

	temp[0] = primaryWaveNo;

	if(temp[0] == -1) return temp;

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
								temp[1] = lighpaths[i].linkVector[j].wavelengthAndLSP[h].wavelength;
								return temp;
							}
						}
					}
				}
				
			}
		}
	}

	return vector<int>{-1,-1};
}

int lightpathNetwork::getBWaveNumber(int source, int dst,int numOfNodes, int bandwidth,vector<int> primaryPath)
{
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
								return lighpaths[i].linkVector[j].wavelengthAndLSP[h].wavelength;
							}
						}
					}
				}
				
			}
		}
	}

	return -1;
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


map<int, vector<vector<int>>> lightpathNetwork::mapFromsource(int src, int numOfNodes, files &myfile)
{
	map<int, vector<vector<int>>> temp;

	for(int i = 40; i < 80; i++)
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
					int wave = 	lighpaths[i].linkVector[j].wavelengthAndLSP[h].wavelength;
					vector<int> path = lighpaths[i].linkVector[j].wavelengthAndLSP[h].path;

					if(path[0] != src)
					{
						reverse(path.begin(),path.end());
					}
					
					if(temp[40 + wave][dst][0] == 0)
					{
						temp[40 + wave][dst][0] = 1;

						for(int k :path)
						{
							temp[40 + wave][dst].push_back(k);
						}
					}

					else
					{
						if(path.size() < temp[40 + wave][dst].size()-1)
						{
							temp[40 + wave][dst].clear();
							temp[40 + wave][dst].push_back(1);

							for(int k :path)
							{
								temp[40 + wave][dst].push_back(k);
							}
						}
					}
				}
				
			}
		}
	}

	return temp;
}


map<int, vector<vector<int>>> lightpathNetwork::mapFromdst(int dst, int numOfNodes)
{
	map<int, vector<vector<int>>> temp;

	for(int i = 40; i < 80; i++)
	{
		temp[i] = vector<vector<int>> (numOfNodes,vector<int>(1,0));
	}

	for (int i = 0; i < lighpaths.size(); i++)
	{
		int src = lighpaths[i].id;
		for(int j = 0; j < lighpaths[i].linkVector.size(); j++)
		{
			if( dst == lighpaths[i].linkVector[j].destinationID)
			{
				for(int h = 0; h < lighpaths[i].linkVector[j].wavelengthAndLSP.size(); h++)
				{	
					int wave = 	lighpaths[i].linkVector[j].wavelengthAndLSP[h].wavelength;
					vector<int> path = lighpaths[i].linkVector[j].wavelengthAndLSP[h].path;

					if(path[path.size()-1] != dst)
					{
						reverse(path.begin(),path.end());
					}

					if(temp[40 + wave][src][0] == 0)
					{
						temp[40 + wave][src][0] = 1;

						for(int k :path)
						{
							temp[40 + wave][src].push_back(k);
						}
					}

					else
					{
						if(path.size()< temp[40 + wave][src].size()-1)
						{
							temp[40 + wave][src].clear();
							temp[40 + wave][src].push_back(1);

							for(int k :path)
							{
								temp[40 + wave][src].push_back(k);
							}
						}
					}
				}
			}			
		}
	}

	return temp;
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

void lightNode::deleteLpLink(int wl) {
	for (int i = 0; i < linkVector.size(); i++) {
		for (int j = 0; j < linkVector[i].wavelengthAndLSP.size(); j++) {
			if (linkVector[i].wavelengthAndLSP[j].wavelength == wl) {
				if (linkVector[i].wavelengthAndLSP[j].availableBandwidth == linkVector[i].wavelengthAndLSP[j].initialBandwidth) {
					//lighpaths.erase(lighpaths.begin() + i);
					linkVector.erase(linkVector.begin() + i);
					cout << "********		SUCESSFULY DELETED ********" << endl;
				}
				else {
					cout << "the lightpath is in use" << endl;
				}
			}
			else {
				cout << " ---ERROR!--- can not find the wavelenght" << endl;
			}
		}
	}
}


void lightpathNetwork::getTotalNumberOfLightpaths() {
	cout << totalnumOfLighpaths << endl;
}




void lightpathNetwork::releaseEshtablishedLighpath(int source, int destination, int wavelength) {
	//to delete from source node's LP-link's vector
	int positionOne = checkForAvaialableLPNode(source);
	if (positionOne != -1) {
		int temp = lighpaths[positionOne].verifyDestinationNode(destination);

		if (temp != -1) {
			//call the functions
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
}