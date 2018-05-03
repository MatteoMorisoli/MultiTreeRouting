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
#include "ConfigLoader.hpp"

using namespace boost;

using EdgeWeight = property<edge_weight_t, int>;
using VertexName = property<vertex_name_t, std::string> ;
using Graph = adjacency_list<setS, vecS, undirectedS, VertexName, EdgeWeight>;
using vertexDescriptor = graph_traits<Graph>::vertex_descriptor;
using edgeDescriptor = graph_traits<Graph>::edge_descriptor;
using DistanceMatrix = boost::exterior_vertex_property<Graph, int>::matrix_type;
using DiagMatrixInt = boost::numeric::ublas::triangular_matrix<int, boost::numeric::ublas::upper>;
using DiagMatrixFloat = boost::numeric::ublas::triangular_matrix<float, boost::numeric::ublas::upper>;

//prototype
void computeTreeStretch(const int root, const Graph& g, const DiagMatrixInt &dm, DiagMatrixFloat &sm);
void computeTreeCongestion(const int root, const Graph& g, DiagMatrixFloat &cm);


/* Parameters for the main are the path of the file containing the graph, the number of trees to create on that graph,
 the heuristic to use, identified with a integer, and the number of repetitions (use 1 if the selected heuristic is completely deterministic)
 */
