#include <iostream>
#include <vector>
#include <algorithm>

#include "lightpathSettingup.h"
#include "graph.h"
#include "LSP.h"

class lightNode;                                //Classes declaration
class lightpathNetwork;
struct thresholds;

void LSP::makeLSP(int bandwidth, vector<int> shortestPathLSP, vector<int> wholePath, vector<int> LSPwavelengthVec, lightpathNetwork& obj, string type, int identifier, bool protectionType, thresholds thresholdVals, vector<vector<int>> partitionedPath)
{
	//cout << "\n\t New LSP Request : " << identifier;
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
	int LSPbandwidth = bandwidth;

	vector<int> v1 = shortestPathLSP;           //Shortest path for a LSP
	vector<int> positionVector;                 //Positions of the nodes in lightpaths vector
	
	if (shortestPathLSP.size() == 2)                              //If the LSP path has only 2 nodes
	{
		LSP tempObject;
		LSP tempObject2;

		int pos1 = obj.checkForAvaialableLPNode(v1[0]);
		int pos2 = obj.checkForAvaialableLPNode(v1[1]);

		positionVector.push_back(pos1);
		positionVector.push_back(pos2);

		LSP* ptr1 = NULL;
		LSP* ptr2 = NULL;

		size_t i1 = 0, i2 = 0, j1 = 0, j2 = 0, k1 = 0, k2 = 0;

		bool exit1 = false, exit2 = false;

		for (size_t i = 0; i < obj.lighpaths[pos1].linkVector.size(); i++)
		{
			if (obj.lighpaths[pos1].linkVector[i].destinationID == v1[1])
			{
				tempObject.bandwidthOfLSP = LSPbandwidth;
				tempObject.LSPpath = wholePath;
				tempObject.identifier = identifier;
				tempObject.id = obj.lighpaths[pos1].returnId();
				tempObject.prev = NULL;
				tempObject.next = obj.lighpaths[pos1].linkVector[i].destAddress;

				for (size_t j = 0; j < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++) {
					vector <int> tempVVec = wholePath;
					reverse(tempVVec.begin(), tempVVec.end());

					if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[0]
						&& (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].path == wholePath || obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].path == tempVVec))
					{
						
						int remainingBandwidth = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth;

						//cout<<"bandwidth before = "<<obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth<<endl;
						remainingBandwidth = remainingBandwidth - LSPbandwidth;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth = remainingBandwidth;

						//cout<<"bandwidth after = "<<obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth<<endl;

						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath++;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject);
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.reserve(1000);
						
						i1 = i;
						j1 = j;
						k1 = (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.size() - 1);
						//cout << "\n\t ******************* LSP Node ID : " << obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k1].id;
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
		}

		for (size_t i = 0; i < obj.lighpaths[pos2].linkVector.size(); i++)
		{

			if (obj.lighpaths[pos2].linkVector[i].destinationID == v1[0])
			{
				tempObject2.bandwidthOfLSP = LSPbandwidth;
				tempObject2.LSPpath = wholePath;
				tempObject2.identifier = identifier;
				tempObject2.id = obj.lighpaths[pos2].returnId();
				tempObject2.prev = obj.lighpaths[pos2].linkVector[i].destAddress;
				tempObject2.next = NULL;


				for (size_t j = 0; j < obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP.size(); j++) {
					vector <int> tempVVec = wholePath;
					reverse(tempVVec.begin(), tempVVec.end());

					if (obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[0]
						&& (obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].path == wholePath || obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].path == tempVVec))
					{
						
						int remainingBandwidth = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].availableBandwidth;
						//cout<<"bandwidth before = "<<obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth<<endl;
						remainingBandwidth = remainingBandwidth - LSPbandwidth;
						obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].availableBandwidth = remainingBandwidth;

						//cout<<"bandwidth after = "<<obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth<<endl;

						obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath++;
						obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject2);
						obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.reserve(1000);
						i2 = i;
						j2 = j;
						k2 = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.size() - 1;
						//cout << "--- LSP Node ID from node 2 : " << obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k2].id << endl;
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
		}


		obj.lighpaths[pos1].linkVector[i1].wavelengthAndLSP[j1].LSPvec[k1].nextLSP = ptr2;
		obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j2].LSPvec[k2].prevLSP = ptr1;

		if (type == "pLSP")
		{
			partitionedPath.push_back(wholePath);
			obj.checkHeavilyLoadLP(positionVector, partitionedPath, LSPwavelengthVec, protectionType, thresholdVals, true);
		}
	}

	else if (shortestPathLSP.size() > 2)             //If the LSP path has more than 2 nodes
	{
		int pathSize = shortestPathLSP.size();
		int numOfIntermediate = pathSize - 2;

		LSP* ptr1 = NULL;
		LSP* ptr2 = NULL;

		size_t i1 = 0, j1 = 0, k1 = 0;

		LSP tempObject;
		vector<int> PosVectorM;
		vector<int> iVectorM;
		vector<int> jVectorM;

		vector<int>::iterator it;

		int value1 = 0;
		/*for (size_t i = 1; i < v1.size(); i++)
		{
			int value2 = v1[i];
			it = std::find(wholePath.begin(), wholePath.end(), value2);
			if (it != wholePath.end())
			{
				value2 = it - wholePath.begin();
				vector<int> tempVec(wholePath.begin() + value1, wholePath.begin() + value2 + 1);
				partitionedPath.push_back(tempVec);
				tempVec.clear();
				value1 = value2;
			}
		}*/

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

				for (size_t j = 0; j < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++) {
					vector <int> tempVVec = partitionedPath[0];
					reverse(tempVVec.begin(), tempVVec.end());

					if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[0]
						&& (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].path == partitionedPath[0] || obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].path == tempVVec))
					{
						
						int remainingBandwidth = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth;
						remainingBandwidth = remainingBandwidth - LSPbandwidth;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth = remainingBandwidth;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath++;
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.push_back(tempObject);
						obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.reserve(1000);



						i1 = i;
						j1 = j;
						if(identifier == 8264){
							PosVectorM.push_back(pos1);
							iVectorM.push_back(i1);
							jVectorM.push_back(j1);
						}
						k1 = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.size() - 1;
						//cout << "\n\t ***** LSP Node ID from  : " << obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k1].id ;
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
				vector <int> tempVVec = partitionedPath[itr - 1];
				reverse(tempVVec.begin(), tempVVec.end());
				if (obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[itr - 1]
					&& (obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].path == partitionedPath[itr - 1] || obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].path == tempVVec))
				{
								
					int remainingBandwidth = obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].availableBandwidth;
					remainingBandwidth = remainingBandwidth - LSPbandwidth;
					obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].availableBandwidth = remainingBandwidth;
					obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].numOfLSPsInLightpath++;
					obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].LSPvec.push_back(tempObject);
					obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].LSPvec.reserve(1000);
					j2 = j;
					k2 = obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].LSPvec.size() - 1;
					//cout << "-- LSP Node ID from # : " << obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j].LSPvec[k2].id;
					if (identifier == 8264) {
						PosVectorM.push_back(pos2);
						iVectorM.push_back(i2);
						jVectorM.push_back(j2);
					}
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
				/*if (identifier == 714) {
					cout << "value of j: " << j << endl;
					cout << "value of waveSize3: " << waveSize3 << endl;
					cout << "obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].wavelength : " << obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].wavelength<<endl;
					cout << "LSPwavelengthVec[itr] : " << LSPwavelengthVec[itr] << endl;
					cout << "obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].path : " << obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].path[0] << endl;
					cout << "partitionedPath[itr] : " << partitionedPath[itr][0] << endl;
					cout << "obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].path  1 : " << obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].path[1] << endl;
					cout << "partitionedPath[itr] 1 : " << partitionedPath[itr][1] <<"\n\n" <<endl;
				}*/
				//partitionedPath[itr]
				vector <int> temVVec = partitionedPath[itr];
				reverse(temVVec.begin(), temVVec.end());
				if (obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[itr]
					&& (obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].path == partitionedPath[itr] || obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].path == temVVec))
				{
					int remainingBandwidth = obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].availableBandwidth;
					remainingBandwidth = remainingBandwidth - LSPbandwidth;
					obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].availableBandwidth = remainingBandwidth;
					obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].numOfLSPsInLightpath++;
					obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].LSPvec.push_back(tempObject2);
					obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].LSPvec.reserve(1000);
					j3 = j;
					k3 = (obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j3].LSPvec.size() - 1);
					//cout << "-- LSP Node ID from # : " << obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j].LSPvec[k3].id;
					if (identifier == 8264) {
						PosVectorM.push_back(pos2);
						iVectorM.push_back(i3);
						jVectorM.push_back(j3);
					}

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

			//cout << "obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j3].LSPvec.size() = " << obj.lighpaths[pos2].linkVector[i3].wavelengthAndLSP[j3].LSPvec.size() << endl;

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

				for (size_t j = 0; j < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++) {
					vector<int> temVVVec = partitionedPath[numOfIntermediate];
					reverse(temVVVec.begin(), temVVVec.end());
					if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[numOfIntermediate]
						&& (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].path == partitionedPath[numOfIntermediate] || obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].path == temVVVec))
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
						//cout << "-- LSP Node ID from last : " << obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k4].id << endl;
						if (identifier == 8264) {
							PosVectorM.push_back(pos1);
							iVectorM.push_back(i4);
							jVectorM.push_back(j4);
						}
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
				}

				
				obj.lighpaths[pos1].linkVector[i4].wavelengthAndLSP[j4].LSPvec[k4].prevLSP = ptr3;
				obj.lighpaths[pos2].linkVector[i2].wavelengthAndLSP[j2].LSPvec[k2].nextLSP = ptr4;


			}
		}
		for (size_t i = 0; i < shortestPathLSP.size(); i++)
		{
			int pos = obj.checkForAvaialableLPNode(shortestPathLSP[i]);
			positionVector.push_back(pos);
		}
		if (type == "pLSP")
			obj.checkHeavilyLoadLP(positionVector, partitionedPath, LSPwavelengthVec, protectionType, thresholdVals, true);

		/*if (identifier == 8264) {
			cout << "\n posVectorM : ";
			for (int z = 0; z < PosVectorM.size(); z++)
				cout << "\t" << PosVectorM[z];
			cout << "\n iVectorM : ";
			for (int z = 0; z < iVectorM.size(); z++)
				cout << "\t" << iVectorM[z];
			cout << "\n jVectorM : ";
			for (int z = 0; z < jVectorM.size(); z++)
				cout << "\t" << jVectorM[z];

			cout << "\n Mokaka hari ekak: ";
			for (int z = 0; z < PosVectorM.size(); z++) {
				for (int y = 0; y < obj.lighpaths[PosVectorM[z]].linkVector[iVectorM[z]].wavelengthAndLSP[jVectorM[z]].LSPvec.size(); y++) {
					cout << "\t" << obj.lighpaths[PosVectorM[z]].linkVector[iVectorM[z]].wavelengthAndLSP[jVectorM[z]].LSPvec[y].id;
				}
				cout << "\n";
			}
			cout << "hi: ";
		}*/
		
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

							cout << "LSPpath = \n";
							int sizeOfLSPpath = obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].LSPpath.size();
							for (size_t n = 0; n < (sizeOfLSPpath - 1); n++)
							{
								cout << obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].LSPpath[n] << "--->";
							}
							cout << obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].LSPpath[sizeOfLSPpath - 1];
							cout << endl;
							cout << obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].id;
							traversefLSP(obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].nextLSP);
							cout << endl;
						}

						if (obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].prev != NULL && obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].next == NULL)
						{
							cout << "Wavelength = " << obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].wavelength << endl;
							cout << "The LSPs start from light node " << obj.lighpaths[i].returnId() << endl;

							cout << "LSPpath = \n";
							int sizeOfLSPpath = obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].LSPpath.size();
							for (size_t n = 0; n < (sizeOfLSPpath - 1); n++)
							{
								cout << obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].LSPpath[n] << "--->";
							}
							cout << obj.lighpaths[i].linkVector[j].wavelengthAndLSP[k].LSPvec[m].LSPpath[sizeOfLSPpath - 1];
							cout << endl;
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



