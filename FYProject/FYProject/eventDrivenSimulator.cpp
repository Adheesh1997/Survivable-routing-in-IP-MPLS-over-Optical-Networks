#include "eventDrivenSimulator.h"
#include <stdlib.h>     
#include <time.h>
#include <random>

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

	for (size_t i = 0; i < 10; i++)
	{
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
}