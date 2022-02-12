#pragma once
//#pragma once is a commonly used C/C++ pragma,
//As long as this pragma is added at the beginning of the header file,
//You can ensure that the header file is only compiled once.


#include<iostream>
#include<string>
#include<vector>
#include "limits.h"
#include "graph.h"
#include<map>



using namespace std;
/*
 This program uses Dijkstra's algorithm to solve the shortest path problem
 Adjacency matrix used to store graphs
*/
//Record the information of the shortest path from the starting point to each vertex


struct Dis {
    vector<int> path;
    int value;
    bool visit;
    Dis() {
        visit = false;
        value = 0;
    }
};

struct forBackupLightpath {
    int wavelengthBLPNo;
    vector<int> wavelengthBLPath;
    bool canCreatBLPath;

    forBackupLightpath() {
        canCreatBLPath = false;
    }
};

struct findPathDetails {
    bool alreadyPPhave;
    vector<int> primaryShortPath;
    bool canCreatPP;
    int wavelengthNoPP;
    vector<int> tempPrimaryShortPath;
    bool tempCanCreatPP;
    int tempWavelengthNoPP;
    vector<int> tempBackUpShortPath;
    bool tempCanCreatBP;
    int tempWavelengthNoBP;
    vector<int> backUpShortPath;
    bool canCreatBP;
    int wavelengthNoBP;

    findPathDetails()
    {
        alreadyPPhave = false;
        canCreatPP = false;
        tempCanCreatPP = false;
        tempCanCreatBP = false;
        canCreatBP = false;
    }
 };


struct moreOEOConvertion {
    vector<vector<int>> allPathDetailsPP;
    vector<int> wavelengthNumberPP;
    vector<vector<int>> allPathDetailsBP;
    vector<int> wavelengthNumberBP;
    bool canCreatePP;
    bool canCreateBP;

    moreOEOConvertion() {
        canCreatePP = false;
        canCreateBP = false;
    }

};

class Graph_DG {
private:
    int vexnum;   //Number of vertices of the graph
    vector<vector<int>> arc;   //Adjacency matrix
    Dis* dis;   //Record the information of the shortest path of each vertex
    int intMAX = INT_MAX - 100000;
    vector<int> nullRepresenter;
public:
    //Constructor 
    Graph_DG(int vexnum, vector<vector<int>>);
    //Destructor
    ~Graph_DG();
    //Print adjacency matrix
    void print();
    //Find the shortest path
    void Dijkstra(int begin);
    //Print the shortest path
    vector<int> print_path(int, int);
    //delete edge
    void delete_edge(int);
    vector<vector<int>> conditionAppling(); //adding 0 or 1
    vector<vector<int>> conditionApplingForDefaultGraph();
    /*void dijkstraForWholeGraph(int& , vector<waveLengthNetworks>& , map<int, map<int, vector<vector<int>>>>& );*/
};

forBackupLightpath createLightPathBackup(int vexnum, vector<int> heavylightpath, vector<waveLengthNetworks>& waveLengthNetwork, int source, int destination);

moreOEOConvertion createMainGraph(int vexnum, vector<waveLengthNetworks> waveLengthNetwork, int source, int destination, map<int, map<int, vector<vector<int>>>> mapFromLPGraph, map<int, vector<vector<int>>> &);

void dijkstraForDefaultGraph(int&, waveLengthNetworks&, map<int,vector<vector<int>>>&);