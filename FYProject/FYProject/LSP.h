#pragma once
#ifndef LSP_H
#define LSP_H

#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include "lightpathSettingup.h"
#include "graph.h"

using namespace std;

class lightpathNetwork;
class lightNode;

class LSP
{
private:
	friend class lightNode;
	friend class lightpathNetwork;

	/*struct LSPnode
	{
		int id;
		lightNode* prev;
		lightNode* next;
	};
	vector<LSPnode> LSPnodeVec;*/

	int bandwidthOfLSP;
	vector<int> LSPpath;
	int id;
	lightNode* prev = NULL;
	lightNode* next = NULL;


public:
	//void establishANewLSP(vector<int> shortestPathLSP, int wavelength, lightpathNetwork &obj);
	void viewLSPsInALightpath(lightpathNetwork &obj, int node1id, int node2id);
	//void printNode();
	//void viewAllLSPs();
	//bool checkForAvailableLSP();

};


#endif