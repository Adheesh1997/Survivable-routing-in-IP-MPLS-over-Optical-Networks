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
    //cout << "The adjacency matrix of the graph is:" << endl;
    int count_row = 0; //Print the label of the line
    int count_col = 0; //Print the label of the column
    //Start printing
    while (count_row != this->vexnum) {
        count_col = 0;
        while (count_col != this->vexnum) {
            if (arc[count_row][count_col] == intMAX)
                cout << "#" << " ";
            else
                //cout << arc[count_row][count_col] << " ";
            ++count_col;
        }
        //cout << endl;
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
        ////cout << temp + 1 << "  "<<min << endl;
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
    if (dis[end].value != intMAX && dis[end].value != 0) {
        return dis[end].path;
    }
    else {
        return nullRepresenter;
    }

}

void Graph_DG::delete_edge(int end) {
    for (std::vector<int>::const_iterator k = dis[end].path.begin(); k != dis[end].path.end() - 1; ++k) {
        //std:://cout << "v" << *k << " --> ";
        this->arc[*k][*(k) - 1] = intMAX;
    }
    delete[] dis;
    dis = new Dis[this->vexnum];
}
vector<vector<int>> Graph_DG::conditionAppling() {
    vector<vector<int>> totalPathDetails;
    for (int i = 0; i != this->vexnum; i++) {
        if (dis[i].value != intMAX && dis[i].value != 0) { //path exist
            dis[i].path.insert(dis[i].path.begin(), 1);
        }
        else { // path not exist
            ////cout << "There is no shortest path" << endl;
            dis[i].path.insert(dis[i].path.begin(), 0);
        }
        totalPathDetails.push_back(dis[i].path);
    }
    return totalPathDetails;
}

vector<vector<int>> Graph_DG::conditionApplingForDefaultGraph() {
    vector<vector<int>> totalPathDetails;
    for (int i = 0; i != this->vexnum; i++) {
        totalPathDetails.push_back(dis[i].path);
    }
    return totalPathDetails;
}


void dijkstraForAllNodes(int &vexnum, vector<waveLengthNetworks> &waveLengthNetwork, map<int, map<int, vector<vector<int>>>> &mapDetails) {
     
        for (size_t start = 0; start < waveLengthNetwork.size(); start++) {
            for (size_t count = 0; count < vexnum; count++) {
                Graph_DG graph(vexnum, waveLengthNetwork[start].waveAdjacancyMatrix);//graph.print();
                graph.Dijkstra(count);
                mapDetails[start][count] = graph.conditionAppling(); // first Shortest path
            }
        }
}

void dijkstraForDefaultGraph(int& vexnum, waveLengthNetworks &defaulSubWaveNetworks, map<int, vector<vector<int>>>& mapDetails) {
        for (size_t count = 0; count < vexnum; count++) {
            Graph_DG graph(vexnum, defaulSubWaveNetworks.waveAdjacancyMatrix);//graph.print();
            graph.Dijkstra(count);
            mapDetails[count] = graph.conditionApplingForDefaultGraph(); // first Shortest path
        }
}

