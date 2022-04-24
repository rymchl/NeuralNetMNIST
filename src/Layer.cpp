#include"Layer.h"


Layer::Layer(unsigned int ID, int num_nodes, int num_nodes_previous_layer): ID(ID){
    for(int i = 0; i < num_nodes; i++){
        std::vector<float> node_weights;
        for(int j = 0; j < num_nodes_previous_layer; j++){
            node_weights.push_back( ID==0 ? 1 : rand_float());
        }
        biases.push_back(ID==0 ? 0 : rand_float());
        weights.push_back(node_weights);
    }
}

std::vector<float> Layer::calc_z(std::vector<float> input){
    return add_vec(mat_vec_mul(weights,input), biases);
}

std::vector<float> Layer::activate(std::vector<float> input){
    if(ID == 0) return input;
    return apply_sigmoid(add_vec(mat_vec_mul(weights,input), biases));
}
