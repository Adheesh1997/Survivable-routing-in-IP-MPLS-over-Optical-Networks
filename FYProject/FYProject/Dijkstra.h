#pragma once

#include<iostream>
#include<string>
#include<vector>
#include <limits.h>
#include <vector>
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
    vector<int> print_path(int,int);
    //delete edge
    void delete_edge(int);
};

vector<int> shortestPath(int numOfNodes,vector<vector<int>> adjacencyMetrix, int source, int destination);
vector<int> backupShortestPath(int numOfNodes,vector<vector<int>> adjacencyMetrix, int source, int destination);
