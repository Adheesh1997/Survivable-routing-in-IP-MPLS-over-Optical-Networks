#include <iostream>
#include <string>
#include <vector>
#include "graph.h"

using namespace std;

/******************* Begining of node class *******************************/

node::node(int i) //Constructor take id and assing to variable
{
    id = i;
    numOfLinks = 0;
}


void node::addLinks(node* address, int bandwith, int numOfLightpaths )
{
    /* 
    Parameters_
        1. address - address of the other node that connect through the link
        2. bandwith - bandwith of the link
    */
    link templink;
    templink.address = address;
    templink.bandwith = bandwith;
    templink.numOfLightpaths = numOfLightpaths;
    
    links.push_back(templink);

    numOfLinks++;
}


int node::getId()
{
    return id;
}

void node::printLinks()
{
    cout<<"Links = "<<numOfLinks<<" ->";
    for(int i=0;i<numOfLinks;i++)
    {
        cout<<(links[i].address)->id<<", ";
    }
}

/*************** End of node class ***********************/



/***************** Begining of fiberLinkNetwork class *********************/
fiberLinkNetwork::fiberLinkNetwork(int n,int maxWave)
{
    numOfNodes = n;
    maxWaveLengths = maxWave;

    for(int i = 0; i < n ; i++)//create nodes object and push into vector
    {
        node tempNode(i);
        nodes.push_back(tempNode);
    }

}


//Connect all the nodes according to the adjacency metrix
void fiberLinkNetwork::setupFiberLinkNetwork(vector<vector<int>> &adjacencyMetrix)
{
    for(int i=0;i<numOfNodes;i++) 
    {
        for(int j=0;j<numOfNodes;j++)
        {
            if(adjacencyMetrix[i][j]) //If value is not zero then new link will added to the relevant node
            {
                nodes[i].addLinks(&nodes[j],maxWaveLengths,40);
            }
        }
    }
}


void fiberLinkNetwork::printGraph()
{
    for(int i=0;i<numOfNodes;i++)
    {
        cout<<endl<<endl<<"node "<<nodes[i].getId()<<endl;
        nodes[i].printLinks();

    }
}

/******************* End of fiberLinkNetwork Class ***********************/



//***************** Begining of wave length network Class ******************/

waveLengthNetworks::waveLengthNetworks(vector<vector<int>> initAdjacencyMatrix, int waveID)
{
    /* 
    Parameter_
        initAdjacencyMatrix - Initial adjacency matrix that taken from CSV file,
        waveID - wave length number that this network represent
    */
    waveAdjacancyMatrix = initAdjacencyMatrix;
    waveLengthId = waveID;
}

int waveLengthNetworks::getID()
{
    return waveLengthId;
}

void waveLengthNetworks::removeLink(int src, int dst)
{
    /*
    Parameter_
        src - Source node connected to link
        dst - Destinationo node connected to link
    */
   /* cout<<"\n Source : "<<src<<", Dst : "<<dst<<endl;
    waveAdjacancyMatrix[src][dst] = 0; */
    
}

void waveLengthNetworks::removeLinks(int src, int dst)
{
    /*
    Parameter_
        src - Source node connected to link
        dst - Destinationo node connected to link
    */
   //cout<<"\n Source : "<<src<<", Dst : "<<dst<<endl;
    waveAdjacancyMatrix[src][dst] = 0;
    
}

void waveLengthNetworks::addLInk(int src, int dst, int bandwidth)
{
    waveAdjacancyMatrix[src][dst] = bandwidth;
}

/************* end of waveLengthNetworks *******************/


// Seturp networks for different wavelengths
vector<waveLengthNetworks> setupWaveLengthNetworks(vector<vector<int>> initAdjacencyMatrix,int maxWaveLengths)
{
    vector<waveLengthNetworks> temp;

    for(int i =  0 ; i < maxWaveLengths ; i++)
    {
        waveLengthNetworks tempNetwork(initAdjacencyMatrix,i);
        temp.push_back(tempNetwork);
    }

    return temp;
}


