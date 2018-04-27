//
//  main.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 19.02.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/numeric/ublas/triangular.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/graph/johnson_all_pairs_shortest.hpp>
#include <iostream>
#include <fstream>
#include "GraphLoader.hpp"
#include "SimpleHeuristic.hpp"
#include "CenterHeuristic.hpp"
#include "TreeWorker.hpp"
#include "Printer.hpp"
#include "MatrixLoader.hpp"
#include "DataAnalyser.hpp"
#include "Writer.hpp"

using namespace boost;

using EdgeWeight = property<edge_weight_t, int>;
using VertexName = property<vertex_name_t, std::string> ;
using Graph = adjacency_list<setS, vecS, undirectedS, VertexName, EdgeWeight>;
using vertexDescriptor = graph_traits<Graph>::vertex_descriptor;
using edgeDescriptor = graph_traits<Graph>::edge_descriptor;
using DistanceMatrix = boost::exterior_vertex_property<Graph, int>::matrix_type;
using TreeDistanceMatrix = boost::numeric::ublas::matrix<int>;
using SmartDistanceMatrix = boost::numeric::ublas::triangular_matrix<int, boost::numeric::ublas::upper>;
using StretchMatrix = boost::numeric::ublas::matrix<float>;
using SmartStretchMatrix = boost::numeric::ublas::triangular_matrix<float, boost::numeric::ublas::upper>;

//prototype
void computeTree(const int root, const Graph& g, const SmartDistanceMatrix &dm, SmartStretchMatrix &sm, std::vector<Congestion> &c);


/* Parameters for the main are the path of the file containing the graph, the number of trees to create on that graph,
 the heuristic to use, identified with a integer, and the number of repetitions (use 1 if the selected heuristic is completely deterministic)
 */
