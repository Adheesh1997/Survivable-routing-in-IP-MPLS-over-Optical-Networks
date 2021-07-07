#include "lightpathSettingup.h"
#include "graph.h"
#include "LSP.h"

class lightNode;
class lightpathNetwork;

void LSP::makeLSP(vector<int> shortestPathLSP, string wavelengthLSPstr, lightpathNetwork& obj)
{
	int LSPbandwidth = 10;
	stringstream change(wavelengthLSPstr);
	int intLSPwavelength;
	vector<int> v1 = shortestPathLSP;

	change >> intLSPwavelength;

	for (size_t seek = 0; seek < (shortestPathLSP.size() - 1); seek++)
	{
		bool checker = obj.checkForAvilableLightpath(shortestPathLSP[seek], shortestPathLSP[seek + 1]);
		if (!checker)
			cin.get();
	}

	/*cout << "*****************************************\n";
	for (size_t s = 0; s < shortestPathLSP.size(); s++)
		cout << shortestPathLSP[s] << " ";
	cout << endl;
	cout << "*****************************************\n";*/

	if (shortestPathLSP.size() == 2)
	{
		LSP tempObject;

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
				tempObject.id = obj.lighpaths[pos1].returnId();
				tempObject.prev = NULL;
				tempObject.next = obj.lighpaths[pos1].linkVector[i].destAddress;
				/*cout << "pos1 = " << pos1 << endl;
				cout << "i = " << i << endl;
				cout << "id = " << obj.lighpaths[pos1].returnId() << endl;
				cout << "obj.lighpaths[pos1].linkVector[i].destinationID = " << obj.lighpaths[pos1].linkVector[i].destinationID << endl;*/


				for (size_t j = 0; j < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++)

					if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == intLSPwavelength)
					{
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath++;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject);
						i1 = i;
						j1 = j;
						k1 = (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.size() - 1);
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k1].prevLSP = NULL;
						ptr1 = &obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k1];

						/*cout << "i1 = " << i1 << endl;
						cout << "j1 = " << j1 << endl;
						cout << "k1 = " << k1 << endl;
						cout << "ptr1 = " << ptr1 << endl << endl;*/

						break;
					}

			}
		}

		for (size_t i = 0; i < obj.lighpaths[pos2].linkVector.size(); i++)
		{

			if (obj.lighpaths[pos2].linkVector[i].destinationID == v1[0])
			{
				tempObject.bandwidthOfLSP = LSPbandwidth;
				tempObject.LSPpath = shortestPathLSP;
				tempObject.id = obj.lighpaths[pos2].returnId();
				tempObject.prev = obj.lighpaths[pos2].linkVector[i].destAddress;
				tempObject.next = NULL;
				/*cout << "pos2 = " << pos2 << endl;
				cout << "i = " << i << endl;
				cout << "id = " << obj.lighpaths[pos2].returnId() << endl;
				cout << "obj.lighpaths[pos2].linkVector[i].destinationID = " << obj.lighpaths[pos2].linkVector[i].destinationID << endl;*/


				for (size_t j = 0; j < obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP.size(); j++)

					if (obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].wavelength == intLSPwavelength)
					{
						obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath++;
						obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject);
						i2 = i;
						j2 = j;
						k2 = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.size() - 1;
						obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k2].nextLSP = NULL;
						ptr2 = &obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k2];

						/*cout << "i2 = " << i2 << endl;
						cout << "j2 = " << j2 << endl;
						cout << "k2 = " << k2 << endl;
						cout << "ptr2 = " << ptr2 << endl << endl;*/

						break;
					}
			}
		}


		obj.lighpaths[pos1].linkVector[i1].wavelengthAndLSP[j1].LSPvec[k1].nextLSP = ptr2;
		obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j2].LSPvec[k2].prevLSP = ptr1;


		/*cout << obj.lighpaths[pos1].returnId() << endl;
		cout << obj.lighpaths[pos1].linkVector[i1].destinationID << endl;
		cout << obj.lighpaths[pos1].linkVector[i1].wavelengthAndLSP[j1].LSPvec[k1].id << endl;
		//cout << obj.lighpaths[pos1].linkVector[i1].wavelengthAndLSP[j1].LSPvec[k1].LSPpath;
		cout << obj.lighpaths[pos1].linkVector[i1].wavelengthAndLSP[j1].numOfLSPsInLightpath << endl;
		cout << obj.lighpaths[pos1].linkVector[i1].wavelengthAndLSP[j1].LSPvec[k1].prev << endl;
		cout << obj.lighpaths[pos1].linkVector[i1].wavelengthAndLSP[j1].LSPvec[k1].next << endl;
		cout << obj.lighpaths[pos1].linkVector[i1].wavelengthAndLSP[j1].LSPvec[k1].prevLSP << endl;
		cout << obj.lighpaths[pos1].linkVector[i1].wavelengthAndLSP[j1].LSPvec[k1].nextLSP << endl << endl;


		cout << obj.lighpaths[pos2].returnId() << endl;
		cout << obj.lighpaths[pos2].linkVector[i2].destinationID << endl;
		cout << obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j2].LSPvec[k2].id << endl;
		//cout << obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j2].LSPvec[k2].LSPpath;
		cout << obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j2].numOfLSPsInLightpath << endl;
		cout << obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j2].LSPvec[k2].prev << endl;
		cout << obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j2].LSPvec[k2].next << endl;
		cout << obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j2].LSPvec[k2].prevLSP << endl;
		cout << obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j2].LSPvec[k2].nextLSP << endl << endl;*/

	}

	else if (shortestPathLSP.size() > 2)
	{
		int pathSize = shortestPathLSP.size();
		int numOfIntermediate = pathSize - 2;

		LSP* ptr1 = NULL;
		LSP* ptr2 = NULL;

		size_t i1 = 0, j1 = 0, k1 = 0;

		//LSPnode tempNode1;
		LSP tempObject;

		int pos1 = obj.checkForAvaialableLPNode(v1[0]);
		for (size_t i = 0; i < obj.lighpaths[pos1].linkVector.size(); i++)
		{
			if (obj.lighpaths[pos1].linkVector[i].destinationID == v1[1])
			{
				tempObject.bandwidthOfLSP = LSPbandwidth;
				tempObject.LSPpath = shortestPathLSP;
				tempObject.id = obj.lighpaths[pos1].returnId();
				tempObject.prev = NULL;
				tempObject.next = obj.lighpaths[pos1].linkVector[i].destAddress;
				/*cout << "pos1 = " << pos1 << endl;
				cout << "i = " << i << endl;
				cout << "id = " << obj.lighpaths[pos1].returnId() << endl;
				cout << "obj.lighpaths[pos1].linkVector[i].destinationID = " << obj.lighpaths[pos1].linkVector[i].destinationID << endl;*/

				for (size_t j = 0; j < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++)

					if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == intLSPwavelength)
					{
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath++;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject);
						i1 = i;
						j1 = j;
						k1 = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.size() - 1;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k1].prevLSP = NULL;
						ptr1 = &obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k1];

						/*cout << "i1 = " << i1 << endl;
						cout << "j1 = " << j1 << endl;
						cout << "k1 = " << k1 << endl;
						cout << "ptr1 = " << ptr1 << endl << endl;*/
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

			/*cout << "itr = " << itr << endl;
			cout << "pos2 = " << pos2 << endl;
			cout << "pos3 = " << pos3 << endl;
			cout << "obj.lighpaths[pos1].returnId() = " << obj.lighpaths[pos1].returnId() << endl;
			cout << "obj.lighpaths[pos2].returnId() = " << obj.lighpaths[pos2].returnId() << endl;
			cout << "obj.lighpaths[pos3].returnId() = " << obj.lighpaths[pos3].returnId() << endl << endl;*/

			size_t i = 0;
			bool switch1 = false, switch2 = false;
			for (; i < obj.lighpaths[pos2].linkVector.size(); i++)
			{
				if (obj.lighpaths[pos2].linkVector[i].destinationID == v1[itr - 1])
				{
					tempObject.bandwidthOfLSP = LSPbandwidth;
					tempObject.LSPpath = shortestPathLSP;
					tempObject.id = obj.lighpaths[pos2].returnId();
					tempObject.prev = obj.lighpaths[pos1].returnSelfAddress();
					tempObject.next = obj.lighpaths[pos3].returnSelfAddress();
					i2 = i;
					switch1 = true;

					/*cout << "v1[itr - 1] = " << v1[itr - 1] << endl;
					cout << "tempObject.id = " << obj.lighpaths[pos2].returnId() << endl;
					cout << "tempObject.prev = " << obj.lighpaths[pos1].returnId() << endl;
					cout << "tempObject.next = " << obj.lighpaths[pos3].returnId() << endl;
					cout << "i2 = " << i2 << endl;*/

				}
				if (obj.lighpaths[pos2].linkVector[i].destinationID == v1[itr + 1])
				{
					tempObject2.bandwidthOfLSP = LSPbandwidth;
					tempObject2.LSPpath = shortestPathLSP;
					tempObject2.id = obj.lighpaths[pos2].returnId();
					tempObject2.prev = obj.lighpaths[pos1].returnSelfAddress();
					tempObject2.next = obj.lighpaths[pos3].returnSelfAddress();
					tempObject2.prevLSP = ptr1;
					i3 = i;
					//tempObject2.next = obj.lighpaths[pos2].linkVector[i].destAddress;
					switch2 = true;

					/*cout << "v1[itr + 1] = " << v1[itr + 1] << endl;
					cout << "tempObject.id = " << obj.lighpaths[pos2].returnId() << endl;
					cout << "tempObject.prev = " << obj.lighpaths[pos1].returnId() << endl;
					cout << "tempObject.next = " << obj.lighpaths[pos3].returnId() << endl;
					cout << "i3 = " << i3 << endl;*/
				}

				if (switch1 && switch2)
					break;
			}


			for (size_t j = 0; j < obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP.size(); j++)
			{
				if (obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].wavelength == intLSPwavelength)
				{
					obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].numOfLSPsInLightpath++;
					obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].LSPvec.push_back(tempObject);
					j2 = j;
					k2 = obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].LSPvec.size() - 1;
					ptr2 = &obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].LSPvec[k2];

					/*cout << "i2 = " << i2 << endl;
					cout << "j2 = " << j2 << endl;
					cout << "k2 = " << k2 << endl;
					cout << "ptr2 = " << ptr2 << endl << endl;*/

					break;
				}
			}

			for (size_t j = 0; j < obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP.size(); j++)
			{
				if (obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].wavelength == intLSPwavelength)
				{
					obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].numOfLSPsInLightpath++;
					obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].LSPvec.push_back(tempObject2);
					j3 = j;
					k3 = (obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].LSPvec.size() - 1);
					ptr3 = &obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].LSPvec[k3];

					/*cout << "i3 = " << i3 << endl;
					cout << "j3 = " << j3 << endl;
					cout << "k3 = " << k3 << endl;
					cout << "ptr3 = " << ptr3 << endl << endl;*/

					break;
				}
			}

			obj.lighpaths[pos1].linkVector[i1].wavelengthAndLSP[j1].LSPvec[k1].nextLSP = ptr2;
			obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j3].LSPvec[k3].prevLSP = ptr1;

			/*cout << "obj.lighpaths[pos1].linkVector[i1].wavelengthAndLSP[j1].LSPvec[k1].nextLSP = ptr2 = " << ptr2 << endl;
			cout << "obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j3].LSPvec[k3].prevLSP = ptr1 = " << ptr1 << endl;*/


			/*cout << "ptr1 = " << ptr1 << endl;
			cout << "ptr3 = " << ptr3 << endl;
			cout << "ptr1 = ptr3" << endl;*/

			ptr1 = ptr3;

			//cout << "So ptr1 = " << ptr1 << endl;

			i1 = i2; 
			j1 = j2; 
			k1 = k2; 
			pos1 = pos2;

			/*cout << "i1 = i2" << endl;
			cout << "j1 = j2" << endl;
			cout << "k1 = k2" << endl;
			cout << "pos1 = pos2" << endl;

			cout << "i1 = " << i1 << endl;
			cout << "j1 = " << j1 << endl;
			cout << "k1 = " << k1 << endl;
			cout << "pos1 = " << pos1 << endl;*/
		}

		//cout << endl << endl;
		pos1 = obj.checkForAvaialableLPNode(v1[pathSize - 1]);

		/*cout << "pos1 = obj.checkForAvaialableLPNode(v1[pathSize - 1])" << endl;
		cout << "pos1 = " << pos1 << endl;*/

		for (size_t i = 0; i < obj.lighpaths[pos1].linkVector.size(); i++)
		{
			if (obj.lighpaths[pos1].linkVector[i].destinationID == v1[pathSize - 2])
			{
				//Availablebandwidth = totalBandwidth - LSPbandwidth
				tempObject.bandwidthOfLSP = LSPbandwidth;
				tempObject.LSPpath = shortestPathLSP;
				tempObject.id = obj.lighpaths[pos1].returnId();
				tempObject.prev = obj.lighpaths[pos1].linkVector[i].destAddress;
				tempObject.next = NULL;
				tempObject.nextLSP = NULL;

				/*cout << "tempObject.id = obj.lighpaths[pos1].returnId() = " << obj.lighpaths[pos1].returnId() << endl;
				cout << "tempObject.prev = " << obj.lighpaths[pos1].linkVector[i].destinationID << endl;
				cout << "tempObject.next = NULL" << endl;*/

				for (size_t j = 0; j < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++)

					if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == intLSPwavelength)
					{
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath++;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject);
						i4 = i;
						j4 = j;
						k4 = (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.size() - 1);
						ptr4 = &obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k4];
						/*cout << "i4 = " << i4 << endl;
						cout << "j4 = " << j4 << endl;
						cout << "k4 = " << k4 << endl;
						cout << "ptr4 = " << ptr4 << endl << endl;*/
						break;
					}

				obj.lighpaths[pos1].linkVector[i4].wavelengthAndLSP[j4].LSPvec[k4].prevLSP = ptr3;
				obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j2].LSPvec[k2].nextLSP = ptr4;

				/*cout << "obj.lighpaths[pos1].linkVector[i1].wavelengthAndLSP[j1].LSPvec[k1].prevLSP = ptr3 = " << ptr3 << endl;
				cout << "obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j2].LSPvec[k2].nextLSP = ptr1 = " << ptr1 << endl;*/

			}
		}

	}

}

