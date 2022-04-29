#include"Layer.h"

//Randomized layer
Layer::Layer(unsigned int ID, int num_nodes, int num_nodes_previous_layer): ID(ID), SIZE(num_nodes){
    for(int i = 0; i < num_nodes; i++){
        std::vector<float> node_weights;

        for(int j = 0; j < num_nodes_previous_layer; j++)
            node_weights.push_back( ID==0 ? 1 : rand_float());

        biases.push_back(ID==0 ? 0 : rand_float());
        weights.push_back(node_weights);
    }
}

//Prebuilt layer
Layer::Layer(unsigned int ID, std::vector<std::vector<float>> weights, std::vector<float> biases) : ID(ID), SIZE(biases.size()) {
    this->weights = weights;
    this->biases = biases;
}

//Weighted sum of input plus bias
std::vector<float> Layer::calc_z(std::vector<float> input){
    if (ID == 0) return input;
    return add_vec(mat_vec_mul(weights,input), biases);
}

//activation function ( sigmoid(z_node) ) for all nodes
std::vector<float> Layer::activate(std::vector<float> input){
    if(ID == 0) return input;
    return apply_sigmoid(calc_z(input));
}
