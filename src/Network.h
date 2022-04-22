#pragma once
#ifndef NETWORK
#define NETWORK

#include <vector>
#include "Node.h"
#include "Image.h"

class Network{
public:
    std::vector< std::vector<Node> > layers;
    Network();
    unsigned int classify(Image image);
    void clear_activations();
    void print_structure();
};

#endif