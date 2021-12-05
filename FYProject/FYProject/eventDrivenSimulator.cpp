#include "eventDrivenSimulator.h"
#include <stdlib.h>     
#include <time.h>
#include <random>
#include <algorithm>
//////////////////////////
bool compareByLength(const requestDetails& a, const requestDetails& b)
{
	return a.remainingTime < b.remainingTime;
}

void requestCreation::requestGenerator(int numberOfLSPrequests, double erlang, double meanHoldingTime, int bandwidthCap)
{
	std::default_random_engine generator1, generator2, generator3;
	std::uniform_int_distribution<int> distribution1(0, 13), distribution2(0, 13), distribution3(1, bandwidthCap);

	requestDetails obj;
	srand(time(NULL));
	
	random_device rd;            // uniformly-distributed integer random number generator
	mt19937 rng(rd());           // mt19937: Pseudo-random number generation

	//double averageArrival = 15;
	//double erlang = 1 / averageArrival;
	//erlang = 10;
	exponential_distribution<double> exp(erlang);

	random_device rd2;           // uniformly-distributed integer random number generator
	mt19937 rng2(rd2());         // mt19937: Pseudo-random number generation

	exponential_distribution<double> exp2(meanHoldingTime);

	for (size_t i = 0; i < numberOfLSPrequests; i++)
	{
		obj.identifier = reqID++;
		obj.sourceNode = distribution1(generator1);
		do
		{
			obj.destinationNode = distribution2(generator2);
		} while (obj.destinationNode == obj.sourceNode);
		obj.bandwidth = distribution3(generator3);
		obj.interArrivalTime = exp.operator() (rng); // generates the next random number in the distribution 
		obj.holdingTime = exp2.operator() (rng2);    // generates the next random number in the distribution 
		obj.remainingTime = obj.holdingTime;


		requestVector.push_back(obj);
	}
}

void requestCreation::printLSPrequests()
{
	for (size_t i = 0; i < requestVector.size(); i++)
	{
		cout << "***************************\n";
		cout << "identifier = " << requestVector[i].identifier << endl;
		cout << "sourceNode = " << requestVector[i].sourceNode << endl;
		cout << "destinationNode = " << requestVector[i].destinationNode << endl;
		cout << "bandwidth = " << requestVector[i].bandwidth << endl;
		cout << "holdingTime = " << requestVector[i].holdingTime << endl;
		cout << "interArrivalTime = " << requestVector[i].interArrivalTime << endl;
	}
	cout << "***************************\n";
}

vector<events> requestCreation::eventCreation()
{
	events eventObject;
	eventObject.identifier = requestVector[0].identifier;
	eventObject.sourceNode = requestVector[0].sourceNode;
	eventObject.destinationNode = requestVector[0].destinationNode;
	eventObject.bandwidth = requestVector[0].bandwidth;
	eventObject.action = 1;   //establish the LSP connection
	eventVector.push_back(eventObject);

	vector<requestDetails> tempRequest;
	tempRequest.push_back(requestVector[0]);
	vector<requestDetails>::iterator it;

	it = requestVector.begin();
	requestVector.erase(it);   //remove this element from request Vector
	

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


				eventObject.identifier = requestVector[0].identifier;
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
				eventObject.identifier = tempRequest[0].identifier;
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

					eventObject.identifier = requestVector[0].identifier;
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
		eventObject.identifier = tempRequest[0].identifier;
		eventObject.sourceNode = tempRequest[0].sourceNode;
		eventObject.destinationNode = tempRequest[0].destinationNode;
		eventObject.bandwidth = tempRequest[0].bandwidth;
		eventObject.action = 0; //remove the LSP connection
		eventVector.push_back(eventObject);

		it = tempRequest.begin();
		tempRequest.erase(it);
	}
	return eventVector;
}

void requestCreation::printEvents()
{
	for (size_t i = 0; i < eventVector.size(); i++)
	{
		cout << "**************************\n";
		cout << "eventVector[" << i << "].identifier = " << eventVector[i].identifier << endl;
		cout << "eventVector[" << i << "].sourceNode = " << eventVector[i].sourceNode << endl;
		cout << "eventVector[" << i << "].destinationNode = " << eventVector[i].destinationNode << endl;
		cout << "eventVector[" << i << "].bandwidth = " << eventVector[i].bandwidth << endl;
		cout << "eventVector[" << i << "].action = " << eventVector[i].action << endl;
		cout << "**************************\n";
	}
}