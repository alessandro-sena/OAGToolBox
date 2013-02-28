#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <utility>   
using namespace std;


void readGraph(string &filename, vector<vector<pair<unsigned int, double> > > &graph){
    unsigned int edges;
    unsigned int vertex;
    unsigned int i;

    unsigned int edge1, edge2;
    double weight;
    FILE *fin = fopen(filename.c_str(), "r");
    fscanf(fin, "%d %d", &vertex, &edges);

    //Init graph
    for (i=0; i<vertex; i++){
        vector<pair<unsigned int, double> > v;
        graph.push_back(v);
    }
    
  
    for (i=0; i<edges; i++){
        fscanf(fin, "%d %d %lf", &edge1, &edge2, &weight);
        pair <unsigned int,double> p1(edge1, weight);
        pair <unsigned int,double> p2(edge2, weight);
        graph[edge1].push_back(p2);
        graph[edge2].push_back(p1); 
    }

    fclose(fin);
}

void markovIt(vector<vector<pair<unsigned int, double> > > &graph){
    unsigned int n = graph.size();
    unsigned int i, j;
    for (i=0; i<n;i++){   
        vector<pair<unsigned int, double> > *adjacencyList = &graph[i];
        unsigned int pairs = adjacencyList->size();

        double sum = 0;            
        for (j=0; j<pairs;j++)
            sum+= (*adjacencyList)[j].second;

        for (j=0; j<pairs;j++)
            (*adjacencyList)[j].second = (*adjacencyList)[j].second/sum;
    }


	printf("%d %d %d\n", (int)graph.size(), (int)graph.size(), (int)n);
    for (i=0; i<n;i++){   
        vector<pair<unsigned int, double> > *adjacencyList = &graph[i];
        unsigned int pairs = adjacencyList->size();

        for (j=0; j<pairs;j++)
        {
            printf("%d %d %lf\n", i, (*adjacencyList)[j].first, (*adjacencyList)[j].second);
        }

    }

}





int main()
{
    string filename("entOAG.txt");
    vector<vector<pair<unsigned int, double> > > graph;
    readGraph(filename, graph);
    markovIt(graph);
    return 0;
}

