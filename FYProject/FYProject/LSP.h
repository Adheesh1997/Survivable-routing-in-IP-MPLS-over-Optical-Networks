//#pragma once
#ifndef LSP_H
#define LSP_H

#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include <sstream>
#include "graph.h"
#include "lightpathSettingup.h"

using namespace std;

class lightpathNetwork;        //Classes declaration
class lightNode;
struct thresholds;

class LSP
{
private:
	friend class lightNode;   //Creating friend classes
	friend class lightpathNetwork;
	friend struct lighpathWavelength;

	int bandwidthOfLSP;        //Bandwidth for the LSP
	vector<int> LSPpath;       //Path of the LSP
	string LSPtype;            //Primary LSP (pLSP) or backup LSP (bLSP)
	int identifier;            //Identifier value for the LSP
	int id;                    //Id of the LSP node
	lightNode* prev = NULL;    //Pointer towards the previous light node
	lightNode* next = NULL;    //Pointer towards the next light node
	LSP* nextLSP = NULL;       //Pointer towards the next LSP node
	LSP* prevLSP = NULL;       //Pointer towards the previous LSP node
	//vector<waveLengthNetworks*> subWaveNetworksVAR;
	vector<waveLengthNetworks> *subWaveNetworksVAR;

public:
	
	LSP() {}
	//LSP(vector<waveLengthNetworks> *subWaveNetworks) :subWaveNetworksVAR(subWaveNetworks) {}
	LSP(vector<waveLengthNetworks> *subWaveNetworks) :subWaveNetworksVAR(subWaveNetworks) {}
	void makeLSP(int bandwidth, vector<int> shortestPathLSP, vector<int> wholePath, vector<int> LSPwavelengthVec, lightpathNetwork& obj, string type, int identifier, bool protectionType, thresholds thresholdVals, bool hybrid, vector<vector<int>> partitionedPath = { {},{} });  //Establish a LSP
	void viewLSPsInALightpath(lightpathNetwork& obj);    //View the LSPs within a lightpath
	void traversefLSP(LSP* nextNode);                    //Traverse a LSP from front the end
	void traversebLSP(LSP* prevNode);                    //Traverse a LSP from back to the front
	void releaseLSP(vector<int> path, vector<int> LSPwavelengthVec, lightpathNetwork& obj, int identifier, thresholds, bool, string, int &, bool hybrid);
	void printVect(vector<int> v1)
	{
		for (int i = 0; i < v1.size(); i++)
			cout << v1[i] << "  ";
		cout << endl;
	}
	void newReleaseLSP(vector<int> path, vector<int> LSPwavelengthVec, lightpathNetwork& obj, int identifier, thresholds THval, bool protectionType, string typeOfLSP, bool hybrid);
	void newCheckForHeavyBeforeRelease(lightpathNetwork& obj, bool hybrid, bool protectionType, int pos1, int pos2, int i, int j, int long tempIdOfLP, thresholds THval);
	~LSP() {}
};


#endif