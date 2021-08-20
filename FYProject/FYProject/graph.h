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
    fiberLinkNetwork(int n, int maxWavelegths);

    //Function that build graph using adjacency matrix
    void setupFiberLinkNetwork(vector<vector<int>> &adjacencyMetrix, int numOfWaves);

    void printGraph();

};


class waveLengthNetworks
{
private:
    int waveLengthId;
    int numOfWaves;
public:
    vector<vector<int>> waveAdjacancyMatrix;
    waveLengthNetworks(vector<vector<int>> initAdjacencyMatrix, int waveID);

    int getID();

    //To remove a link from adjacency matrix
    //void removeLinks(int src, int dst);
    void removeLink(int src, int dst);

    //Add a new link to adjacency matrix
    void addLInk(int src, int dst, int bandwidth = 40);
};

vector<waveLengthNetworks> setupWaveLengthNetworks(vector<vector<int>> initAdjacencyMatrix,int maxWaveLengths);


#endif