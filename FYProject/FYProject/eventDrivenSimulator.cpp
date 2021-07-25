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
	cout << "/////////////////////////////////\n";

	while (!requestVector.empty())
	{
		double sum = 0;
		do
		{
			cout << "requestVector[0].sourceNode = " << requestVector[0].sourceNode << endl;
			cout << "requestVector[0].destinationNode = " << requestVector[0].destinationNode << endl;
			cout << "requestVector[0].holdingTime = " << requestVector[0].holdingTime << endl;
			cout << "requestVector[0].interArrivalTime = " << requestVector[0].interArrivalTime << endl << endl;
			sum += requestVector[0].interArrivalTime;

			cout << "sum = " << sum << endl;

			cout << "tempRequest[0].sourceNode = " << tempRequest[0].sourceNode << endl;
			cout << "tempRequest[0].destinationNode = " << tempRequest[0].destinationNode << endl;
			cout << "tempRequest[0].holdingTime = " << tempRequest[0].holdingTime << endl;
			cout << "tempRequest[0].remainingTime = " << tempRequest[0].remainingTime << endl;
			if (sum < tempRequest[0].remainingTime)
			{
				for (size_t j = 0; j < tempRequest.size(); j++)
				{
					tempRequest[j].remainingTime -= requestVector[0].interArrivalTime;
					cout << "**************************\n";
					cout << "tempRequest[j].sourceNode = " << tempRequest[j].sourceNode << endl;
					cout << "tempRequest[j].destinationNode = " << tempRequest[j].destinationNode << endl;
					cout << "tempRequest[j].holdingTime = " << tempRequest[j].holdingTime << endl;
					cout << "tempRequest[j].remainingTime = " << tempRequest[j].remainingTime << endl;
					cout << "**************************\n";
				}
				tempRequest.push_back(requestVector[0]);
				sort(tempRequest.begin(), tempRequest.end(), compareByLength);//sort tempRequest

				cout << endl << "requestVector[0].sourceNode = " << requestVector[0].sourceNode << endl;
				cout << "requestVector[0].destinationNode = " << requestVector[0].destinationNode << endl;
				cout << "requestVector[0].holdingTime = " << requestVector[0].holdingTime << endl;
				cout << "requestVector[0].interArrivalTime = " << requestVector[0].interArrivalTime << endl;

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
				cout << endl << "tempRequest[0].sourceNode = " << tempRequest[0].sourceNode << endl;
				cout << "tempRequest[0].destinationNode = " << tempRequest[0].destinationNode << endl;
				cout << "tempRequest[0].bandwidth = " << tempRequest[0].bandwidth << endl << endl;

				//eventObject.identifier = tempRequest[0].identifier;
				eventObject.sourceNode = tempRequest[0].sourceNode;
				eventObject.destinationNode = tempRequest[0].destinationNode;
				eventObject.bandwidth = tempRequest[0].bandwidth;
				eventObject.action = 0; //remove the LSP connection
				eventVector.push_back(eventObject);

				for (size_t j = 1; j < tempRequest.size(); j++)
				{
					tempRequest[j].remainingTime -= tempRequest[0].remainingTime;
					cout << "**************************\n";
					cout << "tempRequest[j].sourceNode = " << tempRequest[j].sourceNode << endl;
					cout << "tempRequest[j].destinationNode = " << tempRequest[j].destinationNode << endl;
					cout << "tempRequest[j].holdingTime = " << tempRequest[j].holdingTime << endl;
					cout << "tempRequest[j].remainingTime = " << tempRequest[j].remainingTime << endl;
					cout << "**************************\n";
				}

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
			for (size_t i = 0; i < tempRequest.size(); i++)
			{
				cout << endl << "tempRequest[i].sourceNode = " << tempRequest[i].sourceNode << endl;
				cout << "tempRequest[i].destinationNode = " << tempRequest[i].destinationNode << endl;
				cout << "tempRequest[i].holdingTime = " << tempRequest[i].holdingTime << endl;
				cout << "tempRequest[i].remainingTime = " << tempRequest[i].remainingTime << endl << endl;
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
		cout << "eventVector[i].sourceNode = " << eventVector[i].sourceNode << endl;
		cout << "eventVector[i].destinationNode = " << eventVector[i].destinationNode << endl;
		cout << "eventVector[i].bandwidth = " << eventVector[i].bandwidth << endl;
		cout << "eventVector[i].action = " << eventVector[i].action << endl;
		cout << "**************************\n";
	}
}