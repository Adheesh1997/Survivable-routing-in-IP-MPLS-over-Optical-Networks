#include"Dijkstra.h"
#include "graph.h"
#include <map>
#include <algorithm>   



//Constructor
Graph_DG::Graph_DG(int vexnum, vector<vector<int>> inputVector) {
    //Initialize the number of vertices and edges
    this->nullRepresenter.push_back(-1);
    this->vexnum = vexnum;
    this->arc = inputVector;
    dis = new Dis[this->vexnum];
    for (int i = 0; i < this->vexnum; i++) {
        for (int j = 0; j < this->vexnum; j++) {
            if (arc[i][j] == 0) {
                arc[i][j] = intMAX;
            }
        }
    }
}

//Destructor
Graph_DG::~Graph_DG() {
    delete[] dis;
    this->arc.clear();
}


void Graph_DG::print() {
    cout << "The adjacency matrix of the graph is:" << endl;
    int count_row = 0; //Print the label of the line
    int count_col = 0; //Print the label of the column
    //Start printing
    while (count_row != this->vexnum) {
        count_col = 0;
        while (count_col != this->vexnum) {
            if (arc[count_row][count_col] == intMAX)
                cout << "#" << " ";
            else
                cout << arc[count_row][count_col] << " ";
            ++count_col;
        }
        cout << endl;
        ++count_row;
    }
}

void Graph_DG::Dijkstra(int begin) {
    //First initialize our dis array
    int i;
    for (i = 0; i < this->vexnum; i++) {
        //Set the current path
        dis[i].path.push_back(begin);
        dis[i].path.push_back(i);
        //dis[i].path = "v" + to_string(begin) + "-->v" + to_string(i + 1);
        dis[i].value = arc[begin][i];
    }
    //Set the path from the starting point to the starting point to 0
    dis[begin].value = 0;
    dis[begin].visit = true;

    int count = 0;
    //Calculate the shortest path of the remaining vertices (this->vexnum-1 remaining vertices)
    while (count != this->vexnum) {
        //temp is used to save the smallest subscript in the current dis array
        //The current minimum value recorded by min
        int temp = 0;
        int min = intMAX;
        for (i = 0; i < this->vexnum; i++) {
            if (!dis[i].visit && dis[i].value < min) {
                min = dis[i].value;
                temp = i;
            }
        }
        //cout << temp + 1 << "  "<<min << endl;
        //Add the vertex corresponding to temp to the set of shortest paths that have been found
        dis[temp].visit = true;
        ++count;
        for (i = 0; i < this->vexnum; i++) {
            //Note the condition here arc[temp][i]!=intMAX must be added, otherwise an overflow will occur, resulting in a program exception
            if (!dis[i].visit && arc[temp][i] != intMAX && (dis[temp].value + arc[temp][i]) < dis[i].value) {
                //If the newly obtained edge can affect other vertices for access, then update its shortest path and length

                //dis[i].path.insert(dis[i].path.end(),dis[temp].path.begin(),dis[temp].path.end());
                dis[i].path = dis[temp].path;
                dis[i].path.push_back(i);
                dis[i].value = dis[temp].value + arc[temp][i];

                //dis[i].path = dis[temp].path + "-->v" + to_string(i + 1);
            }
        }
    }

}
vector<int> Graph_DG::print_path(int begin, int end) {

    //string str;
    //str = "v" + to_string(begin);
    //cout << "To"<<str<<"The shortest path of the graph as the starting point is:" << endl;
    //for (int i = 0; i != this->vexnum; i++) {
    //    for (std::vector<int>::const_iterator k = dis[i].path.begin(); k != dis[i].path.end(); ++k) {
    //        std::cout <<"v"<< *k << " --> ";
    //    }
    //    if (dis[i].value != intMAX) {
    //        cout << " = " << dis[i].value << endl;
    //    }
    //    else {
    //        cout << "There is no shortest path" << endl;
    //    }
    //}

    if (dis[end].value != intMAX) {
        return dis[end].path;
    }
    else {
        return nullRepresenter;
    }

}