int main(int argc, const char * argv[]) {
    
    
    //d\Definitions of the parameters for ease of use
    const std::string startingGraphPath = std::string(argv[1]);
    const int treeNum = atoi(argv[2]);
    const int heuristicType = atoi(argv[3]);
    std::string suffix;
    if(heuristicType == 1){
        suffix = "center";
    }else{
        suffix = "random";
    }
    const int repetitions = atoi(argv[4]);
    
    
    Graph graph;
    //Loading the graph from file and file creation as an adjacency list
    {GraphLoader g(startingGraphPath);

        int index = 0;
        //creating the mapping from name to int used in the graph
        std::map<std::string, int> mapping;
        for(auto it = g.getVerticesLabels().begin(); it != g.getVerticesLabels().end(); ++it){
            add_vertex(*it, graph);
            mapping.insert(std::pair<std::string, int>(*it, index));
            index++;
        }
        //setting edge weight at 1 for every edge in our case
        EdgeWeight weight(1);
        for(auto it = g.getEdgeList().begin(); it != g.getEdgeList().end(); ++it){
            add_edge(mapping.at(it->first), mapping.at(it->second), weight, graph);
        }
    }
    
    
    //storing the number of vertices in variable for readability
    const int vertexNum = (int) num_vertices(graph);
    //condition for number of trees less than number of nodes, necessary for certain heuristics
    if(num_vertices(graph) < treeNum){
        return 0;
    }
    std::cout << "graph loaded!" << std::endl;
    std::cout << "Number of vertices: " << vertexNum << std::endl;
    std::cout << "Number of edges: " << num_edges(graph) << std::endl;
    
    
    //Distance matrix creation and population using Johnson
    SmartDistanceMatrix smartDistanceMatrix(vertexNum-1, vertexNum-1);
    {DistanceMatrix distanceMatrix(vertexNum);
        std::size_t dotPos = startingGraphPath.find_last_of(".");
        std::string matrixFileName = startingGraphPath.substr(0, dotPos);
        matrixFileName.append(".dmf");
        MatrixLoader ml(matrixFileName);
        //to save computations, if matrix already computed and in file, load it, otherwise compute it and save it to file
        if(!ml.fileExists()){
            std::cout << "computing..." << std::endl;
            johnson_all_pairs_shortest_paths(graph, distanceMatrix);
            ml.writeMatrix(distanceMatrix, vertexNum);
            for(int i = 0; i < vertexNum - 1; ++i){
                for(int j = i+1; j < vertexNum; ++j){
                    smartDistanceMatrix(i, j-1) = distanceMatrix[i][j];
                }
            }
        }else{
            std::cout << "reading..." << std::endl;
            ml.readMatrix(smartDistanceMatrix, vertexNum-1);
        }
    }
    std::cout << "all-pairs smart matrix ready!" << std::endl;
    
    
    //heuristic for starting node selection
    SmartStretchMatrix stretchStarStar(vertexNum-1, vertexNum-1);
    SmartStretchMatrix stretchStar(vertexNum-1, vertexNum-1);
    for(int i = 0; i < vertexNum - 1; ++i){
        for(int j = i; j < vertexNum -1; ++j){
            stretchStarStar(i, j) = 0;
            stretchStar(i, j) = 0;
        }
    }
    DataAnalyser dAll(startingGraphPath, suffix, false, treeNum);
    for(int i = 0; i < repetitions; ++i){
        for(int k = 0; k < vertexNum - 1; ++k){
            for(int j = k; j < vertexNum -1; ++j){
                stretchStar(k, j) = 0;
            }
        }
        std::cout << "starting repetition " << i+1 << " of " << repetitions << "..." << std::endl;
        //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        //creation of the trees and of the relative distance matrices and stretch matrices
        std::vector<Congestion> congestions; //storage of congestion informations for each tree
        std::vector<int> starterNodes;
        if(heuristicType == 1){
            CenterHeuristic h;
            std::set<int> simpleStarters = h.CenterHeuristic::selectStartingNodes(vertexNum, graph, treeNum);
            starterNodes.insert(starterNodes.end(), simpleStarters.begin(), simpleStarters.end());
            std::cout << "Heuristic chosen: center" << std::endl;
        }else if(heuristicType == 0){
            SimpleHeuristic h;
            std::set<int> simpleStarters = h.SimpleHeuristic::selectStartingNodes(vertexNum, graph, treeNum);
            starterNodes.insert(starterNodes.end(), simpleStarters.begin(), simpleStarters.end());
            std::cout << "Heuristic chosen: random" << std::endl;
        }
        //StretchMatrix stretchStar(vertexNum, vertexNum, 0);
        SmartStretchMatrix sm(vertexNum-1, vertexNum-1);
        for(int x=0; x < starterNodes.size(); ++x){
            computeTree(starterNodes[x], graph, smartDistanceMatrix, sm, congestions);
            std::cout << "starting sum" << std::endl;
            for(int y = 0; y < sm.size1(); ++y){
                for(int z = y; z < sm.size2();++z){
                    stretchStar(y, z) += sm(y,z);
                }
            }
            //stretchStar += sm;
            std::cout << "ended sum" << std::endl;
        }
        std::cout << "starting division" << std::endl;
        stretchStar /= treeNum;
        std::cout << "ended division" << std::endl;
        dAll.addData(stretchStar);
        std::cout << "starting sum" << std::endl;
        for(int y = 0; y < sm.size1(); ++y){
            for(int z = y; z < sm.size2();++z){
                stretchStarStar(y, z) += stretchStar(y,z);
            }
        }
        //stretchStarStar += stretchStar;
        std::cout << "ended sum" << std::endl;
        std::cout << "repetition " << i+1 << " done!" << std::endl;
    }
    dAll.generateFiles();
    if(repetitions != 1){
        stretchStarStar /= repetitions;
        DataAnalyser dStar(startingGraphPath, suffix, true, treeNum);
        dStar.addData(stretchStarStar);
        dStar.generateFiles();
        std::cout << "Files generated" << std::endl;
    }
    return 0;
}

void computeTree(const int root, const Graph& g, const SmartDistanceMatrix &dm, SmartStretchMatrix &sm, std::vector<Congestion> &c){
    std::cout << "starting tree with root " << root << "..." << std::endl;
    int vertexNum = num_vertices(g);
    std::vector<vertexDescriptor> parents(vertexNum); //necessary for dijkstra
    std::vector<int> distances(vertexNum); //necessary for dijkstra
    dijkstra_shortest_paths(g, root, predecessor_map(&parents[0]).distance_map(&distances[0]));
    std::vector<int> predecessors(parents.begin(), parents.end());
    TreeWorker t(distances, predecessors, root, g);
    //c.emplace_back(t.getCongestion());
    for(int i = 0; i < vertexNum-1; ++i) {
                if(i % 1000 == 0){
                    std::cout << ((double) i)/vertexNum*100.0 << "% tree with root " << root << std::endl;
                }
        for(int j = i; j < vertexNum-1; ++j) {
                sm(i, j) = (distances[i] + distances[j+1] - 2 * distances[t.lca(i, j+1)]) / (float) dm(i, j);
        }
    }
    std::cout << "finished tree with root " << root << std::endl;
}

