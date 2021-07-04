#pragma once
#ifndef LSP_H
#define LSP_H

#include <iostream>
#include <vector>
#include <string>
#include <vector>
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
	LSP* nextLSP = NULL;
	LSP* prevLSP = NULL;


public:
	void viewLSPsInALightpath(lightpathNetwork &obj, int node1id, int node2id);
	//void viewAllLSPs();
	//bool checkForAvailableLSP();
	void traverseLSP(LSP* nextNode);

};


#endif