void Graph_DG::delete_edge(int end) {
    for (std::vector<int>::const_iterator k = dis[end].path.begin(); k != dis[end].path.end() - 1; ++k) {
        //std::cout << "v" << *k << " --> ";
        this->arc[*k][*(k) - 1] = intMAX;
    }
    delete[] dis;
    dis = new Dis[this->vexnum];
}




findPathDetails initialize(int vexnum, vector<waveLengthNetworks> waveLengthNetwork, int source, int destination) {

    auto minVal = 1000;
    int waveLengthNumber = -1;
    findPathDetails shortestPathsDetails;
    vector<int> primarySPOutput;
    vector<int> backUpSPOutput;
    map<int, vector<int>> totalPathDetails;



    for (auto start = 0; start < waveLengthNetwork.size(); start++) {
        Graph_DG graph(vexnum, waveLengthNetwork[start].waveAdjacancyMatrix);//graph.print();
        graph.Dijkstra(source);
        vector<int> shortest_path = graph.print_path(source, destination); // first Shortest path

        totalPathDetails[start] = shortest_path;

        if (shortest_path.size() < minVal) {
            minVal = shortest_path.size();
            primarySPOutput.clear();
            primarySPOutput = shortest_path;
            waveLengthNumber = start;
        }
    }

    if (primarySPOutput.size() == 1) { //if primary LSP can't create
        shortestPathsDetails.canCreatPP = false;
        totalPathDetails.clear();
        return shortestPathsDetails;
    }
    else { //if primary LSP can create

        shortestPathsDetails.canCreatPP = true;
        shortestPathsDetails.primaryShortPath = primarySPOutput;
        shortestPathsDetails.wavelengthNoPP = waveLengthNumber;
        minVal = 1000;

        for (auto j = 0; j < primarySPOutput.size() - 1; j++)
        {
            waveLengthNetwork[waveLengthNumber].removeLink(primarySPOutput[j], primarySPOutput[j + 1]);
        }

        Graph_DG graph(vexnum, waveLengthNetwork[waveLengthNumber].waveAdjacancyMatrix);//graph.print();
        graph.Dijkstra(source);
        vector<int> shortest_path = graph.print_path(source, destination); // first Shortest path

        totalPathDetails[waveLengthNumber] = shortest_path;

        for (map<int, vector<int>>::iterator mapIt = totalPathDetails.begin(); mapIt != totalPathDetails.end(); ++mapIt) {

            if (mapIt->second.size() < minVal) {
                bool validate = true;

                for (vector<int>::iterator itPP = primarySPOutput.begin(); itPP != primarySPOutput.end()-1; ++itPP) {
                    vector<int>::iterator itTemp;
                    itTemp = search(mapIt->second.begin(), mapIt->second.end(), itPP, itPP+2);
                    if (itTemp != mapIt->second.end()) {
                        validate = false;
                    }
                }
                if (validate == true) {
                    backUpSPOutput = mapIt->second;            
                    waveLengthNumber = mapIt->first;
                    minVal = mapIt->second.size();
                }
            }
        }

        if (backUpSPOutput.size()==1) {
            shortestPathsDetails.canCreatBP = false;
            totalPathDetails.clear();
            return shortestPathsDetails;
        }
        for (auto j = 0; j < backUpSPOutput.size() - 1; j++)
        {
            waveLengthNetwork[waveLengthNumber].removeLink(backUpSPOutput[j], backUpSPOutput[j + 1]);
        }

    shortestPathsDetails.canCreatBP = true;
    shortestPathsDetails.backUpShortPath = backUpSPOutput;
    shortestPathsDetails.wavelengthNoBP = waveLengthNumber;
    totalPathDetails.clear();
    return shortestPathsDetails;

        //return shortest_path_output;
    }
}