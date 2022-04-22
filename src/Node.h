#pragma once
#ifndef NODE
#define NODE

#include <vector>

class Node{
public:
    int layer;
    float activation;
    std::vector< std::pair<Node*,float> > connections;
    Node(int layer);
    void connect(Node* node, float weight);
};

#endif