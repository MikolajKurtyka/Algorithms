#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <stack>
#include <algorithm>

struct Edge{
    int src;
    int dest;
};

class Graph{
    public:
        std::vector < std::vector <int>> adjList;
        int graph_size;
        
        Graph(){
        }
        
        Graph(std::vector <Edge> const &edges, int n){
            graph_size = n;
            adjList.resize(n);
            
            for(auto &edge: edges){
                adjList[edge.src].push_back(edge.dest);
            }
        }
        
        Graph( std::string file_path , int n, bool not_rev = 1){
            graph_size = n;
            adjList.resize(n);
            std::fstream file;
            file.open(file_path , std::ios::in);
            
            int n_edges = 0;
            if(file.is_open()){
                std::string line;
                std::stringstream ss;
                int src, dest;
                while(getline(file, line)){
                    ss << line;
                    ss>>src>>dest;
                    if(not_rev){
                        adjList[src-1].push_back(dest -1);
                        n_edges++;
                    }
                    else{
                        adjList[dest-1].push_back(src-1);
                        n_edges++;
                    }
                }
            }
            if (not_rev){
                std::cout << "Graph loaded" << std::endl;
            }
            else{
                std::cout << "Graph_rev loaded" << std::endl;
            }
            std::cout << "The biggest vertex index: " << n -1 << std::endl;
            std::cout << "Number of edges loaded: " << n_edges << std::endl;
            file.close();
        }
        
        void print(int n_of_lines){
            for(int i = 0; i < n_of_lines; i++){
                std::cout << i << "-->";
                for(int edge: adjList[i]){
                    std::cout << edge << " ";
                }
                std::cout << std::endl;
            }
        }
};


class Kosaraju{
    public:
    friend Graph;
    std::vector <int> leader_vec;
    Graph graph;
    Graph graph_rev;
    
    Kosaraju(Graph _graph, Graph _graph_rev){
        graph = _graph;
        graph_rev = _graph_rev;
    }
    
    void DFS_1(int v, bool visited[], std::stack<int> &Stack){
        visited[v] = true;
        std::vector<int>::reverse_iterator i;
        for(i = graph_rev.adjList[v].rbegin(); i != graph_rev.adjList[v].rend(); i++){
            if(!visited[*i]){
                DFS_1(*i, visited, Stack);
            }
        }
        Stack.push(v);
    }
    
    void DFS_2(int v, bool visited[], int leader[], int lead){
        visited[v] = true;
        leader[v] = lead;

        std::vector<int>::reverse_iterator i;
        for(i = graph.adjList[v].rbegin(); i != graph.adjList[v].rend(); i++){
            if (!visited[*i])
                DFS_2(*i, visited, leader, lead);
        }
    }
    
    void CSS(){
        std::cout << "CSS start" << std::endl;
        std::stack <int> Stack;
        int n = graph.graph_size;
        std::cout << "graph size: "<< n << std::endl;
        bool *visited = new bool[n];
        
        std::cout << "DFS1 start" << std::endl;
        //first loop
        for(int i = 0; i < n; i++){
            visited[i] = false;
        }
        for(int i = n-1; i >= 0; i--){
            if(!visited[i])
                DFS_1(i, visited, Stack);
        }
        //second loop
        std::cout << "DFS2 start" << std::endl;
        for(int i = 0; i < n; i++){
            visited[i] = false;
        }
        
        int *leader = new int[n];
        while(Stack.empty() == false){
            int v = Stack.top();
            Stack.pop();

            if(visited[v] == false){
                DFS_2(v, visited, leader, v);
            }
        }
        
        for(int i = 0; i < n; i++){
            leader_vec.push_back(leader[i]);
        }   
    }
    void print_leader(){
        std::vector<int>::iterator i;
        for(i = leader_vec.begin(); i != leader_vec.end(); i ++){
            std::cout << *i << std::endl;
        }
    }
};

int main(){


    //Load graph and graph_rev from SCC file
    Graph graph("SCC.txt", 875714);
    Graph graph_rev("SCC.txt", 875714, 0);
    
    //Create Kosaraju algorithm class
    Kosaraju kj(graph, graph_rev);
    //Run kosaraju algorithm
    kj.CSS();
    
    //Find teh size of 10 biggest strongly connected groups
    std::vector <int> lead_vec= kj.leader_vec;
    std::vector <int> count_vec(lead_vec.size(), 0);
    sort(lead_vec.begin(), lead_vec.end());
    std::cout << "lead vec size: " << lead_vec.size() << " " << std::endl;
    for(int i = 0; i < lead_vec.size(); i++){
        count_vec[lead_vec[i]] += 1;
    }
    sort(count_vec.begin(), count_vec.end(), std::greater<int>());
    for(int i = 0; i < 10; i++){
        std::cout << i << " " << count_vec[i] << std::endl;
    }
    
    
	return 0;

}
