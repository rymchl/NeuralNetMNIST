#include <vector>

class Node{
public:
    int layer;
    std::vector< std::pair<Node,float> > connections;

};