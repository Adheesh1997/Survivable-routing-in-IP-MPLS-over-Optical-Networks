#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//Node (router ) to save all the data 
class node
{
private:
    int id; //Save the id of the node that unique from other nodes
    
    int numOfLinks; //Number of links that connected to this node
    
    struct link  //Structure that save all the information of a link that connected to this node
    {
        node* address;  //Address of the other node that connected to link
        int bandwith;  //Maximum bandwith of the link
        int numOfLightpaths; //Maximum light-path that can be created within this link
    };

    vector<link> links; //Vector of link (size = numOfLinks)
    
public:
    node(int i); //Constructor take id and assing to variable
    
    //To add new link to the node this should call
    void addLinks(node* address, int bandwith, int numOfLightpaths);

    void updateNumOfLinks(int n);

    int getId();

    void printLinks();

};


class fiberLinkNetwork
{
private:
    vector<node> nodes; //Vector of node (size = numOfNodes)
    int numOfNodes;

public:
    int maxWaveLengths;
    fiberLinkNetwork(int n, int maxWavelegths = 40);

    //Function that build graph using adjacency matrix
    void setupFiberLinkNetwork(vector<vector<int>> &adjacencyMetrix);

    void printGraph();

};


class waveLengthNetworks
{
private:
    int waveLengthId;
    vector<vector<int>> waveAdjacancyMatrix;

public:
    waveLengthNetworks(vector<vector<int>> initAdjacencyMatrix, int waveID);
    vector<vector<int>> getMatrix();
    void removeLink(int src, int dst);
    void addLInk(int src, int dst);
};

vector<waveLengthNetworks> setupWaveLengthNetworks(vector<vector<int>> initAdjacencyMatrix,int maxWaveLengths);


#endif