#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

bool readCSVFile(int& numOfNodes, vector<vector<int>>& adjacencyMetrix,string fileLocation);

class node
{
private:
    int id; //Save the id of the node that unique from other nodes
    bool visited; //While searching or traveling state the this node is visited(true) or not(false)
    int numOfLinks; //Number of links that connected to this node
    struct link
    {
        node* address;
        int bandwith;
        int numOfLightpaths;
    };

    vector<link> links;
    
public:

    node(int i)
    {
        visited = false;
        id = i;
    }

    void addLinks(node* address, int bandwith, int numOfLightpaths = 1)
    {
        link templink;
        templink.address = address;
        templink.bandwith = bandwith;
        templink.numOfLightpaths = numOfLightpaths;
        
        links.push_back(templink);
    }

    void updateNumOfLinks(int n)
    {
        numOfLinks = n;
    }

};

class Graph
{
private:
    vector<node> nodes;
    int numOfNodes;
public:
    Graph(int n)
    {
        numOfNodes = n;

        for(int i = 0; i < n ; i++)
        {
            node tempNode(i);
            nodes.push_back(tempNode);
        }

    }

    void constructGraph(vector<vector<int>> &adjacencyMetrix)
    {
        for(int i=0;i<numOfNodes;i++) 
        {
            int linksConnected = 0;
            for(int j=0;j<numOfNodes;j++)
            {
               if(adjacencyMetrix[i][j])
               {
                   nodes[i].addLinks(&nodes[j],adjacencyMetrix[i][j]);
                   linksConnected++;
               }
            }

            nodes[i].updateNumOfLinks(linksConnected);
        }
    }
    


};





int main()
{
    int numOfNodes; //Variable to store number of nodes in the network

    vector<vector<int>> adjacencyMetrix;    //Vector to store adjacency metrix that represent netork

    string fileLocation = "Graph_inputs/03/graph03.csv"; //graph input file location
    
    if(readCSVFile(numOfNodes, adjacencyMetrix,fileLocation))//Read csv file and assign values to the matrix
    {
        for(int i=0;i<numOfNodes;i++) //Print the adjacency metrix
        {
            for(int j=0;j<numOfNodes;j++)
            {
                cout<<adjacencyMetrix[i][j]<<" ";
            }
            cout<<endl;
        }
    }
    

    return 0;
}

bool readCSVFile(int& numOfNodes, vector<vector<int>>& adjacencyMetrix,string fileLocation)
{
    ifstream myFileStream(fileLocation);

    if(myFileStream.is_open())
    {

        string line1;
        
        getline(myFileStream,line1);//Read first line of the csv file

        numOfNodes = stoi(line1);//Assign the value to the variable
        cout<<"Num of nodes = "<<numOfNodes<<endl<<endl;


        string rowOfMatrix;

        while(getline(myFileStream,rowOfMatrix)) //Read line and copy to rowOfMatrix
        {
            vector<int> temp;
            stringstream line(rowOfMatrix); //rowOfMatrix stream into line 
            string num;

            while(getline(line,num,',')) //Separate values from line and copy to num
            {
                temp.push_back(stoi(num)); //Convert num into integer and store in temprory vector 
            }

            adjacencyMetrix.push_back(temp); //Push temprary vector that contain a row of adjacency metrix to final vector
        }
        
        myFileStream.close(); //close the file
        return true; //If file read complete then return true
    }

    else    // If file does not open output error message and return from the function
    {
        cout<<"Graph input file failed to open!!!"<<endl;
        return false;
    }
}