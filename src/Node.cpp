#include "Node.h"

Node::Node(int layer) : layer(layer) { activation = 0; };

void Node::connect(Node* node, float weight){
    connections.push_back(std::pair<Node*,float>(node,weight));
}