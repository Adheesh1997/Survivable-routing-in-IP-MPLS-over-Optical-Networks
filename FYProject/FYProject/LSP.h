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

class lightpathNetwork;
class lightNode;

class LSP
{
private:
	friend class lightNode;
	friend class lightpathNetwork;
	friend struct lighpathWavelength;

	int bandwidthOfLSP;        //Bandwidth for the LSP
	vector<int> LSPpath;       //Path of the LSP
	string LSPtype;
	//int identifier;          //Identifier
	int id;                    //Id of the LSP node
	lightNode* prev = NULL;    //Pointer towards the previous light node
	lightNode* next = NULL;    //Pointer towards the next light node
	LSP* nextLSP = NULL;       //Pointer towards the next LSP node
	LSP* prevLSP = NULL;       //Pointer towards the previous LSP node


public:
	void makeLSP(vector<int> shortestPathLSP, vector<int> LSPwavelengthVec, lightpathNetwork& obj, string type);  //Establish a LSP
	void viewLSPsInALightpath(lightpathNetwork& obj);                                           //View the LSPs within a lightpath
	//void viewAllLSPs();
	//bool checkForAvailableLSP();
	void traversefLSP(LSP* nextNode);       //Traverse a LSP from front the end
	void traversebLSP(LSP* prevNode);       //Traverse a LSP from back to the front

};


#endif