#include"Network.h"

//Randomized network
Network::Network(){
    unsigned int num_input_pixel_values = 784;
    unsigned int num_output_values = 10;
    //Randomize total number of layers between [1,3] inclusive
    unsigned int num_hidden_layers = rand() % 2 + 2;
    NUM_LAYERS = num_hidden_layers + 2;
    
    std::vector<unsigned int> network_structure = { num_input_pixel_values };
    for (int i = 0; i < num_hidden_layers; i++) {
       unsigned int num_nodes = ((rand() % (64 - 16)) / fmax(1, i)) + 16; // random int between [16,64], tending down toward output 
       network_structure.push_back(num_nodes);
    }
    network_structure.push_back(num_output_values);
    
  
    //input layer
    layers.push_back(Layer(0,network_structure[0],0));
    //rest of layers according to netweork structure
    for(int i = 1; i < NUM_LAYERS; i++){
        layers.push_back(Layer(i,network_structure[i],network_structure[i-1]));
    }    

    std::cout << "RANDOMIZED NETWORK STRUCTURE : { ";
    for (unsigned int layer_size : network_structure) 
        std::cout << layer_size << " ";
    std::cout << "}" << std::endl;

}

//Set network structure with randomized biases and weights 
Network::Network(std::vector<unsigned int> structure) {
    std::vector<unsigned int> network_structure;

    if (structure.front() != 784 || structure.back() != 10) {
        printf("Invalid structure input, proceeding with random structure.");
        unsigned int num_input_pixel_values = 784;
        unsigned int num_output_values = 10;
        unsigned int num_hidden_layers = rand() % 2 + 2;
       
        network_structure = { num_input_pixel_values };
        for (int i = 0; i < num_hidden_layers; i++) {
            unsigned int num_nodes = ((rand() % (64 - 16)) / fmax(1, i)) + 16; // random int between [16,64], tending down toward output 
            network_structure.push_back(num_nodes);
        }
        network_structure.push_back(num_output_values);
    }
    else {
        network_structure = structure;
    }
    
    NUM_LAYERS = network_structure.size();

    //input layer
    layers.push_back(Layer(0, network_structure[0], 0));
    //rest of layers according to netweork structure
    for (int i = 1; i < NUM_LAYERS; i++) {
        layers.push_back(Layer(i, network_structure[i], network_structure[i - 1]));
    }

    std::cout << "RANDOMIZED NETWORK STRUCTURE : { ";
    for (unsigned int layer_size : network_structure)
        std::cout << layer_size << " ";
    std::cout << "}" << std::endl;

}

Network::Network(std::string filepath) {
    
    std::vector<unsigned int> network_structure;
    std::vector<float> line_values;

    //Read through the input file and save all values in line_values
    {
        std::ifstream network_in(filepath);
        std::string line;
        std::string val_str = "";

        //First line defines network structure, space seperated
        std::getline(network_in, line);
        for (char c : line)
            if (c == ' ' && val_str.size() > 0) {
                network_structure.push_back(atoi(val_str.c_str()));
                val_str = "";
            }
            else val_str += c;
        if (val_str.size() > 0)
            network_structure.push_back(atoi(val_str.c_str()));

        while (std::getline(network_in, line)) {
            line_values.push_back(atof(line.c_str()));
        }

        network_in.close();
    }

    NUM_LAYERS = network_structure.size();
    int line_indx = 0;

    std::vector<std::vector<std::vector<float>>> weights;
    weights.resize(NUM_LAYERS);
    std::vector<std::vector<float>> biases;
    biases.resize(NUM_LAYERS);
    
    //Init weight and bias vectors
    for (int L = 0; L < NUM_LAYERS; L++){
        weights[L].resize(network_structure[L]);
        biases[L].resize(network_structure[L]);
        for (int W = 0; W < weights[L].size(); W++){
            weights[L][W].resize(L == 0 ? 0 : network_structure[L-1]);
        }
    }

    //Weights are stored first in sequence
    for (int L = 1; L < NUM_LAYERS; L++) {
        for (int j = 0; j < weights[L].size(); j++) {
            for (int k = 0; k < weights[L][j].size(); k++) {
               
                weights[L][j][k] = line_values[line_indx++];
            }
        }
    }

    //Biases last
    for (int L = 1; L < NUM_LAYERS; L++) {
        for (int j = 0; j < biases[L].size(); j++) {
            biases[L][j] = line_values[line_indx++];
        }
    }

    //Combine 
    for (int L = 0; L < NUM_LAYERS; L++)
        layers.push_back(Layer(L, weights[L], biases[L]));
    
}

void Network::save_network_to_file(std::string filepath) {
    std::ofstream file_out(filepath);

    //FORMAT:
    // First line shows network structure, every subsequent line shows each weight value, then each bias value (excluding layer 0)
    // LINE1: (space seperated): <num_nodes_input_layer> <num_nodes_input_layer_+_1> ... <num_nodes_output_layer>
    // LINE2: layer[1].weights[0][0] (exclude input layer)
    // LINE3: layer[1].weights[0][1]
    // ...
    // LINE(X-1): layer[TOTAL_LAYERS].weights[MAX_x][MAX_y]
    // LINE(X):   layer[1].biases[0]
    // LINE(X+1): layer[1].biases[1]
    // ...
    // LINE(EOF): layer[TOTAL_LAYERS].biases[MAX_x]


    for (int l = 0; l < layers.size(); l++) {
        file_out << layers[l].SIZE << " ";
    }
    file_out << std::endl;

    for (int l = 0; l < layers.size(); l++) {
        for (int j = 0; j < layers[l].SIZE; j++) {
            for (int k = 0; k < layers[l].weights[j].size(); k++) {
                file_out <<  std::to_string(layers[l].weights[j][k]).c_str() << std::endl;
            }
        }
    }
    file_out.close();
}
