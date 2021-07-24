#include "eventDrivenSimulator.h"
#include <stdlib.h>     
#include <time.h>
#include <random>

bool compareByLength(const request& a, const request& b)
{
	return a.remainingTime < b.remainingTime;
}

void requestCreation::requestGenerator()
{
	request obj;
	srand(time(NULL));
	
	random_device rd; // uniformly-distributed integer random number generator
	mt19937 rng(rd()); // mt19937: Pseudo-random number generation

	double averageArrival = 15;
	double lamda = 1 / averageArrival;
	lamda = 10;
	exponential_distribution<double> exp(lamda);

	random_device rd2; // uniformly-distributed integer random number generator
	mt19937 rng2(rd2()); // mt19937: Pseudo-random number generation

	exponential_distribution<double> exp2(1);

	int numberOfLSPrequests = 5;
	for (size_t i = 0; i < numberOfLSPrequests; i++)
	{
		//obj.identifier = ;
		obj.sourceNode = rand() % 14;
		do
		{
			obj.destinationNode = rand() % 14;
		} while (obj.destinationNode == obj.sourceNode);
		obj.bandwidth = 10 + rand() % 6;
		//obj.interArrivalTime = exp.operator() (rng);// generates the next random number in the distribution 
		//obj.holdingTime = exp2.operator() (rng2);// generates the next random number in the distribution 
		//obj.remainingTime = obj.holdingTime;

		obj.interArrivalTime = 1 + rand() % 10;
		obj.holdingTime = 1 + rand() % 28;
		obj.remainingTime = obj.holdingTime;

		requestVector.push_back(obj);
	}

	for (size_t i = 0; i < requestVector.size(); i++)
	{
		cout << "***************************\n";
		cout << "sourceNode = " << requestVector[i].sourceNode << endl;
		cout << "destinationNode = " << requestVector[i].destinationNode << endl;
		cout << "bandwidth = " << requestVector[i].bandwidth << endl;
		cout << "holdingTime = " << requestVector[i].holdingTime << endl;
		cout << "interArrivalTime = " << requestVector[i].interArrivalTime << endl;
	}
	cout << "***************************\n";
}

void requestCreation::eventCreation()
{
	event eventObject;
	//eventObject.identifier = requestVector[0].identifier;
	eventObject.sourceNode = requestVector[0].sourceNode;
	eventObject.destinationNode = requestVector[0].destinationNode;
	eventObject.bandwidth = requestVector[0].bandwidth;
	eventObject.action = 1; //establish the LSP connection
	eventVector.push_back(eventObject);

	vector<request> tempRequest;
	tempRequest.push_back(requestVector[0]);
	vector<request>::iterator it;

	it = requestVector.begin();
	cout << "requestVector.size() = " << requestVector.size() << endl;
	requestVector.erase(it);
	cout << "requestVector.size() = " << requestVector.size() << endl;
	//remove this element from request Vector

	while (!requestVector.empty())
	{
		double sum = 0;
		do
		{
			cout << "requestVector[0].interArrivalTime = " << requestVector[0].interArrivalTime << endl;
			sum += requestVector[0].interArrivalTime;
			cout << "tempRequest[0].remainingTime = " << tempRequest[0].remainingTime << endl;
			if (sum < tempRequest[0].remainingTime)
			{
				for (size_t j = 0; j < tempRequest.size(); j++)
				{
					tempRequest[j].remainingTime -= requestVector[0].interArrivalTime;
				}
				tempRequest.push_back(requestVector[0]);
				sort(tempRequest.begin(), tempRequest.end(), compareByLength);//sort tempRequest

				//eventObject.identifier = requestVector[0].identifier;
				eventObject.sourceNode = requestVector[0].sourceNode;
				eventObject.destinationNode = requestVector[0].destinationNode;
				eventObject.bandwidth = requestVector[0].bandwidth;
				eventObject.action = 1; //establish the LSP connection
				eventVector.push_back(eventObject);

				it = requestVector.begin();
				requestVector.erase(it);
				//remove that request from requestVector
			}
			else
			{
				//eventObject.identifier = tempRequest[0].identifier;
				eventObject.sourceNode = tempRequest[0].sourceNode;
				eventObject.destinationNode = tempRequest[0].destinationNode;
				eventObject.bandwidth = tempRequest[0].bandwidth;
				eventObject.action = 0; //remove the LSP connection
				eventVector.push_back(eventObject);

				//remove it from tempRequest
				it = tempRequest.begin();
				tempRequest.erase(it);
			}
		}
		while (tempRequest[0].remainingTime>sum);
	}
}