#include "graph.h"
#include "lightpathSettingup.h"
#include "LSP.h"

/* void LSP::establishANewLSP(vector<int> shortestPathLSP, int wavelength, int LSPbandwidth)
{
	lightpathNetwork obj;
	bandwidthOfLSP = LSPbandwidth;

	if (shortestPathLSP.size() == 2)
	{
		LSPnode tempNode;


		tempNode.prev = NULL;
		int positionOfNextLPnode = obj.checkForAvaialableLPNode(shortestPathLSP[1]);
		//tempNode.next = obj.adrressReturn(positionOfNextLPnode);
		LSPnodeVec.push_back(tempNode);  //First node of the LSP

		tempNode.next = NULL;
		int positionOfPrevLPnode = obj.checkForAvaialableLPNode(shortestPathLSP[0]);
		//tempNode.prev = obj.adrressReturn(positionOfPrevLPnode);
		LSPnodeVec.push_back(tempNode);  //Second(Last) node of the LSP
		
	}

	else if (shortestPathLSP.size() > 2)
	{
		int pathSize = shortestPathLSP.size();
		int numOfIntermediate = pathSize - 2;
		

		LSPnode tempNode1;
		
		tempNode1.prev = NULL;
		int positionOfNextLPnode = obj.checkForAvaialableLPNode(shortestPathLSP[1]);
		//tempNode1.next = obj.adrressReturn(positionOfNextLPnode);
		LSPnodeVec.push_back(tempNode1);  //First node of the LSP


		LSPnode tempNode2;
		for (int i = 1; i <= numOfIntermediate; i++)
		{
			int positionOfPrevLPnode = obj.checkForAvaialableLPNode(shortestPathLSP[i - 1]);
			//tempNode2.prev = obj.adrressReturn(positionOfPrevLPnode);

			positionOfNextLPnode = obj.checkForAvaialableLPNode(shortestPathLSP[i + 1]);
			//tempNode2.next = obj.adrressReturn(positionOfNextLPnode);

			LSPnodeVec.push_back(tempNode1);
		}

		int positionOfPrevLPnode = obj.checkForAvaialableLPNode(shortestPathLSP[numOfIntermediate]);
		//tempNode2.prev = obj.adrressReturn(positionOfPrevLPnode);

		tempNode2.next = NULL;
		LSPnodeVec.push_back(tempNode2);  //Final node of LSP
	}
} */