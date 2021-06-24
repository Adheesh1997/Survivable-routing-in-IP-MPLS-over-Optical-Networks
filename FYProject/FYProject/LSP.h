#pragma once
#include "graph.h"
#include "lightpathSettingup.h"



class LSP
{
	private:
		struct LSPnode
		{
			lightNode* prev;
			lightNode* next;
		};
		vector<LSPnode> LSPnodeVec;
		int bandwidthOfLSP;
	public:
		void establishANewLSP(vector<int> shortestPathLSP, int wavelength, int LSPbandwidth, lightpathNetwork &obj);
		//void viewAllLSPs();
		//bool checkForAvailableLSP();

};