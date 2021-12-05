#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>

#include "lspRequestGenarator.h"


using namespace std;


lspRequestGenarator::lspRequestGenarator(int numberOfNode)
{
    numOfNode = numberOfNode;
    bandwithArray = vector<int>{1,2,3,5,7,9,10};
    lastId = 0;
    srand(time(NULL));
}

lspRequest lspRequestGenarator::generateRequest()
{
    lspRequest temp;

    //cout<<"\nsize = "<<reqArray.size()<<endl;
    int prob = rand() % 100;

    if(prob > 50 || reqArray.empty())
    {
        temp.srcNode = rand() % numOfNode;
        temp.DstNode = rand() % numOfNode;

        while(temp.srcNode == temp.DstNode) temp.DstNode = rand() % numOfNode;

        temp.bandwidthSize = bandwithArray[rand()  % bandwithArray.size()];

        temp.request = true;
        temp.id = lastId;
        lastId++;
        reqArray.push_back(temp);

        return temp;
    }
    
    temp = reqArray[rand() % reqArray.size()];
    temp.request = false;
    
    for(int i = 0; i < reqArray.size(); i++)
    {
        if(temp.id == reqArray[i].id)
        {
            reqArray.erase(reqArray.begin() + i);
            return temp; 
        }
    }
 
}
