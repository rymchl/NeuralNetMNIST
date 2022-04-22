#include"Network.h"
#include<math.h>
#include<iostream>
#include<map>

#define LOG(x) std::cout << " TEST " << x << std::endl

float rand_float(float l, float u){ 
    return (float)(rand()%1001)/1000.0f;
}

Network::Network(){
    //Input nodes:
    unsigned int num_input_pixel_values = 784;
    unsigned int num_output_values = 10;

    std::vector<unsigned int> network_structure = {num_input_pixel_values, 64, 64, num_output_values};

    unsigned int num_layers = network_structure.size();

    std::vector<Node> layer;

    layer.clear();
    for(int i = 0; i < num_input_pixel_values; i++){layer.push_back(Node(0));}

    layers.push_back(layer);

    for(int i = 1; i < num_layers; i++){
        layer.clear();
        for(int j = 0; j < network_structure[i]; j++){
            Node node(i);
            int index = 0;
            for(Node &prev_node : layers[i-1]){
                node.connect(&prev_node, rand_float(0,1));
            }
            layer.push_back(node);
        }
        layers.push_back(layer);
    }    
}

void Network::print_structure(){
    for(std::vector<Node> layer : layers) {
        printf("%lu | ", layer.size());
    }
    printf("\n");

}

void Network::clear_activations(){
    for(std::vector<Node> &layer : layers){
        for(Node &node : layer){
            node.activation = 0;
        }
    }
}

unsigned int Network::classify(Image image){
    clear_activations();

    //Send input
    for(int y = 0; y < image.HEIGHT; y++){
        for (int x = 0; x < image.WIDTH; x++){
            int node_index = y * image.HEIGHT + x;
            layers[0][node_index].activation = image.get(x,y);
        }
    }

    //Pass input thru network
    for(int i = 1; i < layers.size(); i++){
        for(Node &node : layers[i]){
            for(std::pair<Node*,float> connection : node.connections){
                node.activation += connection.first->activation * connection.second;
            }
           
        }
    }

    std::vector<Node> output_layer = layers[layers.size()-1];
    unsigned int output_value = 0;

    //Parse output
    for(int i = 0; i < output_layer.size(); i++){
        if(output_layer[i].activation > output_layer[output_value].activation) output_value = i;
        printf("%d : %f\n",i,output_layer[i].activation);
    }

    image.print();
    printf("I THINK ITS A %d\n",output_value);


    return output_value;
}