#pragma once
#ifndef LAYER
#define LAYER

#include<vector>
#include"Util.h"

class Layer{
public:
    unsigned int ID;
    std::vector< std::vector < float > > weights;
    std::vector< float > biases;

    Layer(unsigned int ID, int num_nodes, int num_nodes_previous_layer);
    std::vector<float> calc_z(std::vector<float> input);
    std::vector<float> activate(std::vector<float> input);
};

#endif