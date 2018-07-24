//
//  ConfigLoader.cpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 03.05.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#include "ConfigLoader.hpp"

//TODO Create separate method so that it can be changed in case of new interface
ConfigLoader::ConfigLoader(std::string filePath){
    std::ifstream readFile(filePath);
    std::string line;
    std::getline(readFile, line);
    std::istringstream issGraph(line);
    std::string header, content;
    issGraph >> header >> content;
    graphPath = content;
    std::getline(readFile, line);
    std::istringstream issHeuristic(line);
    issHeuristic >> header >> content;
    heuristicId = std::stoi(content);
    std::getline(readFile, line);
    std::istringstream issMetric(line);
    issMetric >> header >> content;
    metricId = std::stoi(content);
    std::getline(readFile, line);
    std::istringstream issRepetitions(line);
    issRepetitions >> header >> content;
    repetitions = std::stoi(content);
    std::getline(readFile, line);
    std::istringstream issExperiments(line);
    issExperiments >> header >> content;
    experiments = std::stoi(content);
    std::getline(readFile, line);
    std::istringstream issTrees(line);
    issTrees >> header;
    for(int i = 0; i < experiments; ++i){
        issTrees >> content;
        treeNumbers.emplace_back(std::stoi(content));
    }
}

std::string ConfigLoader::getGraphPath(){
    return graphPath;
}

int ConfigLoader::getExperiments(){
    return experiments;
}

int ConfigLoader::getHeuristicId(){
    return heuristicId;
}

int ConfigLoader::getMetricId(){
    return metricId;
}

int ConfigLoader::getRepetitions(){
    return repetitions;
}

std::vector<int> ConfigLoader::getTreeNumbers(){
    return treeNumbers;
}
