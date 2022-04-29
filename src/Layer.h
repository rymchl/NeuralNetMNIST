#pragma once
#ifndef LAYER
#define LAYER

#include<vector>
#include"Util.h"

class Layer{
public:
    //Layer[ID==0] = input-layer ... Layer[ID==NUM_LAYERS] = output-layer
    unsigned int ID;
    //Number of nodes in the layer
    unsigned int SIZE;

    std::vector< std::vector <float>> weights;
    std::vector<float> biases;

    //Randomized weights + biases
    Layer(unsigned int ID, int num_nodes, int num_nodes_previous_layer); 
    //Prebuilt layer
    Layer(unsigned int ID, std::vector<std::vector<float>> weights, std::vector<float> biases); 
    //Weighted sum of input plus bias
    std::vector<float> calc_z(std::vector<float> input); 
    //activation function ( sigmoid(z_node) ) for all nodes
    std::vector<float> activate(std::vector<float> input);
};

#endif