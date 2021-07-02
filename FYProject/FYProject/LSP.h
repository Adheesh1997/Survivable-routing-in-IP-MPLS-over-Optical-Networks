#pragma once
#ifndef LSP_H
#define LSP_H

#include <iostream>
#include <vector>
#include <string>
#include <vector>
#include "graph.h"

using namespace std;

class lightpathNetwork;
class lightNode;

class LSP
{
private:
	friend class lightNode;
	friend class lightpathNetwork;

	struct LSPnode
	{
		lightNode* prev;
		lightNode* next;
	};
	vector<LSPnode> LSPnodeVec;
	int bandwidthOfLSP;


public:
	void establishANewLSP(vector<int> shortestPathLSP, int wavelength, lightpathNetwork &obj);
	void viewLSPsInALightpath(lightpathNetwork &obj);
	//void viewAllLSPs();
	//bool checkForAvailableLSP();

};


#endif