void LSP::releaseLSP(vector<int> path, vector<int> LSPwavelengthVec, lightpathNetwork& obj, int identifier, thresholds THval, bool protectionType, string typeOfLSP , int &_counter) {
	//cout << "\n\t\t NEW DELETE  : " << identifier;
	/*cout << "path: ";
	for (int i = 0; i < path.size(); i++)
		cout << "\t" << path[i];
	cout << "\nwavelengths :";
	for (int i = 0; i < LSPwavelengthVec.size(); i++)
		cout << "\t" << LSPwavelengthVec[i];*/


	if (path.size() == 2) {

		vector<int> v1 = path;
		int pos1 = obj.checkForAvaialableLPNode(v1[0]);
		int pos2 = obj.checkForAvaialableLPNode(v1[1]);
		vector<int> pos1Vector;
		vector <int> pos2Vector;
		vector<int> iVector;
		vector<int> IdVector;
		vector<int> wVector;
		vector<int> idVector;
		string type; // storing type of the LSP
		// from souce not to Destination node
		for (size_t i = 0; i < obj.lighpaths[pos1].linkVector.size(); i++) {
			//cout << "\ndestinationID : " << obj.lighpaths[pos1].linkVector[i].destinationID << "\t V1[1] :" << v1[1];
			if (obj.lighpaths[pos1].linkVector[i].destinationID == v1[1]) {//check LP destination Address
				for (size_t j = 0; j < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++) {
					//cout << "\n\twavelength : " << obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength << "\tLSPwavelenghtVec[0] : " << LSPwavelengthVec[0];
					if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[0]) { //check LSP wavelength
						for (size_t k = 0; k < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.size(); k++) {
							//cout << "\n\t\tidentifier : " << obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].identifier << "\t identifier : " << identifier;
							type = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPtype;
							if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].identifier == identifier && type == typeOfLSP) { //check LSP Identifier


								/*if (type == "pLSP") {
									cout << "\n\t\t\tPrimary LSP deleted	: " << identifier << endl;
								}
								else {
									cout << "\n\t\t\tBackup LSP deleted	: " << identifier << endl;
								}*/


								if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPtype == "pLSP") {	//if LSP is a Primary LSP
											// Saving type of LSP
									obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath--;	// update primary LSP counter
									// update primary LSP bandwidht of the LP
									obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth + obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].bandwidthOfLSP;
								}
								// update available bandwith of LP afeter Deletion
								//cout << "\n\t\tBefore delete -Number of LSP in LP : " << obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath;
								obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth + obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].bandwidthOfLSP;
								// update LSP counter in LP
								obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath--;
								// Deletion of LPS
								obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.erase(obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.begin() + k);
								//cout << "\n\t\tAfter delete -Number of LSP in LP : " << obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath;

								pos1Vector.push_back(pos1);
								pos2Vector.push_back(pos2);
								iVector.push_back(i);
								IdVector.push_back(obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LPidentifier);
								wVector.push_back(obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength);
								//idVector.push_back(obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LPidentifier);
							}
						}
					}
				}
			}
		}
		//from destinaion node to source node
		//cout << "\n\n\t second part of deletion";
		for (size_t i = 0; i < obj.lighpaths[pos2].linkVector.size(); i++) {
			//cout << "\ndestinationID  : " << obj.lighpaths[pos2].linkVector[i].destinationID << "\t V1[0] :" << v1[0];
			if (obj.lighpaths[pos2].linkVector[i].destinationID == v1[0]) {
				for (size_t j = 0; j < obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP.size(); j++) {
					//cout << "\n\twavelength : " << obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].wavelength << "\t LSPwavelenghtVec[0] : " << LSPwavelengthVec[0];
					if (obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[0]) {
						for (size_t k = 0; k < obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.size(); k++) {
							//cout << "\n\t\tidentifier : " << obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k].identifier << "\t identifier : " << identifier;
							type = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPtype;
							if (obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k].identifier == identifier && type == typeOfLSP) {


								/*if (type == "pLSP") {
										cout << "\n\t\t\tPrimary LSP Deleted : : " << identifier << endl;
								}
								else {
										cout << "\n\t\t\tBackup LSP Deleted : : " << identifier << endl;
								}*/


								if (obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPtype == "pLSP") {

									obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath--; // update primary LSP counter
									obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth + obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k].bandwidthOfLSP;
								}
								obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].availableBandwidth = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].availableBandwidth + obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k].bandwidthOfLSP;
								//cout << "\n\t\t\t\t before Delete -> number of LSP In LP : " << obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath;
								obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath--;// update LSP counter
								obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.erase(obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.begin() + k);		//deleting of LSP happen here				
								int long tempIdOfLP = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LPidentifier;
								/*cout << "\n\t\t\t\t after Delete -> number of LSP In LP : " << obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath;
								cout << "\npos2: " << pos2 << endl;
								cout << "pos1: " << pos1 << endl;
								cout << "tempIDOfLP : " << tempIdOfLP << endl;
								cout << "\n\n  ---------------------------------------------\n";
								cout << "LSP deleted" << endl;
								cout << " ---------------------------------------------\n\n";*/

								//cout << "\n type : " << type << "\t havingBackup : " << obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].havingBackup;
								if (type == "pLSP" && obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].havingBackup) {
									//cout << "\n\t protection type : " << protectionType;
									if (protectionType) {
										int tempBD = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth;
										float bandwithPecentage = tempBD / obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].initialBandwidth;
										//cout << "\n\t\t bandwidth Percentage : " << bandwithPecentage;
										//cout << "\n\t\t bandwidthThreshold : " << THval.bandwidthThreshold;
										if (bandwithPecentage <= THval.bandwidthThreshold) {
											//delete backup LP (need to find Backup LP)
											//int long tempIdOfBackupLP = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LPidentifier;
											for (int m = 0; m < obj.lighpaths.size(); m++) {
												for (int n = 0; n < obj.lighpaths[m].linkVector.size(); n++) {
													for (int o = 0; o < obj.lighpaths[m].linkVector[n].wavelengthAndLSP.size(); o++) {
														if (obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].LPidentifier == tempIdOfLP && obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].lightpathType == "bp") {
															int tempWaveL = obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].wavelength;
															//cout << "Backup LS Is Deleted :" << endl;
															obj.releaseEshtablishedLighpath(pos2, pos1, tempIdOfLP, tempWaveL, "bp");

														}
													}
												}
											}
										}
									}
									else {
										//cout << "\n\t\t number of LSP in LP : " << obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath;
										//cout << "\n\t\t numOfLSP Threshold : " << THval.numLSPthreshold;
										if (obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath <= THval.numLSPthreshold) {
											//delete backup LP
											//tempIdOfBackupLP = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LPidentifier;
											for (int m = 0; m < obj.lighpaths.size(); m++) {
												for (int n = 0; n < obj.lighpaths[m].linkVector.size(); n++) {
													for (int o = 0; o < obj.lighpaths[m].linkVector[n].wavelengthAndLSP.size(); o++) {
														if (obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].LPidentifier == tempIdOfLP && obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].lightpathType == "bp") {
															int tempWaveL = obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].wavelength;
															//cout << "Backup LS Is Deleted :" << endl;
															obj.releaseEshtablishedLighpath(pos2, pos1, tempIdOfLP, tempWaveL, "bp");
														}
													}
												}
											}
										}
									}
								}


								//cout << "Wavelenght : " << LSPwavelengthVec[0] << endl;
								//cout << "available Bandwidth : " << obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].availableBandwidth << endl;
								//cout << "Initial Bandwidth : " << obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].initialBandwidth << endl;
								// delete LP if it is empty

								//goto label;
								//break;
							}
						}
					}
				}
			}
		}

		for (int z = 0; z < pos1Vector.size(); z++) {
			for (int x = 0; x < obj.lighpaths[pos1Vector[z]].linkVector[iVector[z]].wavelengthAndLSP.size(); x++) {
				if ((obj.lighpaths[pos1Vector[z]].linkVector[iVector[z]].wavelengthAndLSP[x].availableBandwidth == obj.lighpaths[pos1Vector[z]].linkVector[iVector[z]].wavelengthAndLSP[x].initialBandwidth)
					&& (obj.lighpaths[pos1Vector[z]].linkVector[iVector[z]].wavelengthAndLSP[x].lightpathType == "pp") &&
					(obj.lighpaths[pos1Vector[z]].linkVector[iVector[z]].wavelengthAndLSP[x].LPidentifier == IdVector[z])) {
					//cout << "\n\t\tLP which LSP was established is Deleted" <<endl ;
						obj.releaseEshtablishedLighpath(pos1Vector[z], pos2Vector[z], IdVector[z], wVector[z], "pp");
				}
			}

		}

	}
	//label:

	if (path.size() > 2) {

		int pathSize = path.size();
		vector<int> v1 = path;
		int numOfIntermediate = pathSize - 2;
		int pos1 = obj.checkForAvaialableLPNode(v1[0]);
		int pos2 = obj.checkForAvaialableLPNode(v1[v1.size() - 1]);
		string type;

		vector<int> posVector;
		vector<int> iVector;
		vector<int> jVector;
		vector<int long> idVector;

		vector<int> posVector2;
		vector<int> pos2Vector2;
		vector<int> iVector2;
		vector<int> jVector2;
		vector<int long> idVector2;
		vector<int> wVector2;
		bool posVector2Activated = false;

		vector<int> i2Vector;
		vector<int> j2Vector;

		bool avoideMultiplePushing = false;
		for (size_t i = 0; i < obj.lighpaths[pos1].linkVector.size(); i++) {
			//cout << "\ndestination ID : " << obj.lighpaths[pos1].linkVector[i].destinationID << "\t\tv1[1] : " << v1[1];
			if (obj.lighpaths[pos1].linkVector[i].destinationID == v1[1]) {

				for (size_t j = 0; j < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP.size(); j++) {
					//cout << "\n\twavelentth : " << obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength << "\t\tLSPwavelengthVec[0] : " << LSPwavelengthVec[0];
					if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[0]) {
						
						for (size_t k = 0; k < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.size(); k++) {
							//cout << "\n\t\tidentifier : " << obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].identifier << "\t\tidentifier 2 : " << identifier;
							type = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPtype;
							//cout << "\n\ttype : " << type << "\ttypeOfLPS : " << typeOfLSP;
							if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].identifier == identifier && type == typeOfLSP) {



								/*if (type == "pLSP") {
									cout << "\n\t\t\tPrimary LSP Deleted : : " << identifier << endl;
								}
								else {
									cout << "\n\t\t\tBackup LSP Deleted : : " << identifier << endl;
								}*/

								if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPtype == "pLSP") {
									type = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPtype;
									obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath--;
									obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth + obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].bandwidthOfLSP;
								}
								obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].availableBandwidth + obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].bandwidthOfLSP;
								obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath--;
								
								//cout << " \n LSP path  :  ";
								/*for (int a = 0; a < obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPpath.size(); a++)
									cout << "--" << obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPpath[a];*/
								
								obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.erase(obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LSPvec.begin() + k);
								if (!avoideMultiplePushing) {
									posVector.push_back(pos1);
									iVector.push_back(i);
									jVector.push_back(j);
									idVector.push_back(obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LPidentifier);
									//cout << "\n\t\t\t values are pushed";
								}
								
								//goto label2;
								/************************************************************************************************************************/
								if (avoideMultiplePushing) {
									posVector2Activated = true;
									posVector2.push_back(pos1);
									pos2Vector2.push_back(obj.checkForAvaialableLPNode(v1[1]));
									iVector2.push_back(i);
									jVector2.push_back(j);
									idVector2.push_back(obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LPidentifier);
									wVector2.push_back(obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].wavelength);

									if (type == "pLSP" && obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].havingBackup) {
										if (protectionType) {
											float bandwithPecentage = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth / obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].initialBandwidth;
											if (bandwithPecentage <= THval.bandwidthThreshold) {
												//delete backup LP (need to find Backup LP)
												int long tempIdOfBackupLP = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LPidentifier;
												for (int m = 0; m < obj.lighpaths.size(); m++) {
													for (int n = 0; n < obj.lighpaths[m].linkVector.size(); n++) {
														for (int o = 0; o < obj.lighpaths[m].linkVector[n].wavelengthAndLSP.size(); o++) {
															if (obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].LPidentifier == tempIdOfBackupLP && obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].lightpathType == "bp") {
																int tempWaveL = obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].wavelength;
																//cout << "\nBackup LS Is Deleted :" << endl;
																obj.releaseEshtablishedLighpath(pos1, obj.checkForAvaialableLPNode(v1[1]) , tempIdOfBackupLP, tempWaveL, "bp");
															}
														}
													}
												}
											}
										}
										else {
											if (obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath <= THval.numLSPthreshold) {
												//delete backup LP
												int long tempIdOfBackupLP = obj.lighpaths[pos1].linkVector[i].wavelengthAndLSP[j].LPidentifier;
												for (int m = 0; m < obj.lighpaths.size(); m++) {
													for (int n = 0; n < obj.lighpaths[m].linkVector.size(); n++) {
														for (int o = 0; o < obj.lighpaths[m].linkVector[n].wavelengthAndLSP.size(); o++) {
															if (obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].LPidentifier == tempIdOfBackupLP && obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].lightpathType == "bp") {
																int tempWaveL = obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].wavelength;
																//cout << "\nBackup LS Is Deleted :" << endl;
																obj.releaseEshtablishedLighpath(pos1, obj.checkForAvaialableLPNode(v1[1]), tempIdOfBackupLP, tempWaveL, "bp");
															}
														}
													}
												}
											}
										}
									}
								}
								/**********************************************************************/

								avoideMultiplePushing = true;
							}
						}
					}
				}
			}
		}
		//label2:
		//cout << "\n\n\t second part of deletion";
		for (size_t itr = 1; itr <= numOfIntermediate; itr++) {
			int intermediateTemp01 = obj.checkForAvaialableLPNode(v1[itr]);
			int intermediateTemp02 = obj.checkForAvaialableLPNode(v1[itr + 1]);
		//	cout << "\n\t intermediateTemp 01 : " << intermediateTemp01;
			//cout << "\n\t intermediateTemp 02 : " << intermediateTemp02;
			//cout << "\n\t size of link vector : " << obj.lighpaths[intermediateTemp01].linkVector.size();
			for (size_t i = 0; i < obj.lighpaths[intermediateTemp01].linkVector.size(); i++) {
			//	cout << "\ndestination ID : " << obj.lighpaths[intermediateTemp01].linkVector[i].destinationID << "\t\tv1[itr + 1] : " << v1[itr + 1];
				if (obj.lighpaths[intermediateTemp01].linkVector[i].destinationID == v1[itr + 1]) {
					for (size_t j = 0; j < obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP.size(); j++) {
					//	cout << "\n\twavelentth : " << obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].wavelength << "\t\tLSPwavelengthVec[itr] : " << LSPwavelengthVec[itr];
						if (obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[itr]) {
							
							for (size_t k = 0; k < obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec.size(); k++) {
							//	cout << "\n\t\tidentifier : " << obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec[k].identifier << "\t\tidentifier 2 : " << identifier;
								type = obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPtype;
								if (obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec[k].identifier == identifier && type == typeOfLSP) {

									if (obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPtype == "pLSP") {
										obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath--;
										obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth = obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth + obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec[k].bandwidthOfLSP;
									}

									obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].availableBandwidth = obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].availableBandwidth + obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec[k].bandwidthOfLSP;
									obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath--;
									obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec.erase(obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec.begin() + k);
									
										posVector.push_back(intermediateTemp01);
										iVector.push_back(i);
										jVector.push_back(j);
										idVector.push_back(obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LPidentifier);
									
									
									
								}
							}
						}
					}
				}
				//cout << "\ndestination ID : " << obj.lighpaths[intermediateTemp01].linkVector[i].destinationID << "\t\tv1[itr - 1] : " << v1[itr - 1];
				if (obj.lighpaths[intermediateTemp01].linkVector[i].destinationID == v1[itr - 1]) {
					for (size_t j = 0; j < obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP.size(); j++) {
					//	cout << "\n\twavelentth : " << obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].wavelength << "\t\tLSPwavelengthVec[itr-1] : " << LSPwavelengthVec[itr-1];
						if (obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[itr - 1]) {
							for (size_t k = 0; k < obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec.size(); k++) {
								//cout << "\n\t\tidentifier : " << obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec[k].identifier << "\t\tidentifier 2 : " << identifier;
								type = obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPtype;
								if (obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec[k].identifier == identifier && type == typeOfLSP) {

									if (obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPtype == "pLSP") {
										obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath--;
										obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth = obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth + obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec[k].bandwidthOfLSP;
									}
									obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].availableBandwidth = obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].availableBandwidth + obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec[k].bandwidthOfLSP;
									obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath--;
									obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec.erase(obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LSPvec.begin() + k);

									i2Vector.push_back(i);
									j2Vector.push_back(j);

									if (type == "pLSP" && obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].havingBackup) {
										if (protectionType) {
											float bandwithPecentage = obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth / obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].initialBandwidth;
											if (bandwithPecentage <= THval.bandwidthThreshold) {
												//delete backup LP (need to find Backup LP)
												int long tempIdOfBackupLP = obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LPidentifier;
												for (int m = 0; m < obj.lighpaths.size(); m++) {
													for (int n = 0; n < obj.lighpaths[m].linkVector.size(); n++) {
														for (int o = 0; o < obj.lighpaths[m].linkVector[n].wavelengthAndLSP.size(); o++) {
															if (obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].LPidentifier == tempIdOfBackupLP && obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].lightpathType == "bp") {
																int tempWaveL = obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].wavelength;
																//cout << "\nBackup LS Is Deleted :" << endl;
																obj.releaseEshtablishedLighpath(intermediateTemp01, intermediateTemp02, tempIdOfBackupLP, tempWaveL, "bp");
															}
														}
													}
												}
											}
										}
										else {
											if (obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath <= THval.numLSPthreshold) {
												//delete backup LP
												int long tempIdOfBackupLP = obj.lighpaths[intermediateTemp01].linkVector[i].wavelengthAndLSP[j].LPidentifier;
												for (int m = 0; m < obj.lighpaths.size(); m++) {
													for (int n = 0; n < obj.lighpaths[m].linkVector.size(); n++) {
														for (int o = 0; o < obj.lighpaths[m].linkVector[n].wavelengthAndLSP.size(); o++) {
															if (obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].LPidentifier == tempIdOfBackupLP && obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].lightpathType == "bp") {
																int tempWaveL = obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].wavelength;
																//cout << "\nBackup LS Is Deleted :" << endl;
																obj.releaseEshtablishedLighpath(intermediateTemp01, intermediateTemp02, tempIdOfBackupLP, tempWaveL, "bp");
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}

		}



		for (size_t i = 0; i < obj.lighpaths[pos2].linkVector.size(); i++) {
			//cout << "\ndestination ID : " << obj.lighpaths[pos2].linkVector[i].destinationID << "\t\tv1[v1.size() - 2] : " << v1[v1.size() - 2];
			if (obj.lighpaths[pos2].linkVector[i].destinationID == v1[v1.size() - 2]) {
				for (size_t j = 0; j < obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP.size(); j++) {
					//cout << "\n\twavelentth : " << obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].wavelength << "\t\tLSPwavelengthVec[LSPwavelengthVec.size() - 1] : " << LSPwavelengthVec[LSPwavelengthVec.size() - 1];
					if (obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].wavelength == LSPwavelengthVec[LSPwavelengthVec.size() - 1]) {
						for (size_t k = 0; k < obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.size(); k++) {
							//cout << "\n\t\tidentifier : " << obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k].identifier << "\t\tidentifier 2 : " << identifier;
							type = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPtype;
							if (obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k].identifier == identifier && type == typeOfLSP) {

								if (obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k].LSPtype == "pLSP") {
									obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath--;
									obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth + obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k].bandwidthOfLSP;
								}

								obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].availableBandwidth = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].availableBandwidth + obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec[k].bandwidthOfLSP;
								obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].numOfLSPsInLightpath--;

								obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.erase(obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LSPvec.begin() + k);

								posVector.push_back(pos2);

								if (type == "pLSP" && obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].havingBackup) {
									if (protectionType) {
										float bandwithPecentage = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].primaryLSPbandwidth / obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].initialBandwidth;
										if (bandwithPecentage <= THval.bandwidthThreshold) {
											//delete backup LP (need to find Backup LP)
											int long tempIdOfBackupLP = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LPidentifier;
											for (int m = 0; m < obj.lighpaths.size(); m++) {
												for (int n = 0; n < obj.lighpaths[m].linkVector.size(); n++) {
													for (int o = 0; o < obj.lighpaths[m].linkVector[n].wavelengthAndLSP.size(); o++) {
														if (obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].LPidentifier == tempIdOfBackupLP && obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].lightpathType == "bp") {
															int tempWaveL = obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].wavelength;
															//cout << "\nBackup LS Is Deleted :" << endl;
															obj.releaseEshtablishedLighpath(pos2, obj.checkForAvaialableLPNode(v1[v1.size() -1]), tempIdOfBackupLP, tempWaveL, "bp");
														}
													}
												}
											}
										}
									}
									else {
										if (obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].numOfPrimaryLSPsInLightpath <= THval.numLSPthreshold) {
											//delete backup LP
											int long tempIdOfBackupLP = obj.lighpaths[pos2].linkVector[i].wavelengthAndLSP[j].LPidentifier;
											for (int m = 0; m < obj.lighpaths.size(); m++) {
												for (int n = 0; n < obj.lighpaths[m].linkVector.size(); n++) {
													for (int o = 0; o < obj.lighpaths[m].linkVector[n].wavelengthAndLSP.size(); o++) {
														if (obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].LPidentifier == tempIdOfBackupLP && obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].lightpathType == "bp") {
															int tempWaveL = obj.lighpaths[m].linkVector[n].wavelengthAndLSP[o].wavelength;
															//cout << "\nBackup LS Is Deleted :" << endl;
															obj.releaseEshtablishedLighpath(pos2, obj.checkForAvaialableLPNode(v1[v1.size() - 1]), tempIdOfBackupLP, tempWaveL, "bp");
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		
		/*cout << "\nPosVect : ";
		for (int x = 0; x < posVector.size(); x++) {
			cout << "   " << posVector[x];
		}
		cout << "\n idVector : ";
		for (int x = 0; x < idVector.size(); x++) {
			cout << "   " << idVector[x];
		}
		cout << "\n LSPwavelenthVec : ";
		for (int x = 0; x < LSPwavelengthVec.size(); x++) {
			cout << "\t" << LSPwavelengthVec[x];
		}
		cout << endl;*/
		
		for (int z = 0; z < posVector.size() - 1; z++) {
			for (int x = 0; x < obj.lighpaths[posVector[z]].linkVector[iVector[z]].wavelengthAndLSP.size(); x++) {
				//if (identifier )
							//cout << "avaliable bandwidth : " << obj.lighpaths[posVector[z]].linkVector[iVector[z]].wavelengthAndLSP[jVector[z]].availableBandwidth << endl;
							//cout << "initial Bandwidth : " << obj.lighpaths[posVector[z]].linkVector[iVector[z]].wavelengthAndLSP[jVector[z]].initialBandwidth <<endl ;
				if (obj.lighpaths[posVector[z]].linkVector[iVector[z]].wavelengthAndLSP[x].availableBandwidth == obj.lighpaths[posVector[z]].linkVector[iVector[z]].wavelengthAndLSP[x].initialBandwidth &&
					obj.lighpaths[posVector[z]].linkVector[iVector[z]].wavelengthAndLSP[x].lightpathType == "pp" 
					&& (obj.lighpaths[posVector[z]].linkVector[iVector[z]].wavelengthAndLSP[x].LPidentifier == idVector[z])) {
					//cout << "\n\t\tLP which LSP was established is Deleted" << endl;
					obj.releaseEshtablishedLighpath(posVector[z], posVector[z + 1], idVector[z], LSPwavelengthVec[z], "pp");
				}
			}
			
		}

		//cout << "\n\t\t AvoidMultiplePushing : " << avoideMultiplePushing;
		if (posVector2Activated) {
			_counter++;
			for (int z = 0; z < posVector2.size(); z++) {
				for (int x = 0; x < obj.lighpaths[posVector2[z]].linkVector[iVector2[z]].wavelengthAndLSP.size(); x++) {
					if (obj.lighpaths[posVector2[z]].linkVector[iVector2[z]].wavelengthAndLSP[x].availableBandwidth == obj.lighpaths[posVector2[z]].linkVector[iVector2[z]].wavelengthAndLSP[x].initialBandwidth &&
						obj.lighpaths[posVector2[z]].linkVector[iVector2[z]].wavelengthAndLSP[x].lightpathType == "pp" 
						&&  (obj.lighpaths[posVector2[z]].linkVector[iVector2[z]].wavelengthAndLSP[x].LPidentifier == idVector2[z]) ) {
						//cout << "\n\t\tLP which LSP was established is Deleted" << endl;
						obj.releaseEshtablishedLighpath(posVector2[z], pos2Vector2[z], idVector2[z], wVector2[z], "pp");
					}
				}
				
			}

		}


	}


}