void dijkstraForOneNode(int& vexnum,int &source,int & destination, vector<vector<int>>& djGraph, vector<int> &shortest_path) {
    Graph_DG graph(vexnum, djGraph);//graph.print();
    graph.Dijkstra(source);
    shortest_path = graph.print_path(source, destination); // first Shortest path
}
void print(vector<int> vec) {
    for (int i : vec) {
        cout << i << ' ';
    }
    cout << endl;
}
void initialize(int& vexnum, vector<waveLengthNetworks>& waveLengthNetwork, int& source, int& destination, map<int, map<int, vector<vector<int>>>> mapFromLPGraph, moreOEOConvertion& outputDetails, map<int, vector<vector<int>>>& defaultGraphDetails) {
    size_t minVal = 10000;
    vector<int> primarySPOutput;
    vector<int> backUpSPOutput;
    int waveLengthNumber = -1;
    vector<int> shortest_path;
    bool foundCheck = true;
    for (size_t start = 0; start < waveLengthNetwork.size(); start++) {
        shortest_path = mapFromLPGraph[start + waveLengthNetwork.size()][source][destination];
        if (shortest_path.size() < minVal && shortest_path[0] == 1) {
            shortest_path.erase(shortest_path.begin());
            minVal = shortest_path.size();
            primarySPOutput = shortest_path;
            waveLengthNumber = start + waveLengthNetwork.size();
            if (minVal == defaultGraphDetails[source][destination].size()) {
                foundCheck = false;
                break;
            }
        }
    }
    if (foundCheck){
        for (size_t start = 0; start < waveLengthNetwork.size(); start++) {
            Graph_DG graph(vexnum, waveLengthNetwork[start].waveAdjacancyMatrix);//graph.print();
                graph.Dijkstra(source);
                shortest_path = graph.print_path(source, destination);// first Shortest path
                if (shortest_path.size() < minVal && shortest_path.size() != 1) {
                    minVal = shortest_path.size();
                        primarySPOutput = shortest_path;
                        waveLengthNumber = start;
                }
        }
    }
    if (primarySPOutput.size() == 0) { //if primary LSP can't create
        outputDetails.canCreatePP = false;
        outputDetails.canCreateBP = false;
    }
    else {

        outputDetails.canCreatePP = true;
        if (waveLengthNumber >= waveLengthNetwork.size()) {
            mapFromLPGraph[waveLengthNumber][source][destination].clear();
            mapFromLPGraph[waveLengthNumber][source][destination].push_back(0);
            mapFromLPGraph[waveLengthNumber][destination][source].clear();
            mapFromLPGraph[waveLengthNumber][destination][source].push_back(0);
        }
        outputDetails.allPathDetailsPP.push_back(primarySPOutput);
        outputDetails.wavelengthNumberPP.push_back(waveLengthNumber);
        minVal = 10000;
        for (size_t start = 0; start < waveLengthNetwork.size(); start++) {
            for (size_t j = 0; j < primarySPOutput.size() - 1; j++)
            {
                waveLengthNetwork[start].removeLink(primarySPOutput[j], primarySPOutput[j + 1]);
            }
        }
        for (int start = 2 * (waveLengthNetwork.size()) -1; start >= 0  ; start --) {
            if (start < waveLengthNetwork.size()){
                Graph_DG graph(vexnum, waveLengthNetwork[waveLengthNetwork.size() - (start +1)].waveAdjacancyMatrix);//graph.print();
                graph.Dijkstra(source);
                shortest_path = graph.print_path(source, destination);
            }
            if (start >= waveLengthNetwork.size()) {
                if (mapFromLPGraph[start][source][destination][0] == 1) {
                    shortest_path = mapFromLPGraph[start][source][destination];
                    shortest_path.erase(shortest_path.begin());
                    //print(shortest_path);
                }
                else {
                    shortest_path.clear();
                    shortest_path.push_back(-1);
                }
            }
            if (shortest_path.size() < minVal && shortest_path.size() != 1) {
                bool validate = true;
                for (vector<int>::iterator itPP = primarySPOutput.begin(); itPP != primarySPOutput.end()-1; ++itPP) {
                    vector<int>::iterator itTemp;
                    itTemp = search(shortest_path.begin(), shortest_path.end(), itPP, itPP+2);
                    if (itTemp != shortest_path.end()) {
                        validate = false;
                    }
                }
                if (validate == true) {
                    backUpSPOutput = shortest_path;
                    if (start < waveLengthNetwork.size()) {
                        waveLengthNumber = waveLengthNetwork.size() -  (start+1);
                    }
                    else {
                        waveLengthNumber = start;
                    }
                    minVal = shortest_path.size();
                    if (minVal == defaultGraphDetails[source][destination].size()) {
                        foundCheck = false;
                        break;
                    }
                }
            }
        }
        if (backUpSPOutput.size()==0) {
            outputDetails.canCreateBP = false;
        }
        else {
            outputDetails.canCreateBP = true;
            outputDetails.allPathDetailsBP.push_back(backUpSPOutput);
            outputDetails.wavelengthNumberBP.push_back(waveLengthNumber);
        }
    }
}

