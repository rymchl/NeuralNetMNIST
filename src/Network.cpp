#include"Network.h"


Network::Network(){
    //Input nodes:
    unsigned int num_input_pixel_values = 784;
    unsigned int num_output_values = 10;

    std::vector<unsigned int> network_structure = {num_input_pixel_values, 16, 16, num_output_values};

    NUM_LAYERS = network_structure.size();
    
    //input layer
    layers.push_back(Layer(0,network_structure[0],0));
    
    for(int i = 1; i < NUM_LAYERS; i++){
        layers.push_back(Layer(i,network_structure[i],network_structure[i-1]));
    }    
}

std::vector<float> Network::classify(Image image){
    

    std::vector<std::vector<float> > activations;
    std::vector<std::vector<float> > z_values;

    activations.resize(layers.size());
    z_values.resize(layers.size());
    for (int i = 0; i < layers.size(); i++) {
        activations[i].resize(layers[i].biases.size());
        z_values[i].resize(layers[i].biases.size());
    }

    for (unsigned int y = 0; y < image.HEIGHT; y++) {
        for (unsigned int x = 0; x < image.WIDTH; x++) {
            activations[0][y * image.HEIGHT + x] = image.get(x, y) / 255.0f;
        }
    }

    //calculate z-values and activations for all nodes
    for (unsigned int id = 1; id < layers.size(); id++) {
        z_values[id] = layers[id].calc_z(activations[id - 1]);
        activations[id] = apply_sigmoid(z_values[id]);
    }
    

    return activations[NUM_LAYERS - 1];
}


void Network::save_weights_biases() {
    std::ofstream weights_out("weights.txt");
    std::ofstream biases_out("biases.txt");

    for (int l = 0; l < layers.size(); l++) {
        for (int j = 0; j < layers[l].SIZE; j++) {
            for (int k = 0; k < layers[l].weights[j].size(); k++) {
                weights_out << std::to_string(l) + ' ' + std::to_string(j) + ' ' + std::to_string(k) + ' ' + std::to_string(layers[l].weights[j][k]) << std::endl;
            }
            biases_out << std::to_string(l) + ' ' + std::to_string(j) + ' ' + std::to_string(layers[l].biases[j]) << std::endl;
        }
    }

    weights_out.close();
    biases_out.close();
}
