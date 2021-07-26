#include "lightpathSettingup.h"
#include "graph.h"
#include "LSP.h"

class lightNode;                                //Classes declaration
class lightpathNetwork;



void LSP::makeLSP(vector<int> shortestPathLSP, vector<int> LSPwavelengthVec, lightpathNetwork& obj, string type, int identifier, bool protectionType)
{
	/*
	* Parameters
		vector<int> shortestPathLSP - The path of the LSP
		vector<int> LSPwavelengthVec - List of wavelengths
		lightpathNetwork& obj - The one object of lightpathNetwork class
		string type - Primary(pLSP) or Backup(bLSP)
		int identifier - The id value of the LSP
		bool protectionType - Relevant protection scheme(Based on the bandwidth/Based on the # of LSPs)
	*/
	int intLSPwavelength;
	int LSPbandwidth = 10;
	
	vector<int> v1 = shortestPathLSP;           //Shortest path for a LSP


	if (shortestPathLSP.size() == 2)                              //If the LSP path has only 2 nodes
	{
		LSP tempObject;
		LSP tempObject2;

		int pos1 = obj.checkForAvaialableLPNode(v1[0]);
		int pos2 = obj.checkForAvaialableLPNode(v1[1]);

		LSP* ptr1 = NULL;
		LSP* ptr2 = NULL;

		size_t i1 = 0, i2 = 0, j1 = 0, j2 = 0, k1 = 0, k2 = 0;

		bool exit1 = false, exit2 = false;

		for (size_t i = 0; i < obj.lighpaths[pos1].linkVector.size(); i++)
		{
			if (obj.lighpaths[pos1].linkVector[i].destinationID == v1[1])
			{
				tempObject.bandwidthOfLSP = LSPbandwidth;
				tempObject.LSPpath = shortestPathLSP;
				tempObject.identifier = identifier;
				tempObject.id = obj.lighpaths[pos1].returnId();
				tempObject.prev = NULL;
				tempObject.next = obj.lighpaths[pos1].linkVector[i].destAddress;

				for (size_t j = 0; j < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++)

					if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[0])
					{
						int remainingBandwidth = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth;
						remainingBandwidth = remainingBandwidth - LSPbandwidth;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth = remainingBandwidth;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath++;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject);
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.reserve(1000);
						i1 = i;
						j1 = j;
						k1 = (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.size() - 1);
						if (type == "pLSP")
						{
							obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath++;
							obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth += LSPbandwidth;
							obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k1].LSPtype = "pLSP";
						}
						else if (type == "bLSP")
							obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k1].LSPtype = "bLSP";
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k1].prevLSP = NULL;
						ptr1 = &obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k1];

						break;
					}

			}
		}

		for (size_t i = 0; i < obj.lighpaths[pos2].linkVector.size(); i++)
		{

			if (obj.lighpaths[pos2].linkVector[i].destinationID == v1[0])
			{
				tempObject2.bandwidthOfLSP = LSPbandwidth;
				tempObject2.LSPpath = shortestPathLSP;
				tempObject2.identifier = identifier;
				tempObject2.id = obj.lighpaths[pos2].returnId();
				tempObject2.prev = obj.lighpaths[pos2].linkVector[i].destAddress;
				tempObject2.next = NULL;


				for (size_t j = 0; j < obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP.size(); j++)

					if (obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[0])
					{
						int remainingBandwidth = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].availableBandwidth;
						remainingBandwidth = remainingBandwidth - LSPbandwidth;
						obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].availableBandwidth = remainingBandwidth;
						obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath++;
						obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject2);
						obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.reserve(1000);
						i2 = i;
						j2 = j;
						k2 = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.size() - 1;
						if (type == "pLSP")
						{
							obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath++;
							obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth += LSPbandwidth;
							obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k2].LSPtype = "pLSP";
						}
						else if (type == "bLSP")
							obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k2].LSPtype = "bLSP";
						obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k2].nextLSP = NULL;
						ptr2 = &obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k2];

						break;
					}
			}
		}


		obj.lighpaths[pos1].linkVector[i1].wavelengthAndLSP[j1].LSPvec[k1].nextLSP = ptr2;
		obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j2].LSPvec[k2].prevLSP = ptr1;

		if (type == "pLSP")
			obj.checkHeavilyLoadLP(shortestPathLSP, LSPwavelengthVec, protectionType);
	}

	else if (shortestPathLSP.size() > 2)             //If the LSP path has more than 2 nodes
	{
		int pathSize = shortestPathLSP.size();
		int numOfIntermediate = pathSize - 2;

		LSP* ptr1 = NULL;
		LSP* ptr2 = NULL;

		size_t i1 = 0, j1 = 0, k1 = 0;

		LSP tempObject;

		int pos1 = obj.checkForAvaialableLPNode(v1[0]);
		for (size_t i = 0; i < obj.lighpaths[pos1].linkVector.size(); i++)
		{
			if (obj.lighpaths[pos1].linkVector[i].destinationID == v1[1])
			{
				tempObject.bandwidthOfLSP = LSPbandwidth;
				tempObject.LSPpath = shortestPathLSP;
				tempObject.identifier = identifier;
				tempObject.id = obj.lighpaths[pos1].returnId();
				tempObject.prev = NULL;
				tempObject.next = obj.lighpaths[pos1].linkVector[i].destAddress;
				
				for (size_t j = 0; j < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++)

					if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[0])
					{
						int remainingBandwidth = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth;
						remainingBandwidth = remainingBandwidth - LSPbandwidth;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth = remainingBandwidth;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath++;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject);
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.reserve(1000);
						i1 = i;
						j1 = j;
						k1 = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.size() - 1;
						if (type == "pLSP")
						{
							obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath++;
							obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth += LSPbandwidth;
							obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k1].LSPtype = "pLSP";
						}
						else if (type == "bLSP")
							obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k1].LSPtype = "bLSP";
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k1].prevLSP = NULL;
						ptr1 = &obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k1];

					}

			}
		}

		size_t i2 = 0, i3 = 0, j2 = 0, j3 = 0, k2 = 0, k3 = 0, i4 = 0, j4 = 0, k4 = 0;
		int pos2 = 0, pos3 = 0;
		LSP tempObject2;
		LSP* ptr3 = NULL;
		LSP* ptr4 = NULL;

		for (size_t itr = 1; itr <= numOfIntermediate; itr++)
		{

			pos2 = obj.checkForAvaialableLPNode(v1[itr]);
			pos3 = obj.checkForAvaialableLPNode(v1[itr + 1]);

			size_t i = 0;
			bool switch1 = false, switch2 = false;
			for (; i < obj.lighpaths[pos2].linkVector.size(); i++)
			{
				if (obj.lighpaths[pos2].linkVector[i].destinationID == v1[itr - 1])
				{
					tempObject.bandwidthOfLSP = LSPbandwidth;
					tempObject.LSPpath = shortestPathLSP;
					tempObject.identifier = identifier;
					tempObject.id = obj.lighpaths[pos2].returnId();
					tempObject.prev = obj.lighpaths[pos1].returnSelfAddress();
					tempObject.next = obj.lighpaths[pos3].returnSelfAddress();
					i2 = i;
					switch1 = true;

				}
				if (obj.lighpaths[pos2].linkVector[i].destinationID == v1[itr + 1])
				{
					tempObject2.bandwidthOfLSP = LSPbandwidth;
					tempObject2.LSPpath = shortestPathLSP;
					tempObject2.identifier = identifier;
					tempObject2.id = obj.lighpaths[pos2].returnId();
					tempObject2.prev = obj.lighpaths[pos1].returnSelfAddress();
					tempObject2.next = obj.lighpaths[pos3].returnSelfAddress();
					tempObject2.prevLSP = ptr1;
					i3 = i;
					switch2 = true;

				}

				if (switch1 && switch2)
					break;
			}

			size_t waveSize2 = obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP.size();
			
			for (size_t j = 0; j < waveSize2; j++)
			{
				if (obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[itr])
				{
					int remainingBandwidth = obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].availableBandwidth;
					remainingBandwidth = remainingBandwidth - LSPbandwidth;
					obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].availableBandwidth = remainingBandwidth;
					obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].numOfLSPsInLightpath++;
					obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].LSPvec.push_back(tempObject);
					obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].LSPvec.reserve(1000);
					j2 = j;
					k2 = obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].LSPvec.size() - 1;
					if (type == "pLSP")
					{
						obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath++;
						obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].primaryLSPbandwidth += LSPbandwidth;
						obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].LSPvec[k2].LSPtype = "pLSP";
					}
					else if (type == "bLSP")
						obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].LSPvec[k2].LSPtype = "bLSP";
					ptr2 = &obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].LSPvec[k2];

					break;
				}
			}

			size_t waveSize3 = obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP.size();
			
			for (size_t j = 0; j < waveSize3; j++)
			{
				if (obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[itr])
				{
					int remainingBandwidth = obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].availableBandwidth;
					remainingBandwidth = remainingBandwidth - LSPbandwidth;
					obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].availableBandwidth = remainingBandwidth;
					obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].numOfLSPsInLightpath++;
					obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].LSPvec.push_back(tempObject2);
					obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].LSPvec.reserve(1000);
					j3 = j;
					k3 = (obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j3].LSPvec.size() - 1);
					if (type == "pLSP")
					{
						obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath++;
						obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].primaryLSPbandwidth += LSPbandwidth;
						obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].LSPvec[k3].LSPtype = "pLSP";
					}
					else if (type == "bLSP")
						obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].LSPvec[k3].LSPtype = "bLSP";
					ptr3 = &obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j3].LSPvec[k3];


					break;
				}
			}

			obj.lighpaths[pos1].linkVector[i1].wavelengthAndLSP[j1].LSPvec[k1].nextLSP = ptr2;
			obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j3].LSPvec[k3].prevLSP = ptr1;

			ptr1 = ptr3;

			i1 = i2;
			j1 = j2;
			k1 = k2;
			pos1 = pos2;

		}

		pos1 = obj.checkForAvaialableLPNode(v1[pathSize - 1]);

		for (size_t i = 0; i < obj.lighpaths[pos1].linkVector.size(); i++)
		{
			if (obj.lighpaths[pos1].linkVector[i].destinationID == v1[pathSize - 2])
			{
				//Availablebandwidth = totalBandwidth - LSPbandwidth
				tempObject.bandwidthOfLSP = LSPbandwidth;
				tempObject.LSPpath = shortestPathLSP;
				tempObject.identifier = identifier;
				tempObject.id = obj.lighpaths[pos1].returnId();
				tempObject.prev = obj.lighpaths[pos1].linkVector[i].destAddress;
				tempObject.next = NULL;
				tempObject.nextLSP = NULL;

				for (size_t j = 0; j < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++)

					if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[numOfIntermediate])
					{
						int remainingBandwidth = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth;
						remainingBandwidth = remainingBandwidth - LSPbandwidth;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth = remainingBandwidth;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath++;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject);
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.reserve(1000);
						i4 = i;
						j4 = j;
						k4 = (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.size() - 1);
						if (type == "pLSP")
						{
							obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath++;
							obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth += LSPbandwidth;
							obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k4].LSPtype = "pLSP";
						}
						else if (type == "bLSP")
							obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k4].LSPtype = "bLSP";
						ptr4 = &obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k4];
						
						break;
					}

				obj.lighpaths[pos1].linkVector[i4].wavelengthAndLSP[j4].LSPvec[k4].prevLSP = ptr3;
				obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j2].LSPvec[k2].nextLSP = ptr4;


			}
		}
		if (type == "pLSP")
			obj.checkHeavilyLoadLP(shortestPathLSP, LSPwavelengthVec, protectionType);
	}

}

