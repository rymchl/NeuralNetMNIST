#pragma once
#ifndef NETWORK
#define NETWORK

#include <vector>
#include<math.h>
#include<iostream>
#include<fstream>
#include<map>

#include"Util.h"
#include "Image.h"
#include "Layer.h"
#include <string>

class Network{
public:
    std::vector<Layer> layers;
    unsigned int NUM_LAYERS;
    
    std::map<std::pair<unsigned int, unsigned int>, float> known_dCdA_values;
    
    Network();

    void print_classification(Image image, std::vector<float> classification);
    std::vector<float> classify(Image image);
    float cost(int label, std::vector<float> classification);
    float total_cost(std::vector<Image> image_set);
    void train(std::vector<Image> training_set, int iterations);
    float get_classification_rate(std::vector<Image> image_set);
    float calc_dCdA(unsigned int id, unsigned int j, std::vector<std::vector<float> > z_values, std::vector<std::vector<float> > activations, std::vector<float> y);
    void save_weights_biases();
};

#endif