#include "lightpathSettingup.h"
#include "graph.h"
#include "LSP.h"

class lightNode;
class lightpathNetwork;


void LSP::viewLSPsInALightpath(lightpathNetwork &obj, int node1id, int node2id)
{

	for (size_t i = 0; i < obj.lighpaths.size(); i++)
	{
		for (size_t j = 0; j < obj.lighpaths[i].linkVector.size(); j++)
		{
			for (size_t k = 0; k < obj.lighpaths[i].linkVector[j].wavelengthAndLSP.size(); k++)
			{
				if (obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].numOfLSPsInLightpath > 0)
				{
					cout << "The LSPs start from node" << obj.lighpaths[i].returnId() << endl;
					for (size_t m = 0; k < obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec.size(); k++)
						if (obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].prev == NULL)
						{
							cout << obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].id;
							traverseLSP(obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].nextLSP);
							cout << endl;
						}
				}
			}
		}
	}

	/*int pos1 = obj.checkForAvaialableLPNode(node1id);
	int pos2 = obj.checkForAvaialableLPNode(node2id);

	size_t theSize1 = obj.lighpaths[pos1].linkVector.size();

	for (size_t i = 0; i < theSize1; i++)
	{
		if (obj.lighpaths[pos1].linkVector[i].destinationID == node2id)
		{
			size_t theSize2 = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size();

			if (theSize2 > 0)
			{
				cout << "The LSPs of the lightpath between node" << node1id << " and " << node2id << ":\n";
				for (size_t j = 0; j < theSize2; j++)
				{
					obj.lighpaths[pos1].linkVector[i].vecLSP[j].printNode();
					cout << "->";
				}
				cout << endl;
			}
		}
	}

	for (size_t i = 0; i < obj.lighpaths.size(); i++)
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
					}*/
					//cout << "Lightpath between " << node1id << " to " << node2id << " doesn't exist\n";
}

void LSP::traverseLSP(LSP* nextNode)
{

	LSP temp = *nextNode;
	while (nextNode->nextLSP != NULL)
	{
		cout << "->" << temp.id;
		nextNode = nextNode->nextLSP;
	}

	cout << "->" << temp.id << endl;
	
}

/*void LSP::viewAllLSPs()
{

}*/