void LSP::viewLSPsInALightpath(lightpathNetwork& obj)
{

	for (size_t i = 0; i < obj.lighpaths.size(); i++)
	{
		for (size_t j = 0; j < obj.lighpaths[i].linkVector.size(); j++)
		{
			cout << obj.lighpaths[i].returnId() << endl;
			for (size_t k = 0; k < obj.lighpaths[i].linkVector[j].wavelengthAndLSP.size(); k++)
			{
				if (obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].numOfLSPsInLightpath > 0)
				{
					for (size_t m = 0; m < obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec.size(); m++)
					{
						if (obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].prev == NULL && obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].next != NULL)
						{
							cout << "The LSPs start from light node " << obj.lighpaths[i].returnId() << endl;

							cout << obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].id;
							//cout << " nextLSP = " << obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].nextLSP;
							traversefLSP(obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].nextLSP);
							cout << endl;
						}

						if (obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].prev != NULL && obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].next == NULL)
						{
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

void LSP::traversefLSP(LSP* nextNode)
{

	LSP temp;
	while (nextNode->nextLSP != NULL)
	{
		temp = *nextNode;
		cout << "->" << temp.id;
		//cout << " nextLSP = " << nextNode->nextLSP << endl;
		nextNode = nextNode->nextLSP;
		//cout << "nextNode = " << nextNode << " \n";

	}
	temp = *nextNode;
	cout << "->" << temp.id << endl;

}

void LSP::traversebLSP(LSP* prevNode)
{
	LSP temp;
	while (prevNode->prevLSP != NULL)
	{
		temp = *prevNode;
		cout << "->" << temp.id;
		//cout << " nextLSP = " << nextNode->nextLSP << endl;
		prevNode = prevNode->prevLSP;
		//cout << "nextNode = " << nextNode << " \n";

	}
	temp = *prevNode;
	cout << "->" << temp.id << endl;
}
