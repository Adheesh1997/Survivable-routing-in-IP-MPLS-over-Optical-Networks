#pragma once
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


public:
	

	void makeLSP(int bandwidth, vector<int> shortestPathLSP, vector<int> wholePath, vector<int> LSPwavelengthVec, lightpathNetwork& obj, string type, int identifier, bool protectionType, thresholds thresholdVals);  //Establish a LSP
	void viewLSPsInALightpath(lightpathNetwork& obj);    //View the LSPs within a lightpath
	void traversefLSP(LSP* nextNode);                    //Traverse a LSP from front the end
	void traversebLSP(LSP* prevNode);                    //Traverse a LSP from back to the front
	void releaseLSP(vector<int> path, vector<int> LSPwavelengthVec, lightpathNetwork& obj, int identifier, thresholds, bool, string, int &);

};


#endif