#include <iostream>
#include <string>
#include <vector>
#include "graph.h"

using namespace std;

node::node(int i) //Constructor take id and assing to variable
{
    visited = false;
    id = i;
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
}

void node::updateNumOfLinks(int n)
{
    numOfLinks = n;
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

Graph::Graph(int n)
{
    numOfNodes = n;

    for(int i = 0; i < n ; i++)//create nodes object and push into vector
    {
        node tempNode(i);
        nodes.push_back(tempNode);
    }

}

void Graph::constructGraph(vector<vector<int>> &adjacencyMetrix)
{
    for(int i=0;i<numOfNodes;i++) 
    {
        int linksConnected = 0;
        for(int j=0;j<numOfNodes;j++)
        {
            if(adjacencyMetrix[i][j]) //If value is not zero then new link will added to the relevant node
            {
                nodes[i].addLinks(&nodes[j],adjacencyMetrix[i][j],40);
                linksConnected++;
            }
        }

        nodes[i].updateNumOfLinks(linksConnected);
    }
}


void Graph::printGraph()
{
    for(int i=0;i<numOfNodes;i++)
    {
        cout<<endl<<endl<<"node "<<nodes[i].getId()<<endl;
        nodes[i].printLinks();

    }
}