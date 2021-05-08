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
    bool visited; //While searching or traveling stat, this node will state whether node is visited(true) or not(false)
    int numOfLinks; //Number of links that connected to this node
    
    struct link  //Structure that save all the information of a link that connected to this node
    {
        node* address;  //Address of the other node that connected to link
        int bandwith;  //Maximum bandwith of the link
        int numOfLightpaths; //Maximum light-path that can be created within this link
    };

    vector<link> links; //Vector of link (size = numOfLinks)
    
public:

    node(int i) //Constructor take id and assing to variable
    {
        visited = false;
        id = i;
    }

    //To add new link to the node this should call
    void addLinks(node* address, int bandwith, int numOfLightpaths = 40)
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

    void updateNumOfLinks(int n)
    {
        numOfLinks = n;
    }

    void printId()
    {
        cout<<" id of the node  = "<<links[0].address<<endl;
    }

};

class Graph
{
private:
    vector<node> nodes; //Vector of node (size = numOfNodes)
    int numOfNodes;
public:
    Graph(int n)
    {
        numOfNodes = n;

        for(int i = 0; i < n ; i++)//create nodes object and push into vector
        {
            node tempNode(i);
            nodes.push_back(tempNode);
        }

    }

    //Function that build graph using adjacency matrix
    void constructGraph(vector<vector<int>> &adjacencyMetrix)
    {
        for(int i=0;i<numOfNodes;i++) 
        {
            int linksConnected = 0;
            for(int j=0;j<numOfNodes;j++)
            {
               if(adjacencyMetrix[i][j]) //If value is not zero then new link will added to the relevant node
               {
                   nodes[i].addLinks(&nodes[j],adjacencyMetrix[i][j]);
                   linksConnected++;
               }
            }

            nodes[i].updateNumOfLinks(linksConnected);
        }
    }

    void testGraph()
    {
        nodes[0].printId();
    }


};





int main()
{
    int numOfNodes; //Variable to store number of nodes in the network
    vector<vector<int>> adjacencyMetrix; //Vector to store adjacency metrix that represent netork

    //graph input file location
    string fileLocation = "Graph_inputs/03/graph03.csv"; 
    
    //Read csv file and assign values to the matrix 
    if(readCSVFile(numOfNodes, adjacencyMetrix,fileLocation))
    {
        //If there is no any error while reading file then graph is created
        Graph graph1(numOfNodes);
        graph1.constructGraph(adjacencyMetrix);
        graph1.testGraph();

    }
    

    return 0;
}

bool readCSVFile(int& numOfNodes, vector<vector<int>>& adjacencyMetrix,string fileLocation)
{
    /*
    Parameters_
        1. numOfNodes - number of nodes that network has,that taken from csv file
        2. adjacencyMetrix - network input that taken from csv file
        3. fileLocation - location of the csv file that contain network input

    Return value_
        If file was read successfully and no any error then return true,
        If faild to read file return false 
    */

   ifstream myFileStream(fileLocation);//Create ifstream file object using file location

    //Check whether file is open without any error
    if(myFileStream.is_open())
    {
        
        string line1;  //String to take first line of csv(num.of nodes)
        
        getline(myFileStream,line1);  //Read first line of the csv file

        numOfNodes = stoi(line1);  //Convert string into interger and assign the value to the variable

        string rowOfMatrix;  //String to take line of the matrix

        //Read line and copy to rowOfMatrix
        while(getline(myFileStream,rowOfMatrix)) 
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
        return true; 
    }

    else    // If file does not open output error message and return from the function
    {
        cout<<"Graph input file failed to open!!!"<<endl;
        return false;
    }
}