#pragma once
#ifndef UTIL
#define UTIL

#include<vector>
#include<math.h>
#include<iostream>

#define LOG(x) std::cout << x << std::endl


static float rand_float(){ 
    return (float)(rand()%1001)/500.0f - 1.0f;
}

static float dot_prod(std::vector<float> a, std::vector<float> b){
    float out = 0;
    for(int i = 0; i < a.size(); i++){ out += a[i]*b[i]; }
    return out;
}


static std::vector<float> add_vec(std::vector<float> a, std::vector<float> b){
    for(int i = 0; i < a.size(); i++){ a[i] += b[i]; }
    return a;
}

static std::vector<float> mat_vec_mul(std::vector< std::vector < float > > mat, std::vector< float > vec){
    std::vector<float> out;
    for(std::vector<float> row : mat){
        out.push_back(dot_prod(row,vec));
    }
    return out;
}

static std::vector<float> apply_sigmoid(std::vector<float> vec){
    for(float &x : vec) x = 1/(1 + exp(-x));
    return vec;
}

static float der_sigmoid(float x){
    return exp(-x) / pow(1 + exp(-x), 2);
}

static int max_index(std::vector<float> vec) {
    int index = 0;
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] > vec[index]) index = i;
    }
    return index;
}

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


static void print_as_time(double seconds){

    printf("seconds: %f\n",seconds);

    int seconds_out = int(seconds) % 60;
    int minutes_out = (int(seconds)/60) % 60;
    int hours_out = (int(seconds)/3600) % 60;

    std::string str_out = "";
    if(hours_out > 0) str_out += std::to_string(hours_out) + "h:";
    if(minutes_out > 0) str_out += std::to_string(minutes_out) + "m:";
    str_out += std::to_string(seconds_out) + "s\n";

    std::cout << str_out;
}

#endif