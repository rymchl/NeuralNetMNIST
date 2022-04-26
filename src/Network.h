#pragma once
#ifndef NETWORK
#define NETWORK

#include<algorithm>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<math.h>
#include<map>

#include"Image.h"
#include"Layer.h"
#include"Util.h"


class Network{
public:
    std::vector<Layer> layers;
    unsigned int NUM_LAYERS;
    std::map<std::pair<unsigned int, unsigned int>, float> known_dCdA_values;
    
    Network();

    float cost(int label, std::vector<float> classification);
    float calc_dCdA(unsigned int id, unsigned int j, std::vector<std::vector<float> > z_values, std::vector<std::vector<float> > activations, std::vector<float> y);
    
    void print_classification(Image image, std::vector<float> classification);
    void save_weights_biases();
    void train(std::vector<Image> training_set, std::vector<Image> testing_set, int sample_size);
    void test(std::vector<Image> testing_set);

    std::vector<float> classify(Image image);

};

#endif