forBackupLightpath createLightPathBackup(int vexnum,vector<int> heavylightpath, vector<waveLengthNetworks>& waveLengthNetwork, int source, int destination) {
    int minVal = 1000;
    int waveLengthNumber = -1;
    forBackupLightpath shortestPathsDetails;
    vector<int> primarySPOutput;

    for (size_t start = 0; start < waveLengthNetwork.size(); start++) {
        Graph_DG graph(vexnum, waveLengthNetwork[start].waveAdjacancyMatrix);//graph.print();
        graph.Dijkstra(source);
        vector<int> shortest_path = graph.print_path(source, destination); // first Shortest path

        if (shortest_path.size() < minVal && shortest_path.size() != 1) {

                bool validate = true;
                for (vector<int>::iterator itPP = heavylightpath.begin(); itPP != heavylightpath.end() - 1; ++itPP) {
                    vector<int>::iterator itTemp;
                    itTemp = search(shortest_path.begin(), shortest_path.end(), itPP, itPP + 2);
                    if (itTemp != shortest_path.end()) {
                        validate = false;
                    }
                }
                if (validate == true) {
                    primarySPOutput.clear();
                    primarySPOutput = shortest_path;
                    waveLengthNumber = start;
                    minVal = shortest_path.size();
                }
        }
    }
    if (primarySPOutput.size() == 0) {
        shortestPathsDetails.canCreatBLPath = false;
        return shortestPathsDetails;
    }
    else {
        shortestPathsDetails.canCreatBLPath = true;
        shortestPathsDetails.wavelengthBLPath = primarySPOutput;
        shortestPathsDetails.wavelengthBLPNo = waveLengthNumber;
        for (size_t j = 0; j < primarySPOutput.size() - 1; j++)
        {
            waveLengthNetwork[waveLengthNumber].removeLink(primarySPOutput[j], primarySPOutput[j + 1]);
        }
        return shortestPathsDetails;
    }
}

void createBackUpPathMainGraph(int& vexnum, vector<waveLengthNetworks>& waveLengthNetwork, int& source, int& destination, map<int, map<int, vector<vector<int>>>>& mapDetails, vector<int>& primaryPath, moreOEOConvertion& outputDetails, map<int, vector<vector<int>>>& defaultGraphDetails) {

    map<int, map<int, map <int, int>>> completeMapDetails;
    dijkstraForAllNodes(vexnum, waveLengthNetwork, mapDetails);
    vector<vector<int> > djGraph(vexnum, vector<int>(vexnum, 0));
    for (size_t sourceCount = 0; sourceCount < vexnum; sourceCount++) {
        for (size_t destinationCount = 0; destinationCount < vexnum; destinationCount++) {
            for (int start = mapDetails.size() - 1; start >= 0; start--) {
                if (mapDetails[start][sourceCount][destinationCount][0] == 1) {
                    bool validate = true;
                    for (vector<int>::iterator itPP = primaryPath.begin(); itPP != primaryPath.end() - 1; ++itPP) {
                        vector<int>::iterator itTemp;
                        itTemp = search(mapDetails[start][sourceCount][destinationCount].begin(), mapDetails[start][sourceCount][destinationCount].end(), itPP, itPP + 2);
                        if (itTemp != mapDetails[start][sourceCount][destinationCount].end()) {
                            validate = false;
                        }
                    }
                    if (validate == true) {
                        completeMapDetails[sourceCount][destinationCount].insert(pair <int, int>(mapDetails[start][sourceCount][destinationCount].size(), start));
                        int size = 200 + mapDetails[start][sourceCount][destinationCount].size();
                        if (djGraph[sourceCount][destinationCount] == 0) {
                            djGraph[sourceCount][destinationCount] = size;
                            if (mapDetails[start][sourceCount][destinationCount].size() - 1 == defaultGraphDetails[sourceCount][destinationCount].size()) {
                                start = -1;
                            }
                        }
                        else if (size <= djGraph[sourceCount][destinationCount]) {
                            djGraph[sourceCount][destinationCount] = size;
                            if (mapDetails[start][sourceCount][destinationCount].size() - 1 == defaultGraphDetails[sourceCount][destinationCount].size()) {
                                start = -1;
                            }
                        }
                    }
                }
            }
        }
    }
    vector<int> shortest_pathBP;
    dijkstraForOneNode(vexnum, source, destination, djGraph, shortest_pathBP);
    if (shortest_pathBP.size() != 1) {
        outputDetails.canCreateBP = true;
        for (auto ptr = shortest_pathBP.begin(); ptr < shortest_pathBP.end() - 1; ptr++) {
            vector<int> temp = mapDetails[completeMapDetails[*ptr][*(ptr + 1)].begin()->second][*ptr][*(ptr + 1)];
            temp.erase(temp.begin());
            outputDetails.allPathDetailsBP.push_back(temp);
            outputDetails.wavelengthNumberBP.push_back(completeMapDetails[*ptr][*(ptr + 1)].begin()->second);
        }
    }
    else {
        outputDetails.canCreateBP = false;
    }

}


