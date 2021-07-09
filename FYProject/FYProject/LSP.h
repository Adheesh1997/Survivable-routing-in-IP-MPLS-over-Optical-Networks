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

	int bandwidthOfLSP;
	vector<int> LSPpath;
	int id;
	lightNode* prev = NULL;
	lightNode* next = NULL;
	LSP* nextLSP = NULL;
	LSP* prevLSP = NULL;


public:
	void makeLSP(vector<int> shortestPathLSP, string wavelengthLSPstr, lightpathNetwork& obj);
	void viewLSPsInALightpath(lightpathNetwork& obj);
	//void viewAllLSPs();
	//bool checkForAvailableLSP();
	void traversefLSP(LSP* nextNode);
	void traversebLSP(LSP* prevNode);

};


#endif