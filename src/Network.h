#pragma once
#ifndef NETWORK
#define NETWORK

#include <vector>
#include<math.h>
#include<iostream>
#include<map>

#include"Util.h"
#include "Image.h"
#include "Layer.h"

class Network{
public:
    std::vector<Layer> layers;
    
    Network();

    void print_classification(Image image, std::vector<float> classification);
    std::vector<float> classify(Image image);
    float cost(int label, std::vector<float> classification);
    float total_cost(std::vector<Image> image_set);
    void train(std::vector<Image> training_set, int iterations);
};

#endif