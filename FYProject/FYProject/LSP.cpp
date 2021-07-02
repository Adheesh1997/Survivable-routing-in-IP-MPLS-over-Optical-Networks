#include "graph.h"
#include "lightpathSettingup.h"
#include "LSP.h"

class lightNode;
class lightpathNetwork;

void LSP::establishANewLSP(vector<int> shortestPathLSP, int wavelength, lightpathNetwork &obj)
{
	
	if (shortestPathLSP.size() == 2)
	{
		LSPnode tempNode;

		tempNode.prev = NULL;
		int positionOfNextLPnode = obj.checkForAvaialableLPNode(shortestPathLSP[1]);
		tempNode.next = obj.lighpaths[positionOfNextLPnode].returnSelfAddress();
		LSPnodeVec.push_back(tempNode);  //First node of the LSP

		tempNode.next = NULL;
		int positionOfPrevLPnode = obj.checkForAvaialableLPNode(shortestPathLSP[0]);
		tempNode.prev = obj.lighpaths[positionOfPrevLPnode].returnSelfAddress();
		LSPnodeVec.push_back(tempNode);  //Second(Last) node of the LSP
		
	}

	else if (shortestPathLSP.size() > 2)
	{
		int pathSize = shortestPathLSP.size();
		int numOfIntermediate = pathSize - 2;
		

		LSPnode tempNode1;
		
		tempNode1.prev = NULL;
		int positionOfNextLPnode = obj.checkForAvaialableLPNode(shortestPathLSP[1]);
		tempNode1.next = obj.lighpaths[positionOfNextLPnode].returnSelfAddress();
		LSPnodeVec.push_back(tempNode1);  //First node of the LSP


		LSPnode tempNode2;
		for (size_t i = 1; i <= numOfIntermediate; i++)
		{
			int positionOfPrevLPnode = obj.checkForAvaialableLPNode(shortestPathLSP[i - 1]);
			tempNode2.prev = obj.lighpaths[positionOfPrevLPnode].returnSelfAddress();

			positionOfNextLPnode = obj.checkForAvaialableLPNode(shortestPathLSP[i + 1]);
			tempNode2.next = obj.lighpaths[positionOfNextLPnode].returnSelfAddress();

			LSPnodeVec.push_back(tempNode1);
		}

		int positionOfPrevLPnode = obj.checkForAvaialableLPNode(shortestPathLSP[numOfIntermediate]);
		tempNode2.prev = obj.lighpaths[positionOfPrevLPnode].returnSelfAddress();

		tempNode2.next = NULL;
		LSPnodeVec.push_back(tempNode2);  //Final node of LSP
	}



}

/*void LSP::viewLSPsInALightpath(lightpathNetwork &obj)
{
	int node1id, node2id;

	for (int i = 0; i < obj.lighpaths.size(); i++)
	{
		if (obj.lighpaths[i].returnId() == node1id)
		{
			bool check = obj.lighpaths[i].serachLighpathNode(node2id);
			/*if (check)
				cout << "Lightpath between " << node1id << " to " << node2id << " exists\n";
			else
				cout << "Lightpath between " << node1id << " to " << node2id << " doesn't exist\n";*/
			
		/*}
		if (obj.lighpaths[i].returnId() == node2id)
		{
			bool check = obj.lighpaths[i].serachLighpathNode(node1id);
			/*if (check)
				cout << "Lightpath between " << node1id << " to " << node2id << " exists\n";
			else
				cout << "Lightpath between " << node1id << " to " << node2id << " doesn't exist\n";*/
			
		/*}
	}
	//cout << "Lightpath between " << node1id << " to " << node2id << " doesn't exist\n";

}*/

/*void LSP::viewAllLSPs()
{

}*/
