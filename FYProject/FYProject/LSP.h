#pragma once
#include <vector>
#include "graph.h"
#include "lightpathSettingup.h"


class LSP: public lightNode, public lightpathNetwork
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
		void establishANewLSP(vector<int> shortestPathLSP, int wavelength, lightpathNetwork obj);
		//void viewLSPsInALightpath();
		//void viewAllLSPs();
		//bool checkForAvailableLSP();

};