void LSP::viewLSPsInALightpath(lightpathNetwork& obj)
{
	/*
	* Parameter
		lightpathNetwork& obj - The one object of lightpathNetwork class
	*/
	for (size_t i = 0; i < obj.lighpaths.size(); i++)
	{
		for (size_t j = 0; j < obj.lighpaths[i].linkVector.size(); j++)
		{
			for (size_t k = 0; k < obj.lighpaths[i].linkVector[j].wavelengthAndLSP.size(); k++)
			{
				if (obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].numOfLSPsInLightpath > 0)
				{
					for (size_t m = 0; m < obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec.size(); m++)
					{
						if (obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].prev == NULL && obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].next != NULL)
						{
							cout << "Wavelength = " << obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].wavelength << endl;
							cout << "The LSPs start from light node " << obj.lighpaths[i].returnId() << endl;

							cout << obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].id;
							traversefLSP(obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].nextLSP);
							cout << endl;
						}

						if (obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].prev != NULL && obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].next == NULL)
						{
							cout << "Wavelength = " << obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].wavelength << endl;
							cout << "The LSPs start from light node " << obj.lighpaths[i].returnId() << endl;

							cout << obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].id;
							traversebLSP(obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].prevLSP);
							cout << endl;
						}
					}
				}
			}
		}
	}

}

void LSP::traversefLSP(LSP* nextNode)    //Traverse the LSP from forward
{
	/*
	* Parameter
		LSP* nextNode - Next LSP node
	*/
	LSP temp;
	while (nextNode->nextLSP != NULL)
	{
		temp = *nextNode;
		cout << "->" << temp.id;
		nextNode = nextNode->nextLSP;
		
	}
	temp = *nextNode;
	cout << "->" << temp.id << endl;

}

void LSP::traversebLSP(LSP* prevNode)     //Traverse  the LSP from backward
{
	/*
	* Parameter
		LSP* nextNode - Next LSP node
	*/
	LSP temp;
	while (prevNode->prevLSP != NULL)
	{
		temp = *prevNode;
		cout << "->" << temp.id;
		prevNode = prevNode->prevLSP;
		
	}
	temp = *prevNode;
	cout << "->" << temp.id << endl;
}
