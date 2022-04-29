#pragma once
#ifndef UTIL
#define UTIL

#include<vector>
#include<math.h>
#include<string>
#include<iostream>

#include"Image.h"

#define LOG(x) std::cout << x << std::endl

//Parses the input vectors (args 2,3,4,5) into vectors of my Image class
//and puts them into the input sets (args 1 and 2) directly. 
template <typename Pixel = uint8_t, typename Label = uint8_t>
static void parse_as_images(std::vector<Image> &training_set, std::vector<Image> &testing_set,
                            std::vector<std::vector<Pixel>> training_images,
                            std::vector<std::vector<Pixel>> test_images,
                            std::vector<Label> training_labels,
                            std::vector<Label> test_labels){
    uint8_t WIDTH = 28;
    uint8_t HEIGHT = 28;
    unsigned int NUM_TRAINING_EXAMPLES = 60000;
    unsigned int NUM_TESTING_EXAMPLES = 10000;

    for (int ex = 0; ex < NUM_TRAINING_EXAMPLES; ex++) {
        uint8_t LABEL = training_labels[ex];
        Image image(WIDTH, HEIGHT, LABEL);
        for (int i = 0; i < training_images[ex].size(); i++) {
            int x = i % WIDTH;
            int y = i / HEIGHT;
            image.put(x, y, training_images[ex][i]);
        }
        training_set.push_back(image);
    }

    for (int ex = 0; ex < NUM_TESTING_EXAMPLES; ex++) {
        uint8_t LABEL = test_labels[ex];
        Image image(WIDTH, HEIGHT, LABEL);
        for (int i = 0; i < test_images[ex].size(); i++) {
            int x = i % WIDTH;
            int y = i / HEIGHT;
            image.put(x, y, test_images[ex][i]);
        }
        testing_set.push_back(image);
    }
}

//Random float value between -1 and 1 inclusive [-1,1].
static float rand_float(){ 
    return (float)(rand()%1001)/500.0f - 1.0f;
}

//Assumes a and b of the same size
static float dot_prod(std::vector<float> a, std::vector<float> b){
    float out = 0;
    for(int i = 0; i < a.size(); i++){ out += a[i]*b[i]; }
    return out;
}

//Assumes a and b of the same size
static std::vector<float> add_vec(std::vector<float> a, std::vector<float> b){
    for(int i = 0; i < a.size(); i++){ a[i] += b[i]; }
    return a;
}

//Multiply the input matrix with the input vector.
//Assumes that the vector is of the same size as the matrices rows, 
//and that every row in the matrix is this size.
static std::vector<float> mat_vec_mul(std::vector< std::vector < float > > mat, std::vector< float > vec){
    std::vector<float> out;
    for(std::vector<float> row : mat){
        out.push_back(dot_prod(row,vec));
    }
    return out;
}

//Applys the sigmoid function to every element in vec
static std::vector<float> apply_sigmoid(std::vector<float> vec){
    std::vector<float> out;
    for (float x : vec) out.push_back(1.0f / (1.0f + exp(-x)));
    return out;
}

//Derivative of the sigmoid function
static float der_sigmoid(float x){
    return exp(-x) / pow(1 + exp(-x), 2);
}

template <class T>
static int max_index(std::vector<T> vec) {
    int index = 0;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] > vec[index]) index = i;
    }
    return index;
}

//Randomly samples n elements from the input vec
template <class T>  
static std::vector<T> sample(int n, std::vector<T> vec){
    std::vector<T> out; 
    for(int i = 0; i < n; i++) {
        //index = random int between [0, vec.size() - i - 1]
        int selection = rand() % ((int)vec.size() - i);
        out.push_back(vec[selection]);

        T swap = vec[vec.size() - i - 1];
        vec[vec.size() - i - 1] = vec[selection];
        vec[selection] = swap;
    }
    return out;
}

//Prints the seconds as XXh:XXm:XXs
static void print_as_time(double seconds){

    int seconds_out = int(seconds) % 60;
    int minutes_out = (int(seconds)/60) % 60;
    int hours_out = (int(seconds)/3600) % 60;

    std::string str_out = "Took : ";
    if(hours_out > 0) str_out += std::to_string(hours_out) + "h:";
    if(minutes_out > 0) str_out += std::to_string(minutes_out) + "m:";
    str_out += std::to_string(seconds_out) + "s\n";

    std::cout << str_out << std::endl;
}

#endif