//
//  ConfigLoader.hpp
//  MultiTreeRouting
//
//  Created by Matteo Morisoli on 03.05.18.
//  Copyright © 2018 MatteoMorisoli. All rights reserved.
//

#ifndef ConfigLoader_hpp
#define ConfigLoader_hpp

#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class ConfigLoader {
public:
    ConfigLoader(const std::string filePath);
    std::string getGraphPath();
    int getExperiments();
    int getHeuristicId();
    int getMetricId();
    int getRepetitions();
    std::vector<int> getTreeNumbers();
    
private:
    std::string graphPath;
    int heuristicId;
    int metricId;
    int repetitions;
    int experiments;
    std::vector<int> treeNumbers;
};

#endif /* ConfigLoader_hpp */
