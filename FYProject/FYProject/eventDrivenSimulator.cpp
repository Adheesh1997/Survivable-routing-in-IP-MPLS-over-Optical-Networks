#include "eventDrivenSimulator.h"
#include <stdlib.h>     
#include <time.h>
#include <random>

bool compareByLength(const request& a, const request& b)
{
	return a.remainingTime < b.remainingTime;
}

void requestCreation::requestGenerator(int numberOfLSPrequests)
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

	//int numberOfLSPrequests = 100;
	for (size_t i = 0; i < numberOfLSPrequests; i++)
	{
		//obj.identifier = ;
		obj.sourceNode = rand() % 14;
		do
		{
			obj.destinationNode = rand() % 14;
		} while (obj.destinationNode == obj.sourceNode);
		obj.bandwidth = 10 + rand() % 6;
		obj.interArrivalTime = exp.operator() (rng);// generates the next random number in the distribution 
		obj.holdingTime = exp2.operator() (rng2);// generates the next random number in the distribution 
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
	requestVector.erase(it);
	//remove this element from request Vector
	

	while (!requestVector.empty())
	{
		double sum = 0;
		do
		{			
			sum += requestVector[0].interArrivalTime;

			if (sum < tempRequest[0].remainingTime)
			{
				for (size_t j = 0; j < tempRequest.size(); j++)
					tempRequest[j].remainingTime -= requestVector[0].interArrivalTime;					
				
				tempRequest.push_back(requestVector[0]);
				sort(tempRequest.begin(), tempRequest.end(), compareByLength);//sort tempRequest


				//eventObject.identifier = requestVector[0].identifier;
				eventObject.sourceNode = requestVector[0].sourceNode;
				eventObject.destinationNode = requestVector[0].destinationNode;
				eventObject.bandwidth = requestVector[0].bandwidth;
				eventObject.action = 1; //establish the LSP connection
				eventVector.push_back(eventObject);

				it = requestVector.begin();
				requestVector.erase(it);//remove that request from requestVector

			}
			else
			{
				//eventObject.identifier = tempRequest[0].identifier;
				eventObject.sourceNode = tempRequest[0].sourceNode;
				eventObject.destinationNode = tempRequest[0].destinationNode;
				eventObject.bandwidth = tempRequest[0].bandwidth;
				eventObject.action = 0; //remove the LSP connection
				eventVector.push_back(eventObject);

				for (size_t j = 1; j < tempRequest.size(); j++)
					tempRequest[j].remainingTime -= tempRequest[0].remainingTime;
				

				if (requestVector.size() > 0)
					requestVector[0].interArrivalTime -= tempRequest[0].remainingTime;

				//remove it from tempRequest
				it = tempRequest.begin();
				tempRequest.erase(it);

				sort(tempRequest.begin(), tempRequest.end(), compareByLength);//sort tempRequest

				sum = 0;
				if (tempRequest.empty())//&&!requestVector.empty()
				{
					tempRequest.push_back(requestVector[0]);

					//eventObject.identifier = requestVector[0].identifier;
					eventObject.sourceNode = requestVector[0].sourceNode;
					eventObject.destinationNode = requestVector[0].destinationNode;
					eventObject.bandwidth = requestVector[0].bandwidth;
					eventObject.action = 1; //establish the LSP connection
					eventVector.push_back(eventObject);

					it = requestVector.begin();
					requestVector.erase(it);
				}
			}
		}
		while (tempRequest[0].remainingTime>sum && !requestVector.empty());
	}

	while (!tempRequest.empty())
	{
		eventObject.sourceNode = tempRequest[0].sourceNode;
		eventObject.destinationNode = tempRequest[0].destinationNode;
		eventObject.bandwidth = tempRequest[0].bandwidth;
		eventObject.action = 0; //remove the LSP connection
		eventVector.push_back(eventObject);

		it = tempRequest.begin();
		tempRequest.erase(it);
	}

	for (size_t i = 0; i < eventVector.size(); i++)
	{
		cout << "**************************\n";
		cout << "eventVector[" << i << "].sourceNode = " << eventVector[i].sourceNode << endl;
		cout << "eventVector[" << i << "].destinationNode = " << eventVector[i].destinationNode << endl;
		cout << "eventVector[" << i << "].bandwidth = " << eventVector[i].bandwidth << endl;
		cout << "eventVector[" << i << "].action = " << eventVector[i].action << endl;
		cout << "**************************\n";
	}
}