moreOEOConvertion createMainGraph(int vexnum, vector<waveLengthNetworks> waveLengthNetwork, int source, int destination, map<int, map<int, vector<vector<int>>>> mapFromLPGraph, map<int, vector<vector<int>>> &defaultGraphDetails) {
    map<int, map<int, vector<vector<int>>>> mapDetails;
    moreOEOConvertion outputDetails;
    map<int, vector<vector<int>>> fromSource;
    map<int, map<int, map <int, int>>> completeMapDetails;
    initialize(vexnum, waveLengthNetwork, source, destination, mapFromLPGraph, outputDetails, defaultGraphDetails);
    if (outputDetails.canCreatePP && outputDetails.canCreateBP) {

        return outputDetails;
    }
    if (outputDetails.canCreatePP && !outputDetails.canCreateBP) {
        vector<int> shortest_path = outputDetails.allPathDetailsPP[0];
        mapDetails.insert(mapFromLPGraph.begin(), mapFromLPGraph.end());
        createBackUpPathMainGraph(vexnum, waveLengthNetwork, source, destination, mapDetails, shortest_path, outputDetails, defaultGraphDetails);
        return outputDetails;
    }
    dijkstraForAllNodes(vexnum, waveLengthNetwork, mapDetails);
    mapDetails.insert(mapFromLPGraph.begin(), mapFromLPGraph.end());
    vector<vector<int> > djGraph(vexnum, vector<int>(vexnum, 0));
    for (size_t sourceCount = 0; sourceCount < vexnum; sourceCount++) {
        for (size_t destinationCount = 0; destinationCount < vexnum; destinationCount++) {
            for (int start = mapDetails.size() -1; start >= 0; start--) {
                if (mapDetails[start][sourceCount][destinationCount][0] == 1) {
                    completeMapDetails[sourceCount][destinationCount].insert(pair <int, int>(mapDetails[start][sourceCount][destinationCount].size(), start));
                    int size = 200 + mapDetails[start][sourceCount][destinationCount].size();
                    if (djGraph[sourceCount][destinationCount] == 0) {
                        djGraph[sourceCount][destinationCount] = size;
                        if (mapDetails[start][sourceCount][destinationCount].size() - 1 == defaultGraphDetails[sourceCount][destinationCount].size()) {
                            start = -1;
                        }
                    }
                    else if (size <= djGraph[sourceCount][destinationCount]) {
                        djGraph[sourceCount][destinationCount] = size;
                        if (mapDetails[start][sourceCount][destinationCount].size() - 1 == defaultGraphDetails[sourceCount][destinationCount].size()) {
                            start = -1;
                        }
                    }
                }
            }
        }
    }
    vector<int> primaryPathDetailsPP;
    vector<int> shortest_pathPP;
    dijkstraForOneNode(vexnum, source, destination, djGraph, shortest_pathPP);
    if (shortest_pathPP.size() != 1) {
        outputDetails.canCreatePP = true;
        for (auto ptr = shortest_pathPP.begin(); ptr < shortest_pathPP.end() - 1; ptr++) {
            vector<int> temp = mapDetails[completeMapDetails[*ptr][*(ptr + 1)].begin()->second][*ptr][*(ptr + 1)];
            temp.erase(temp.begin());
            outputDetails.allPathDetailsPP.push_back(temp);
            outputDetails.wavelengthNumberPP.push_back(completeMapDetails[*ptr][*(ptr + 1)].begin()->second);
            if (completeMapDetails[*ptr][*(ptr + 1)].begin()->second >= waveLengthNetwork.size()) {
                mapDetails[completeMapDetails[*ptr][*(ptr + 1)].begin()->second][*ptr][*(ptr + 1)].clear();
                mapDetails[completeMapDetails[*ptr][*(ptr + 1)].begin()->second][*ptr][*(ptr + 1)].push_back(0);
                mapDetails[completeMapDetails[*ptr][*(ptr + 1)].begin()->second][*(ptr + 1)][*ptr].clear();
                mapDetails[completeMapDetails[*ptr][*(ptr + 1)].begin()->second][*(ptr + 1)][*ptr].push_back(0);
            }
            
            temp.pop_back();
            vector<int>::iterator it;
            it = primaryPathDetailsPP.end();
            primaryPathDetailsPP.insert(it, temp.begin(), temp.end());
        }
        primaryPathDetailsPP.push_back(destination);
        for (size_t start = 0; start < waveLengthNetwork.size(); start++) {
            for (size_t j = 0; j < primaryPathDetailsPP.size() - 1; j++)
            {
                waveLengthNetwork[start].removeLink(primaryPathDetailsPP[j], primaryPathDetailsPP[j + 1]);
            }
        }
        createBackUpPathMainGraph(vexnum, waveLengthNetwork, source, destination, mapDetails, primaryPathDetailsPP, outputDetails, defaultGraphDetails);
        return outputDetails;
    }
    else
    {
        outputDetails.canCreatePP = false;
        return outputDetails;
    }
}
