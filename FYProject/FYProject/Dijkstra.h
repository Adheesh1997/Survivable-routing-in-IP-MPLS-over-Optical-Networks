#pragma once
//#pragma once is a commonly used C/C++ pragma,
//As long as this pragma is added at the beginning of the header file,
//You can ensure that the header file is only compiled once.


#include<iostream>
#include<string>
#include<vector>
#include "limits.h"
#include "graph.h"



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

struct findPathDetails {
    vector<int> primaryShortPath;
    bool canCreatPP;
    int wavelengthNoPP;
    vector<int> backUpShortPath;
    bool canCreatBP;
    int wavelengthNoBP;
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
};

//vector<int> initialize(int, vector<waveLengthNetworks>, int, int);

findPathDetails initialize(int vexnum, vector<waveLengthNetworks> waveLengthNetwork, int source, int destination);