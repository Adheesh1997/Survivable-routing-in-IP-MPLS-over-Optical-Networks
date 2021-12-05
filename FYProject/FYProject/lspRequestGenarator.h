/* #ifndef lspRequestGenarator_H
#define lspRequestGenarator_H */

#pragma once

#include <vector>

using namespace std;

struct lspRequest
{
    int srcNode,DstNode;
    bool request;
    int bandwidthSize;
    int id;
};

class lspRequestGenarator
{
private:
    vector<lspRequest> reqArray;
    vector<int> bandwithArray;
    int numOfNode;
    int lastId;

    
    

public:
    lspRequestGenarator(int numberOfNode);
    
    lspRequest generateRequest();

};





//#endif