int main(int argc, const char * argv[]) {
    
    
    //d\Definitions of the parameters for ease of use
    ConfigLoader config(argv[1]);
//    const std::string startingGraphPath = std::string(argv[1]);
//    const int treeNum = atoi(argv[2]);
//    const int heuristicType = atoi(argv[3]);
    std::string suffix;
    if(config.getHeuristicId() == 1){
        suffix = "center";
    }else{
        suffix = "random";
    }
    if(config.getMetricId() == 1){
        suffix.append("congestion");
    }else{
        suffix.append("stretch");
    }
//    const int repetitions = atoi(argv[4]);
    
    for(int a = 0; a < config.getExperiments(); ++a){
        Graph graph;
        //Loading the graph from file and file creation as an adjacency list
        {GraphLoader g(config.getGraphPath());

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
        std::cout << "graph loaded!" << std::endl;
        std::cout << "Number of vertices: " << vertexNum << std::endl;
        std::cout << "Number of edges: " << num_edges(graph) << std::endl;
        
        
        //Distance matrix creation and population using Johnson
        DiagMatrixInt diagDistanceMatrix(vertexNum-1, vertexNum-1);
        {DistanceMatrix distanceMatrix(vertexNum);
            std::size_t dotPos = config.getGraphPath().find_last_of(".");
            std::string matrixFileName = config.getGraphPath().substr(0, dotPos);
            matrixFileName.append(".dmf");
            MatrixLoader ml(matrixFileName);
            //to save computations, if matrix already computed and in file, load it, otherwise compute it and save it to file
            if(!ml.fileExists()){
                std::cout << "computing..." << std::endl;
                johnson_all_pairs_shortest_paths(graph, distanceMatrix);
                ml.writeMatrix(distanceMatrix, vertexNum);
                for(int i = 0; i < vertexNum - 1; ++i){
                    for(int j = i+1; j < vertexNum; ++j){
                        diagDistanceMatrix(i, j-1) = distanceMatrix[i][j];
                    }
                }
            }else{
                std::cout << "reading..." << std::endl;
                ml.readMatrix(diagDistanceMatrix, vertexNum-1);
            }
        }
        std::cout << "all-pairs smart matrix ready!" << std::endl;
        
        
        //heuristic for starting node selection
        DiagMatrixFloat resultStarStar(vertexNum-1, vertexNum-1);
        DiagMatrixFloat resultStar(vertexNum-1, vertexNum-1);
        for(int i = 0; i < vertexNum - 1; ++i){
            for(int j = i; j < vertexNum -1; ++j){
                resultStarStar(i, j) = 0;
                resultStar(i, j) = 0;
            }
        }
        DataAnalyser dAll(config.getGraphPath(), suffix, false, config.getTreeNumbers()[a]);
        for(int i = 0; i < config.getRepetitions(); ++i){
            for(int k = 0; k < vertexNum - 1; ++k){
                for(int j = k; j < vertexNum -1; ++j){
                    resultStar(k, j) = 0;
                }
            }
            std::cout << "starting repetition " << i+1 << " of " << config.getRepetitions() << "..." << std::endl;
            //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            //creation of the trees and of the relative distance matrices and stretch matrices
            std::vector<int> starterNodes;
            if(config.getHeuristicId() == 1){
                CenterHeuristic h;
                std::set<int> simpleStarters = h.CenterHeuristic::selectStartingNodes(vertexNum, graph, config.getTreeNumbers()[a]);
                starterNodes.insert(starterNodes.end(), simpleStarters.begin(), simpleStarters.end());
                std::cout << "Heuristic chosen: center" << std::endl;
            }else if(config.getHeuristicId() == 0){
                SimpleHeuristic h;
                std::set<int> simpleStarters = h.SimpleHeuristic::selectStartingNodes(vertexNum, graph, config.getTreeNumbers()[a]);
                starterNodes.insert(starterNodes.end(), simpleStarters.begin(), simpleStarters.end());
                std::cout << "Heuristic chosen: random" << std::endl;
            }
            //StretchMatrix stretchStar(vertexNum, vertexNum, 0);
            DiagMatrixFloat sm(vertexNum-1, vertexNum-1);
            for(int x=0; x < starterNodes.size(); ++x){
                if(config.getMetricId() == 0){
                    computeTreeStretch(starterNodes[x], graph, diagDistanceMatrix, sm);
                }else if(config.getMetricId() == 1){
                    for(int k = 0; k < vertexNum - 1; ++k){
                        for(int j = k; j < vertexNum -1; ++j){
                            sm(k, j) = 0;
                        }
                    }
                    computeTreeCongestion(starterNodes[x], graph, sm);
                }
                std::cout << "starting sum" << std::endl;
                for(int y = 0; y < sm.size1(); ++y){
                    for(int z = y; z < sm.size2();++z){
                        resultStar(y, z) += sm(y,z);
                    }
                }
                //stretchStar += sm;
                std::cout << "ended sum" << std::endl;
            }
            std::cout << "starting division" << std::endl;
            resultStar /= config.getTreeNumbers()[a];
            std::cout << "ended division" << std::endl;
            dAll.addData(resultStar);
            std::cout << "starting sum" << std::endl;
            for(int y = 0; y < sm.size1(); ++y){
                for(int z = y; z < sm.size2();++z){
                    resultStarStar(y, z) += resultStar(y,z);
                }
            }
            //stretchStarStar += stretchStar;
            std::cout << "ended sum" << std::endl;
            std::cout << "repetition " << i+1 << " done!" << std::endl;
        }
        dAll.generateStretchFiles();
        if(config.getRepetitions() != 1){
            resultStarStar /= config.getRepetitions();
            DataAnalyser dStar(config.getGraphPath(), suffix, true, config.getTreeNumbers()[a]);
            dStar.addData(resultStarStar);
            dStar.generateStretchFiles();
            std::cout << "Files generated" << std::endl;
        }
    }
    return 0;
}

void computeTreeStretch(const int root, const Graph& g, const DiagMatrixInt &dm, DiagMatrixFloat &sm){
    std::cout << "starting tree with root " << root << "..." << std::endl;
    int vertexNum = num_vertices(g);
    std::vector<vertexDescriptor> parents(vertexNum); //necessary for dijkstra
    std::vector<int> distances(vertexNum); //necessary for dijkstra
    dijkstra_shortest_paths(g, root, predecessor_map(&parents[0]).distance_map(&distances[0]));
    std::vector<int> predecessors(parents.begin(), parents.end());
    TreeWorker t(distances, predecessors, root, g);
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

void computeTreeCongestion(const int root, const Graph& g, DiagMatrixFloat &cm){
    int vertexNum = num_vertices(g);
    std::vector<vertexDescriptor> parents(vertexNum); //necessary for dijkstra
    std::vector<int> distances(vertexNum); //necessary for dijkstra
    dijkstra_shortest_paths(g, root, predecessor_map(&parents[0]).distance_map(&distances[0]));
    std::vector<int> predecessors(parents.begin(), parents.end());
    TreeWorker t(distances, predecessors, root, g);
    Congestion congestion = t.getCongestion();
    for(int i = 0; i < congestion.edges.size(); ++i){
        cm(congestion.edges[i].first, congestion.edges[i].second -1) = congestion.congestionValues[i];
    }
}

