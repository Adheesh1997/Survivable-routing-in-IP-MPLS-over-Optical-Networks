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

    if (dis[end].value != intMAX && dis[end].value != 0) {
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
vector<vector<int>> Graph_DG::conditionAppling() {
    vector<vector<int>> totalPathDetails;
    for (int i = 0; i != this->vexnum; i++) {
        if (dis[i].value != intMAX && dis[i].value != 0) { //path exist
            dis[i].path.insert(dis[i].path.begin(), 1);
        }
        else { // path not exist
            //cout << "There is no shortest path" << endl;
            dis[i].path.insert(dis[i].path.begin(), 0);
        }
        totalPathDetails.push_back(dis[i].path);
    }
    return totalPathDetails;
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

        if (shortest_path.size() < minVal && shortest_path.size() != 1) {
            minVal = shortest_path.size();
            primarySPOutput.clear();
            primarySPOutput = shortest_path;
            waveLengthNumber = start;
        }
    }

    if (primarySPOutput.size() == 0) { //if primary LSP can't create
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
            if (mapIt->second.size() < minVal && mapIt->second.size() != 1) {
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
        if (backUpSPOutput.size()==0) {
            shortestPathsDetails.canCreatBP = false;
            totalPathDetails.clear();
            return shortestPathsDetails;
        }
        else {
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
}



findPathDetails startingPoint(int vexnum, vector<waveLengthNetworks> waveLengthNetwork, int source, int destination, vector<vector<int>> adjMetrixForPrimaryLSP) {


    if (adjMetrixForPrimaryLSP[source][destination] == 0 && adjMetrixForPrimaryLSP[destination][source] == 0) {
        findPathDetails pathDetails = initialize(vexnum, waveLengthNetwork, source, destination);
        pathDetails.alreadyPPhave = false;
        return pathDetails;
    }
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

        if (shortest_path.size() < minVal && shortest_path.size() != 1) {
            minVal = shortest_path.size();
            primarySPOutput.clear();
            primarySPOutput = shortest_path;
            waveLengthNumber = start;
        }
    }

    if (primarySPOutput.size() == 0) { //if primary LSP can't create
        shortestPathsDetails.tempCanCreatPP = false;
        shortestPathsDetails.alreadyPPhave = true;
        totalPathDetails.clear();
        return shortestPathsDetails;
    }
    else { //if primary LSP can create

        shortestPathsDetails.tempCanCreatPP = true;
        shortestPathsDetails.alreadyPPhave = true;
        shortestPathsDetails.tempPrimaryShortPath = primarySPOutput;
        shortestPathsDetails.tempWavelengthNoPP = waveLengthNumber;
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
            if (mapIt->second.size() < minVal && mapIt->second.size() != 1) {
                bool validate = true;
                for (vector<int>::iterator itPP = primarySPOutput.begin(); itPP != primarySPOutput.end() - 1; ++itPP) {
                    vector<int>::iterator itTemp;
                    itTemp = search(mapIt->second.begin(), mapIt->second.end(), itPP, itPP + 2);
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
        if (backUpSPOutput.size() == 0) {
            shortestPathsDetails.tempCanCreatBP = false;
            totalPathDetails.clear();
            return shortestPathsDetails;
        }
        else {
            for (auto j = 0; j < backUpSPOutput.size() - 1; j++)
            {
                waveLengthNetwork[waveLengthNumber].removeLink(backUpSPOutput[j], backUpSPOutput[j + 1]);
            }
            shortestPathsDetails.tempCanCreatBP = true;
            shortestPathsDetails.tempBackUpShortPath = backUpSPOutput;
            shortestPathsDetails.tempWavelengthNoBP = waveLengthNumber;
            totalPathDetails.clear();
            return shortestPathsDetails;
            //return shortest_path_output;
        }
    }
}

forRemainingBackUpPath createRemaing(int vexnum, vector<waveLengthNetworks> waveLengthNetwork, int source, int destination, map<int,vector<vector<int>>> FS, map<int,vector<vector<int>>>TD, vector<int> PPD) {
    forRemainingBackUpPath combinationDetails;
    map<int, vector<vector<int>>> fromSource;

    for (auto start = 0; start < waveLengthNetwork.size(); start++) {
        Graph_DG graph(vexnum, waveLengthNetwork[start].waveAdjacancyMatrix);//graph.print();
        graph.Dijkstra(source);
        fromSource[start] = graph.conditionAppling(); // first Shortest path
    }
    
    fromSource.insert(FS.begin(), FS.end());

    //for (int start = 0; start != fromSource.size(); start++) {
    //    cout << " 00000000000000000000000000 " << start << " 0000000000000000000000000" << endl;
    //    for (int i = 0; i < vexnum; i++) {
    //        for (std::vector<int>::iterator k = fromSource[start][i].begin(); k != fromSource[start][i].end(); ++k) {
    //            std::cout << *k << " --> ";
    //        }
    //        cout << endl;
    //    }
    //}

    map<int, vector<vector<int>>> fromDestination;

    for (auto start = 0; start < waveLengthNetwork.size(); start++) {
        Graph_DG graph(vexnum, waveLengthNetwork[start].waveAdjacancyMatrix);//graph.print();
        graph.Dijkstra(destination);
        fromDestination[start] = graph.conditionAppling(); // first Shortest path
    }
    fromDestination.insert(TD.begin(), TD.end());

    //for (int start = 0; start != fromDestination.size(); start++) {
    //    cout << " **************************** " << start << " ***************************" << endl;
    //    for (int i = 0; i < vexnum; i++) {
    //        for (std::vector<int>::iterator k = fromDestination[start][i].begin(); k != fromDestination[start][i].end(); ++k) {
    //            std::cout << *k << " --> ";
    //        }
    //        cout << endl;
    //    }
    //}
    int minumuVal = 10000;
   
    //fromSource[sourceWavelegth][intermediateNode].pop_back();
        vector<int> sourcePathBP;
        vector<int> destinationPathPathBP;
        int w1BP;
        int w2BP;
        int intNode;
        minumuVal = 10000;
        int xxx = fromSource.size();
        for (int startsourse = 0; startsourse < fromSource.size(); startsourse++) {
            for (int startdesti = 0; startdesti < fromSource.size(); startdesti++) {
                for (int i = 0; i < vexnum; i++) {
                    if (fromSource[startsourse][i][0] == 1 && fromDestination[startdesti][i][0] == 1) {

                        vector<int> tempVecDes = fromDestination[startdesti][i];
                        vector<int> tempVecSource = fromSource[startsourse][i];
                        tempVecDes.erase(tempVecDes.begin());
                        tempVecSource.erase(tempVecSource.begin());
                        tempVecDes.pop_back();
                        reverse(tempVecDes.begin(), tempVecDes.end());
                        tempVecDes.insert(tempVecDes.begin(), tempVecSource.begin(), tempVecSource.end());
                        if (tempVecDes.size() < minumuVal) {
                            bool validate = true;
                            for (vector<int>::iterator itPP = PPD.begin(); itPP != PPD.end() - 1; ++itPP) {
                                vector<int>::iterator itTemp;
                                itTemp = search(tempVecDes.begin(), tempVecDes.end(), itPP, itPP + 2);
                                if (itTemp != tempVecDes.end()) {
                                    validate = false;
                                }
                            }
                            if (validate == true) {
                                destinationPathPathBP = fromDestination[startdesti][i];
                                sourcePathBP = fromSource[startsourse][i];
                                intNode = i;
                                w1BP = startsourse;
                                w2BP = startdesti;
                                minumuVal = tempVecDes.size();
                            }
                        }
                    }
                }
            }
        }
        if (minumuVal != 10000) {
            destinationPathPathBP.erase(destinationPathPathBP.begin());
            sourcePathBP.erase(sourcePathBP.begin());
            reverse(destinationPathPathBP.begin(), destinationPathPathBP.end());

            combinationDetails.canCreatCombinationBP = true;
            combinationDetails.connectingNodeBP = intNode;
            combinationDetails.w1ShortPathBP = sourcePathBP;
            combinationDetails.w2ShortPathBP = destinationPathPathBP;
            combinationDetails.wavelengthNo1BP = w1BP;
            combinationDetails.wavelengthNo2BP = w2BP;

            if (w1BP < waveLengthNetwork.size()) 
            {
                for (auto j = 0; j < sourcePathBP.size() - 1; j++)
                {
                    waveLengthNetwork[w1BP].removeLink(sourcePathBP[j], sourcePathBP[j + 1]);
                }
            }
            else {
                fromSource[w1BP][intNode].clear();
                fromSource[w1BP][intNode].push_back(0);
            }
            if (w2BP < waveLengthNetwork.size()) {
                for (auto j = 0; j < destinationPathPathBP.size() - 1; j++)
                {
                    waveLengthNetwork[w2BP].removeLink(destinationPathPathBP[j], destinationPathPathBP[j + 1]);
                }
            }
            else {
                fromDestination[w2BP][intNode].clear();
                fromDestination[w2BP][intNode].push_back(0);
            }
            

            return combinationDetails;

        }
        else {
            combinationDetails.canCreatCombinationBP = false;
            return combinationDetails;
        }
}

combineWavelength pathCombinationCreat(int vexnum, vector<waveLengthNetworks> waveLengthNetwork, int source, int destination, map<int, vector<vector<int>>> FS, map<int, vector<vector<int>>>TD) {
    combineWavelength combinationDetails;
    map<int, vector<vector<int>>> fromSource;

    for (auto start = 0; start < waveLengthNetwork.size(); start++) {

        Graph_DG graph(vexnum, waveLengthNetwork[start].waveAdjacancyMatrix);//graph.print();
        graph.Dijkstra(source);
        fromSource[start] = graph.conditionAppling(); // first Shortest path
    }
    fromSource.insert(FS.begin(), FS.end());

    /*for (int start = 0; start != fromSource.size(); start++) {
        cout << " 00000000000000000000000000 " << start << " 0000000000000000000000000" << endl;
        for (int i = 0; i < vexnum; i++) {
            for (std::vector<int>::iterator k = fromSource[start][i].begin(); k != fromSource[start][i].end(); ++k) {
                std::cout << *k << " --> ";
            }
            cout << endl;
        }
    }*/

    map<int, vector<vector<int>>> fromDestination;

    for (auto start = 0; start < waveLengthNetwork.size(); start++) {
        Graph_DG graph(vexnum, waveLengthNetwork[start].waveAdjacancyMatrix);//graph.print();
        graph.Dijkstra(destination);
        fromDestination[start] = graph.conditionAppling(); // first Shortest path
    }
    fromDestination.insert(TD.begin(), TD.end());

    //for (int start = 0; start != fromDestination.size(); start++) {
    //    cout << " **************************** " << start << " ***************************" << endl;
    //    for (int i = 0; i < vexnum; i++) {
    //        for (std::vector<int>::iterator k = fromDestination[start][i].begin(); k != fromDestination[start][i].end(); ++k) {
    //            std::cout << *k << " --> ";
    //        }
    //        cout << endl;
    //    }
    //}
    int minumuVal = 10000;
    int sourceWavelegth;
    int destinationwavelength;
    int intermediateNode;

    int fromSourceSize = fromSource[16][3].size();
    for (int startsourse = 0; startsourse != fromSource.size(); startsourse++) {
        for (int startdesti = 0; startdesti != fromSource.size(); startdesti++) {
            for (int i = 0; i < vexnum; i++) {
                if (fromSource[startsourse][i][0] == 1 && fromDestination[startdesti][i][0]==1) {
                    int total = fromSource[startsourse][i].size() + fromDestination[startdesti][i].size();
                    if (total < minumuVal) {
                        minumuVal = total;
                        sourceWavelegth = startsourse;
                        destinationwavelength = startdesti;
                        intermediateNode = i;
                    }
                }
            }
        }
    }


    //fromSource[sourceWavelegth][intermediateNode].pop_back();
    if (minumuVal != 10000) {
        fromDestination[destinationwavelength][intermediateNode].erase(fromDestination[destinationwavelength][intermediateNode].begin());
        fromSource[sourceWavelegth][intermediateNode].erase(fromSource[sourceWavelegth][intermediateNode].begin());
        reverse(fromDestination[destinationwavelength][intermediateNode].begin(), fromDestination[destinationwavelength][intermediateNode].end());
        
        combinationDetails.canCreatCombinationPP = true;
        combinationDetails.connectingNodePP = intermediateNode;
        combinationDetails.wavelengthNo1PP = sourceWavelegth;
        combinationDetails.wavelengthNo2PP = destinationwavelength;
        combinationDetails.w1ShortPathPP = fromSource[sourceWavelegth][intermediateNode];
        combinationDetails.w2ShortPathPP = fromDestination[destinationwavelength][intermediateNode];
        
        if (sourceWavelegth < waveLengthNetwork.size()) {
            for (auto j = 0; j < fromSource[sourceWavelegth][intermediateNode].size() - 1; j++)
            {
                waveLengthNetwork[sourceWavelegth].removeLink(fromSource[sourceWavelegth][intermediateNode][j], fromSource[sourceWavelegth][intermediateNode][j + 1]);
            }
        }
        else {
            fromSource[sourceWavelegth][intermediateNode].clear();
            fromSource[sourceWavelegth][intermediateNode].push_back(0);
        }
        if (destinationwavelength < waveLengthNetwork.size()) {
            for (auto j = 0; j < fromDestination[destinationwavelength][intermediateNode].size() - 1; j++)
            {
                waveLengthNetwork[destinationwavelength].removeLink(fromDestination[destinationwavelength][intermediateNode][j], fromDestination[destinationwavelength][intermediateNode][j + 1]);
            }
        }
        else {
            fromDestination[destinationwavelength][intermediateNode].clear();
            fromDestination[destinationwavelength][intermediateNode].push_back(0);
        }

        vector<int> PPS = fromSource[sourceWavelegth][intermediateNode];
        vector<int> PPD = fromDestination[destinationwavelength][intermediateNode];
        PPS.pop_back();
        PPD.insert(PPD.begin(), PPS.begin(), PPS.end());

        if(sourceWavelegth < waveLengthNetwork.size())
        {
            Graph_DG graph(vexnum, waveLengthNetwork[sourceWavelegth].waveAdjacancyMatrix);//graph.print();
            graph.Dijkstra(source);
            fromSource[sourceWavelegth] = graph.conditionAppling();
        }
        
        if(destinationwavelength < waveLengthNetwork.size())
        {
            Graph_DG graph1(vexnum, waveLengthNetwork[destinationwavelength].waveAdjacancyMatrix);//graph.print();
            graph1.Dijkstra(destination);
            fromDestination[destinationwavelength] = graph1.conditionAppling();
        }
        

        vector<int> sourcePathBP;
        vector<int> destinationPathPathBP;
        int w1BP;
        int w2BP;
        int intNode;
        minumuVal = 10000;
        for (int startsourse = 0; startsourse != fromSource.size(); startsourse++) {
            for (int startdesti = 0; startdesti != fromSource.size(); startdesti++) {
                for (int i = 0; i < vexnum; i++) {
                    if (fromSource[startsourse][i][0] == 1 && fromDestination[startdesti][i][0] == 1) {

                        vector<int> tempVecDes = fromDestination[startdesti][i];
                        vector<int> tempVecSource = fromSource[startsourse][i];
                        tempVecDes.erase(tempVecDes.begin());
                        tempVecSource.erase(tempVecSource.begin());
                        tempVecDes.pop_back();
                        reverse(tempVecDes.begin(), tempVecDes.end());
                        tempVecDes.insert(tempVecDes.begin(), tempVecSource.begin(), tempVecSource.end());
                            if (tempVecDes.size() < minumuVal) {
                                bool validate = true;
                                for (vector<int>::iterator itPP = PPD.begin(); itPP != PPD.end() - 1; ++itPP) {
                                    vector<int>::iterator itTemp;
                                    itTemp = search(tempVecDes.begin(), tempVecDes.end(), itPP, itPP + 2);
                                    if (itTemp != tempVecDes.end()) {
                                        validate = false;
                                    }
                                }
                                if (validate == true) {
                                    destinationPathPathBP = fromDestination[startdesti][i];
                                    sourcePathBP = fromSource[startsourse][i];
                                    intNode = i;
                                    w1BP = startsourse;
                                    w2BP = startdesti;
                                    minumuVal = tempVecDes.size();
                                }
                            }
                    }
                }
            }
        }
        if (minumuVal != 10000) {
            destinationPathPathBP.erase(destinationPathPathBP.begin());
            sourcePathBP.erase(sourcePathBP.begin());
            reverse(destinationPathPathBP.begin(), destinationPathPathBP.end());

            combinationDetails.canCreatCombinationBP = true;
            combinationDetails.connectingNodeBP = intNode;
            combinationDetails.w1ShortPathBP = sourcePathBP;
            combinationDetails.w2ShortPathBP = destinationPathPathBP;
            combinationDetails.wavelengthNo1BP = w1BP;
            combinationDetails.wavelengthNo2BP = w2BP;

            if (w1BP < waveLengthNetwork.size()) {
                for (auto j = 0; j < sourcePathBP.size() - 1; j++)
                {
                    waveLengthNetwork[w1BP].removeLink(sourcePathBP[j], sourcePathBP[j + 1]);
                }
            }
            else {
                fromSource[w1BP][intNode].clear();
                fromSource[w1BP][intNode].push_back(0);
            }
            if (w2BP < waveLengthNetwork.size()) {
                for (auto j = 0; j < destinationPathPathBP.size() - 1; j++)
                {
                    waveLengthNetwork[w2BP].removeLink(destinationPathPathBP[j], destinationPathPathBP[j + 1]);
                }
            }
            else {
                fromDestination[w2BP][intNode].clear();
                fromDestination[w2BP][intNode].push_back(0);
            }
            

            return combinationDetails;
        
        }
        else {
            combinationDetails.canCreatCombinationBP = false;
            return combinationDetails;
        }
    }
    else {
        combinationDetails.canCreatCombinationPP = false;
        return combinationDetails;
    }
}

forBackupLightpath createLightPathBackup(int vexnum,vector<int> heavylightpath, vector<waveLengthNetworks>& waveLengthNetwork, int source, int destination) {
    auto minVal = 1000;
    int waveLengthNumber = -1;
    forBackupLightpath shortestPathsDetails;
    vector<int> primarySPOutput;

    for (auto start = 0; start < waveLengthNetwork.size(); start++) {
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
        for (auto j = 0; j < primarySPOutput.size() - 1; j++)
        {
            waveLengthNetwork[waveLengthNumber].removeLink(primarySPOutput[j], primarySPOutput[j + 1]);
        }
        return shortestPathsDetails;
    }
}

moreOEOConvertion createMainGraph(int vexnum, vector<waveLengthNetworks> waveLengthNetwork, int source, int destination, map<int, map<int, vector<vector<int>>>> mapFromLPGraph) {

    moreOEOConvertion outputDetails;
    map<int, vector<vector<int>>> fromSource;
    map<int, map<int, vector<vector<int>>>> mapDetails;
    map<int, map<int, multimap <int, int>>> completeMapDetails;
    for (auto start = 0; start < waveLengthNetwork.size(); start++) {
        for (auto count = 0; count < vexnum; count++) {
            Graph_DG graph(vexnum, waveLengthNetwork[start].waveAdjacancyMatrix);//graph.print();
            graph.Dijkstra(count);
            mapDetails[start][count] = graph.conditionAppling(); // first Shortest path
        }
    }

    mapDetails.insert(mapFromLPGraph.begin(), mapFromLPGraph.end());

    vector<vector<int>> djGraph;
    for (int i = 0; i < vexnum; i++) {
        vector<int> v1;

        for (int j = 0; j < vexnum; j++) {
            v1.push_back(0);
        }
        djGraph.push_back(v1);
    }


    for (auto sourceCount = 0; sourceCount < vexnum; sourceCount++) {
        for (auto destinationCount = 0; destinationCount < vexnum; destinationCount++) {
            for (auto start = 0; start < mapDetails.size(); start++) {
                if (mapDetails[start][sourceCount][destinationCount][0] == 1) {
                    completeMapDetails[sourceCount][destinationCount].insert(pair <int, int>(mapDetails[start][sourceCount][destinationCount].size(), start));
                    int size = 200 + mapDetails[start][sourceCount][destinationCount].size();
                    if (djGraph[sourceCount][destinationCount] == 0) {
                        djGraph[sourceCount][destinationCount] = size;
                    }
                    else if (size < djGraph[sourceCount][destinationCount]) {
                        djGraph[sourceCount][destinationCount] = size;
                    }
                }
            }
        }
    }
    //cout << endl;
    //for (int i = 0; i < djGraph.size(); i++) {
    //    for (int j = 0; j < djGraph[i].size(); j++) {
    //        cout << djGraph[i][j] << " ";
    //    }
    // cout << endl;
    //}
    vector<int> primaryPathDetailsPP;
    Graph_DG graph(vexnum, djGraph);//graph.print();
    graph.Dijkstra(source);
    vector<int> shortest_pathPP = graph.print_path(source, destination); // first Shortest path
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
            }
            
            temp.pop_back();
            vector<int>::iterator it;
            it = primaryPathDetailsPP.end();
            primaryPathDetailsPP.insert(it, temp.begin(), temp.end());
        }
        primaryPathDetailsPP.push_back(destination);
        for (auto start = 0; start < waveLengthNetwork.size(); start++) {
            for (auto j = 0; j < primaryPathDetailsPP.size() - 1; j++)
            {
                waveLengthNetwork[start].removeLink(primaryPathDetailsPP[j], primaryPathDetailsPP[j + 1]);
            }
        }

        //mapDetails.clear();
        for (auto start = 0; start < waveLengthNetwork.size(); start++) {
            for (auto count = 0; count < vexnum; count++) {
                Graph_DG graph(vexnum, waveLengthNetwork[start].waveAdjacancyMatrix);//graph.print();
                graph.Dijkstra(count);
                mapDetails[start][count] = graph.conditionAppling(); // first Shortest path
            }
        }

        completeMapDetails.clear();

        for (int i = 0; i < vexnum; i++) {
            for (int j = 0; j < vexnum; j++) {
                djGraph[i][j] = 0;
            }
        }
        //for (int i = 0; i < djGraph.size(); i++) {
        //    for (int j = 0; j < djGraph[i].size(); j++)
        //        cout << djGraph[i][j] << " ";
        //    cout << endl;
        //}
        for (auto sourceCount = 0; sourceCount < vexnum; sourceCount++) {
            for (auto destinationCount = 0; destinationCount < vexnum; destinationCount++) {
                for (auto start = 0; start < mapDetails.size(); start++) {
                    if (mapDetails[start][sourceCount][destinationCount][0] == 1) {

                        bool validate = true;
                        for (vector<int>::iterator itPP = primaryPathDetailsPP.begin(); itPP != primaryPathDetailsPP.end() - 1; ++itPP) {
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
                            }
                            else if (size < djGraph[sourceCount][destinationCount]) {
                                djGraph[sourceCount][destinationCount] = size;
                            }
                        }
                    }
                }
            }
        }

        Graph_DG graph(vexnum, djGraph);//graph.print();
        graph.Dijkstra(source);
        vector<int> shortest_pathBP = graph.print_path(source, destination); // first Shortest path
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
            return outputDetails;
        }
        return outputDetails;
    }
    else
    {
        outputDetails.canCreatePP = false;
        return outputDetails;


//complete
    }
}